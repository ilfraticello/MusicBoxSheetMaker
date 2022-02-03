#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

#include "MusicBox.h"
#include <map>
#include <set>

class TValidator
{
public:
  TValidator(TMusicBox& music_box);
  virtual ~TValidator() {}

  bool Validate(std::map<int, std::set<int> >& sequence,
                int bar_len,
                int minimum_distance,
                const char* outfile = NULL);
  bool ShowHistogram(std::map<int, std::set<int> >& sequence);

private:
  TMusicBox& m_music_box;
};

#endif //__VALIDATOR_H__
