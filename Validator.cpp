#include "Validator.h"
#include "Draft.h"
#include "Util.h"
#include <fstream>
#include <unordered_set>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

TValidator::TValidator(TMusicBox& music_box)
  : m_music_box(music_box)
{}

bool
TValidator::Validate(std::map<int, std::set<int> >& sequence,
                             int bar_len,
                             int timing_stretch,
                             int minimum_distance,
                             const char* outfile)
{
  std::map<int, int> last_timestamp_per_note;

  if (m_music_box.NumKeys() != 30) {
    // TODO assert or throw
    // not supported
    return false;
  }

  TDraft draft(m_music_box, sequence.begin()->first, sequence.rbegin()->first, bar_len, timing_stretch);

  bool valid = true;
  std::map<int, std::set<int> >::iterator it1;
  for (it1 = sequence.begin(); it1 != sequence.end(); ++it1) {
    int t = it1->first;
    std::set<int>::iterator it2;
    for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
      int note = *it2;
      if (!m_music_box.IsValidKey(note)) {
        valid = false;
        draft.AddNote(t, note, 2);
      }
      else {
        if (last_timestamp_per_note.find(note) != last_timestamp_per_note.end() &&
            t - last_timestamp_per_note[note] < minimum_distance) {
          valid = false;
          draft.AddNote(t, note, 1);
        }
        else {
          draft.AddNote(t, note, 0);
        }
        last_timestamp_per_note[note] = t;
      }
    }
  }

  draft.Finalize();

  if (outfile) {
    std::ofstream ofs(outfile);
    ofs << draft.GetDoc();
    ofs.close();
  }

  return valid;
}

bool TValidator::ShowHistogram(std::map<int, std::set<int> >& sequence)
{
  if (m_music_box.NumKeys() != 30) {
    // TODO assert or throw
    // not supported
    return false;
  }

  std::set<int> keys = m_music_box.GetKeys();
  std::map<int, int> m;
  std::set<int>::iterator di;
  for (di = keys.begin(); di != keys.end(); ++di) {
    m[*di] = 0;
  }

  std::map<int, std::set<int> >::iterator it1;
  for (it1 = sequence.begin(); it1 != sequence.end(); ++it1) {
    std::set<int>::iterator it2;
    for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
      int note = *it2;
      if (m.find(note) == m.end()) {
        m[note] = 1;
      }
      else {
        ++m[note];
      }
    }
  }

  std::map<int, int>::reverse_iterator mi;
  for (mi = m.rbegin(); mi != m.rend(); ++mi) {
    bool valid = false;
    char c = '-';
    if (m_music_box.IsValidKey(mi->first)) {
      valid = true;
      c = 'O';
    }
    int note = mi->first;
    int amount = mi->second;
    char buf[40];
    memset(buf, 0, 40);
    if (amount < 39) {
      memset(buf, c, amount);
      buf[amount] = '\0';
    }
    else {
      memset(buf, c, 36);
      memset(buf + 36, '.', 3);
      buf[39] = '\0';
    }
    fprintf(stdout, "%6s: %s%s\n",
            TUtil::KeyName(note).c_str(), buf, valid ? "" : "   <-- unsupported key");
  }
  fflush(stdout);
  return true; 
}
