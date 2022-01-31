#include "FinalPrint.h"
#include "Util.h"
#include <iostream>
#include <fstream>

TFinalPrint::TFinalPrint(std::map<int, std::set<int> >& sequence,
                         TMusicBox& music_box,
                         const char* text1,
                         const char* text2,
                         double text1_offset_mm,
                         double text2_offset_mm,
                         int bar_len)
  : m_sequence(sequence),
    m_music_box(music_box),
    m_bar_len(bar_len),
    m_finalized(false), 
    m_first_clock(0),
    m_last_clock(0),
    m_clock_width(0), m_offset_mm(0),
    m_paper_margin_mm(10.0),
    m_box_horizontal_margin_mm(30.0), m_box_virtical_margin_mm(6.0),
    m_paper_width_mm(0.0), m_paper_height_mm(0.0),
    m_text1(text1), m_text2(text2),
    m_text1_offset_mm(text1_offset_mm), m_text2_offset_mm(text2_offset_mm),
    m_num_holes(0)
{
  init();
  parseSequence();
  finalize();
}

TFinalPrint::~TFinalPrint()
{}

void
TFinalPrint::init()
{
  std::set<int> keys = m_music_box.GetKeys();
  std::set<int>::iterator key;
  double y = m_paper_margin_mm + m_box_virtical_margin_mm;
  for (key = keys.begin(); key != keys.end(); ++key) {
    m_key_to_y_mm[*key] = y;
    y += 2.0;
  }

  m_first_clock = m_sequence.begin()->first;
  m_last_clock = m_sequence.rbegin()->first;
  m_clock_width = m_last_clock - m_first_clock + 1;
  m_offset_mm = 0 - convertClockToMM(m_first_clock);

  // 480 clocks = 1/4 note = 8 mm
  // 60 clocks = 1mm

  // distance between left-most hole center to right-most hole center
  double score_net_width_mm = m_clock_width / 60.0;

  double box_width_mm = score_net_width_mm + m_box_horizontal_margin_mm * 2;
  double box_height_mm = 70.0; // fixed
  m_paper_width_mm = box_width_mm + m_paper_margin_mm * 2;
  m_paper_height_mm = box_height_mm + m_paper_margin_mm * 2;
  
  m_graph_stream << "0 J "          // lineCap 0
                    "0 j "          // lineJoint 0
                 << std::endl;

  // box
  m_graph_stream << convertMMToPoint(m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_margin_mm) << " "
                 << convertMMToPoint(box_width_mm) << " "
                 << convertMMToPoint(box_height_mm) << " re S"
                 << std::endl
                 << std::endl;

  // tombo
  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(m_paper_margin_mm) << " "
                 << convertMMToPoint(0) << " m "
                 << convertMMToPoint(m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_margin_mm * 0.8) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_height_mm) << " m "
                 << convertMMToPoint(m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm * 0.8) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(m_paper_width_mm - m_paper_margin_mm) << " "
                 << convertMMToPoint(0) << " m "
                 << convertMMToPoint(m_paper_width_mm - m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_margin_mm * 0.8) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(m_paper_width_mm - m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_height_mm) << " m "
                 << convertMMToPoint(m_paper_width_mm - m_paper_margin_mm) << " "
                 << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm * 0.8) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(0) << " "
                 << convertMMToPoint(m_paper_margin_mm) << " m "
                 << convertMMToPoint(m_paper_margin_mm * 0.8) << " "
                 << convertMMToPoint(m_paper_margin_mm) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(0) << " "
                 << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm) << " m "
                 << convertMMToPoint(m_paper_margin_mm * 0.8) << " "
                 << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(m_paper_width_mm) << " "
                 << convertMMToPoint(m_paper_margin_mm) << " m "
                 << convertMMToPoint(m_paper_width_mm - m_paper_margin_mm * 0.8) << " "
                 << convertMMToPoint(m_paper_margin_mm) << " l S "
                 << std::endl;

  m_graph_stream << "0.1 w 0 0 0 RG "   // lineWidth 0.1, black
                 << convertMMToPoint(m_paper_width_mm) << " "
                 << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm) << " m "
                 << convertMMToPoint(m_paper_width_mm - m_paper_margin_mm * 0.8) << " "
                 << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm) << " l S "
                 << std::endl;

  // texts
  m_text_stream << "BT q "
                   "/F1 14 Tf "
                   "0 1 -1 0 0 0 Tm "
                << convertMMToPoint(m_paper_margin_mm + m_text1_offset_mm) << " "
                << convertMMToPoint(-m_paper_margin_mm - 14) << " Td "
                << "(" << m_text1 << ") Tj "
                << "Q ET "
                << std::endl;  

  m_text_stream << "BT q "
                   "/F1 14 Tf "
                   "0 1 -1 0 0 0 Tm "
                << convertMMToPoint(m_paper_margin_mm + m_text2_offset_mm) << " "
                << convertMMToPoint(-m_paper_margin_mm - 22) << " Td "
                << "(" << m_text2 << ") Tj "
                << "Q ET "
                << std::endl;

  // horizontal lines
  for (double y = m_paper_margin_mm + m_box_virtical_margin_mm;
       y <= m_paper_height_mm - m_paper_margin_mm - m_box_virtical_margin_mm;
       y += 2.0) {

    double left_x = m_paper_margin_mm + m_box_horizontal_margin_mm - 4;
    double right_x = left_x + score_net_width_mm + 8;
    m_graph_stream << "0.3 w 0 0 0 RG "   // lineWidth 0.3, black
                   << convertMMToPoint(left_x) << " "
                   << convertMMToPoint(y) << " m "
                   << convertMMToPoint(right_x) << " "
                   << convertMMToPoint(y) << " l S"
                   << std::endl;
  }
  m_graph_stream << std::endl;

  // vertical lines
  for (int t = 0; t <= m_last_clock; t += 480) {
    if (t < m_first_clock) {
      continue;
    }
    double x = t / 60.0
             + m_paper_margin_mm + m_box_horizontal_margin_mm + m_offset_mm;
    double low_y = m_paper_margin_mm + m_box_virtical_margin_mm;
    double high_y = m_paper_height_mm - m_paper_margin_mm - m_box_virtical_margin_mm;
    if (t % m_bar_len == 0) {
      double adj = ((int)t / m_bar_len > 9) ? 1.5 : 0;
      m_graph_stream << "0.3 w 0 0 0 RG ";   // lineWidth 0.3, black

      m_text_stream << "BT "
                       "/F1 6 Tf "
                    << convertMMToPoint(x - 2 - adj) << " "
                    << convertMMToPoint(high_y + 2) << " Td "
                    << "(" << ((int)t / m_bar_len) << ") Tj "
                    << "ET "
                    << std::endl;
    }
    else {
      m_graph_stream << "0.1 w 0.5 0.5 0.5 RG ";   // lineWidth 0.1, 50% gray
    }
    m_graph_stream << convertMMToPoint(x) << " "
                   << convertMMToPoint(low_y) << " m "
                   << convertMMToPoint(x) << " "
                   << convertMMToPoint(high_y) << " l S"
                   << std::endl;
  }
  m_graph_stream << std::endl;
}

