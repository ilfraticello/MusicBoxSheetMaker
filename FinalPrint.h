#ifndef __FINAL_PRINT_H__
#define __FINAL_PRINT_H__

#include <sstream>
#include <map>
#include <set>
#include "MusicBox.h"

#ifndef CMMToPoint
#define CMMToPoint 2.8346
#endif

class TFinalPrint
{
public:
  TFinalPrint(std::map<int, std::set<int> >& sequence,
              TMusicBox& music_box,
              const char* text1,
              const char* text2,
              double text1_offset_mm,
              double text2_offset_mm,
              int bar_len = 480 * 4);
  virtual ~TFinalPrint();

  const std::string GetDoc() const { return m_doc.str(); }
  void Save(const char* filename);

private:
  TFinalPrint();
  void init();
  void parseSequence();
  void addNote(int time, int key);
  void finalize();

  double convertMMToPoint(double mm) { return mm * CMMToPoint; }
  double convertClockToMM(int clock) { return clock / 60.0; }
  double convertClockToPoint(int clock, double offsetMM)
    { return convertMMToPoint(convertClockToMM(clock) + offsetMM); }
  double keyToMM(int key, bool& sharp);

  // 480 clocks = 1/4 note = 8 mm

  std::map<int, std::set<int> >& m_sequence;
  TMusicBox& m_music_box;
  int m_bar_len;
  int m_finalized;
  int m_first_clock;
  int m_last_clock;
  int m_clock_width;
  int m_offset_mm;
  double m_paper_margin_mm;
  double m_box_horizontal_margin_mm;
  double m_box_virtical_margin_mm;
  double m_paper_width_mm;
  double m_paper_height_mm;
  const char* m_text1;
  const char* m_text2;
  double m_text1_offset_mm;
  double m_text2_offset_mm;
  std::stringstream m_graph_stream;
  std::stringstream m_text_stream;
  std::stringstream m_doc;
  int m_num_holes;

  std::map<int, double> m_key_to_y_mm;
};

#endif //__FINAL_PRINT_H__
