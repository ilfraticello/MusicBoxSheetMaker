#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <map>
#include <set>

namespace TUtil {
  std::string KeyName(int key);

  void ExportSequence(std::map<int, std::set<int> >& sequence,
                      const char* filename,
                      int bar_len = 480 * 4);

  std::map<int, std::set<int> >
      ImportSequence(const char* filename);

};

#endif //__UTIL_H__
