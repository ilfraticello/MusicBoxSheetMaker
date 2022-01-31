#include "Util.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

std::string
TUtil::KeyName(int key)
{
  static const char* keys[] =
    {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

  std::string name(keys[key % 12]);
  name += " (";
  name += std::to_string(key / 12 - 1);
  name += ")";

  return name;
}

void
TUtil::ExportSequence(std::map<int, std::set<int> >& sequence, const char* filename, int bar_len)
{
  int nextBar = bar_len;

  std::ofstream ofs(filename);

  ofs << "# duration chart:\n"
         "#     1/4 note = 480 ticks (8mm)\n"
         "#     1/8 note = 240 ticks (4mm)\n"
         "#    1/32 note =  60 ticks (1mm)\n"
      << std::endl << std::endl;;

  int barCounter = 1;
  ofs << "# " << barCounter << std::endl;
  std::map<int, std::set<int> >::iterator it1;
  for (it1 = sequence.begin(); it1 != sequence.end(); ++it1)
  {
    int t = it1->first;
    std::set<int>& keys = it1->second;

    if (t >= nextBar) {
      ofs << "# " << ++barCounter << std::endl;
      nextBar += bar_len;
    }

    // reverse iterator, for readability of exported file
    std::set<int>::reverse_iterator it2;
    for (it2 = keys.rbegin(); it2 != keys.rend(); ++it2)
    {
      int key = *it2;
      ofs << t << " " << key << "  // " << KeyName(key) << std::endl;
    }
    ofs << std::endl;
  }
  ofs.close();
}

std::map<int, std::set<int> >
TUtil::ImportSequence(const char* filename)
{
  std::ifstream ifs(filename);

  std::map<int, std::set<int> > m;
  std::string line;
  while (getline(ifs, line)) {
    if (line.empty() || line.c_str()[0] == '#') {
      continue;
    }
    std::istringstream stream(line);
    std::string field;
    getline(stream, field, ' ');
    int t = stoi(field);
    getline(stream, field, ' ');
    int key = stoi(field);
//    std::cout << "Importing : " << t << "," << key << std::endl;
    if (m.find(t) == m.end()) {
      std::set<int> s;
      s.insert(key);
      m[t] = s;
    }
    else {
      m[t].insert(key);
    }
  }
  return m;
}