void
TFinalPrint::finalize()
{
  m_text_stream << "BT "
                   "/F1 8 Tf "
                << convertMMToPoint(m_paper_margin_mm + 1) << " "
                << convertMMToPoint(m_paper_height_mm - m_paper_margin_mm + 1) << " Td "
                << "(" << "Number of holes : " << m_num_holes << ") Tj "
                << "ET "
                << std::endl;

  m_doc << "%PDF-1.4\n"
           "1 0 obj\n"
           "  << /Type /Catalog\n"
           "     /Outlines 2 0 R\n"
           "     /Pages 3 0 R\n"
           "  >>\n"
           "endobj\n"
           "\n";

  m_doc << "2 0 obj\n"
           "  << /Type Outlines\n"
           "     /Count 0\n"
           "  >>\n"
           "endobj\n"
           "\n";

  m_doc << "3 0 obj\n"
           "  << /Type /Pages\n"
           "     /Kids [4 0 R]\n"
           "     /Count 1\n"
           "  >>\n"
           "endobj\n"
           "\n";

  m_doc << "4 0 obj\n"
           "  << /Type /Page\n"
           "     /Parent 3 0 R\n"
           "     /MediaBox [0 0 "
        << convertMMToPoint(m_paper_width_mm)
        << " "
        << convertMMToPoint(m_paper_height_mm)
        << "]\n"
           "     /Contents 5 0 R\n"
           "     /Resources << /ProcSet 6 0 R\n"
           "                   /Font << /F1 7 0 R >>\n"
           "                >>\n"
           "  >>\n"
           "endobj\n"
           "\n";

  m_doc << "5 0 obj\n"
           "  << /Length "
        << (m_graph_stream.str().length() + m_text_stream.str().length())
        << " >>\n"
           "stream\n"
        << m_graph_stream.str()
        << m_text_stream.str()
        << "endstream\n"
           "endobj\n"
	       "\n";

  m_doc << "6 0 obj\n"
           "  [/PDF /Text]\n"
           "endobj\n"
           "\n";

  m_doc << "7 0 obj\n"
           "  << /Type /Font\n"
           "     /Subtype /Type1\n"
           "     /Name /F1\n"
           "     /BaseFont /Helvetica\n"
           "     /Encoding /MacRomanEncoding\n"
           "  >>\n"
           "endobj\n"
           "\n";

  m_doc << "trailer\n"
           "  <<\n"
           "    /Root 1 0 R\n"
           "  >>\n";

  m_doc << "%%EOF\n";

  m_finalized = true;
  std::cout << "finalized !" << std::endl;
}

void
TFinalPrint::addNote(int time, int key)
{
  if (m_key_to_y_mm.find(key) == m_key_to_y_mm.end()) {
    assert(false);
  }
  double cx = convertClockToMM(time) + m_offset_mm
            + m_paper_margin_mm + m_box_horizontal_margin_mm;
  double cy = m_key_to_y_mm[key];
  double lw = convertMMToPoint(1.8);

  m_graph_stream << "% time[" << time << "] " << TUtil::KeyName(key) << std::endl;

  // a circle (by zero-size rect with round edge line) for a note
  m_graph_stream << lw << " w 1 J 1 j "
                 << "0 0 0 RG "
                 << convertMMToPoint(cx) << " " << convertMMToPoint(cy) << " 0 0 re S"
                 << std::endl;

  m_graph_stream << std::endl;

  ++m_num_holes;
}

void
TFinalPrint::parseSequence()
{
  std::map<int, std::set<int> >::iterator it1;
  for (it1 = m_sequence.begin(); it1 != m_sequence.end(); ++it1) {
    int t = it1->first;
    std::set<int>::iterator it2;
    for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
      int note = *it2;
      if (m_key_to_y_mm.find(note) == m_key_to_y_mm.end()) {
        std::cout << "skipping an invalid note : time " << t << " / key "
                  << TUtil::KeyName(note) << std::endl;
      }
      else {
        addNote(t, note);
      }
    }
  }
}

void
TFinalPrint::Save(const char* filename)
{
  if (!m_finalized) {
    std::cout << "not finalized. cannot save to file" << std::endl;
    return;
  }
  if (filename) {
    std::ofstream ofs(filename);
    ofs << m_doc.str();
    ofs.close();
  }
}
