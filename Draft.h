#ifndef __DRAFT_H__
#define __DRAFT_H__

#include <sstream>
#include "MusicBox.h"

#ifndef CMMToPoint
#define CMMToPoint 2.8346
#endif

class TDraft
{
public:
  TDraft(TMusicBox& music_box,
         int min_time,
         int max_time,
         int bar_len,
         int timing_strech);
  virtual ~TDraft();

  void AddNote(int time, int key, int warning);
  void Finalize();
  const std::string GetDoc() const { return m_doc.str(); }

private:
  TDraft();
  void init();
  double convertMMToPoint(double mm) { return mm * CMMToPoint; }
  double convertClockToMM(int clock) { return clock / 60.0; }
  double convertClockToPoint(int clock, double offsetMM)
    { return convertMMToPoint(convertClockToMM(clock) + offsetMM); }
  double keyToMM(int key, bool& sharp);

  // 480 clocks = 1/4 note = 8 mm
  int m_finalized;
  TMusicBox& m_music_box;
  int m_min_time;
  int m_max_time;
  int m_bar_len;
  int m_timing_strech;
  int m_clock_width;
  int m_offset_mm;
  std::stringstream m_graph_stream;
  std::stringstream m_doc;
};

#endif //__DRAFT_H__
