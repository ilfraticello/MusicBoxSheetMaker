#include "MIDIReader.h"
#include <assert.h>
#include <iostream>

TMIDIReader::TMIDIReader(const char* midi_data, size_t len, int offset, int timing_strech_ratio)
  : m_offset(offset), m_timing_strech_ratio(timing_strech_ratio)
{
  const char* ptr = midi_data;
  size_t available = len;

  size_t consumed = readHeader(ptr, available);
  assert(available >= consumed);
  ptr += consumed;
  available -= consumed;

  while (available > 0) {
    consumed = readTrack(ptr, available);
    assert(available >= consumed);
    ptr += consumed;
    available -= consumed;
  }
}

TMIDIReader::~TMIDIReader()
{
}

size_t
TMIDIReader::readHeader(const char* ptr, size_t available)
{
  assert(available >= 14);
  assert(!memcmp(ptr, "MThd", 4));
  return 14;
}

size_t
TMIDIReader::readTrack(const char* ptr, size_t available)
{
  assert(available >= 8);
  assert(!memcmp(ptr, "MTrk", 4));

  uint64_t abs_time = 0;

  size_t cur = 4;
  uint32_t len = 0;
  size_t consumed = read4BytesInt(ptr + cur, available - cur, len);
  cur += consumed;
  assert(cur <= available);
  assert(len <= available - cur);

  while (cur < 8 + len) {
    uint64_t delta_time = 0;
    consumed = readVarInt(ptr + cur, available - cur, delta_time);
    cur += consumed;
    delta_time = (double)delta_time * (m_timing_strech_ratio / 100.0);
    abs_time += delta_time;

    assert(cur + 3 <= available);

    unsigned char chunk_type = ptr[cur++];
    assert((chunk_type & 0x80) == 0x80);
    if ((chunk_type & 0xf0) == 0x90) {
      // we are interested in "note on" event only
      unsigned char key = ptr[cur++];
      unsigned char velocity = ptr[cur++];
      int key_adj = (int)key + m_offset;
      if (m_sequence.find(abs_time) == m_sequence.end()) {
        std::set<int> s;
        s.insert(key_adj);
        m_sequence.insert(std::make_pair(abs_time, s));
      }
      // avoid duplicate
      else if (m_sequence[abs_time].find(key_adj) == m_sequence[abs_time].end()) {
        m_sequence[abs_time].insert(key_adj);
      }
    }
    else
    {
      // we will ignore others
      if ((chunk_type & 0xff) == 0xff) {
        unsigned char sub_type = ptr[cur++];
        if (sub_type == 0x00 || sub_type == 0x2f) {
          ++cur; // skip one byte
        }
        else if (sub_type == 0x51) {
          cur += 4;
        }
        else if (sub_type == 0x58) {
          cur += 5;
        }
        else if (sub_type == 0x59) {
          cur += 3;
        }
        else {
          uint64_t skip_bytes = 0;
          cur += readVarInt(ptr + cur, available - cur, skip_bytes);
          cur += skip_bytes;
        }
      }
      else if ((chunk_type & 0xf0) == 0xc0) {
        cur += 1;
      }
      else {
        cur += 2;
      }
    }
  }
  return cur;
}

size_t
TMIDIReader::readVarInt(const char* ptr, size_t available, uint64_t& num)
{
  assert(available >= 1);
  size_t cur = 0;
  num = 0;

  while (cur < available) {
    num <<= 7;
    unsigned char n = ptr[cur++];
    if ((n & 0x80) == 0) {
      num += n;
      break;
    }
    n &= 0x7f;
    num += n;
  }

  return cur;
}

size_t
TMIDIReader::read4BytesInt(const char* ptr, size_t available, uint32_t& num)
{
  assert(available >= 4);
  num = 0;
  for (int i = 0; i < 4; ++i) {
    num <<= 8;
    num += (unsigned char)ptr[i];
  }
  return 4;
}
