#ifndef __MUSIC_BOX_H__
#define __MUSIC_BOX_H__

#include <set>

class TMusicBox
{
public:
  TMusicBox() : m_offset(0) {}
  virtual ~TMusicBox() {}

  bool IsValidKey(int key) const
      { return (m_keys.find(key) != m_keys.end()); }

  std::set<int> GetKeys() const { return m_keys; }

  int NumKeys() const { return m_keys.size(); }

  int GetOffset() const { return m_offset; }

protected:
  std::set<int> m_keys;
  int m_offset;
};

class TMusicBox30 : public TMusicBox
{
public:
  TMusicBox30();
  virtual ~TMusicBox30() {}
};

#endif //__MUSIC_BOX_H__
