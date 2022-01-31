#ifndef __MIDI_READER_H__
#define __MIDI_READER_H__

#include <map>
#include <set>

class TMIDIReader {
public:
  TMIDIReader(const char* midi_data,
              size_t len,
              int offset = 0,
              int timing_strech_ratio = 100);
  virtual ~TMIDIReader();

  std::map<int, std::set<int> >& getSequence() { return m_sequence; }

private:
  TMIDIReader();

  size_t readHeader(const char* ptr, size_t available);
  size_t readTrack(const char* ptr, size_t available);

  size_t read4BytesInt(const char* ptr, size_t available, uint32_t& num);
  size_t readVarInt(const char* ptr, size_t available, uint64_t& num);

  std::map<int, std::set<int> > m_sequence;
  int m_offset;
  int m_timing_strech_ratio;
};

#endif //__MIDI_READER_H__
