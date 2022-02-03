#include "Draft.h"
#include "Util.h"
#include <iostream>

TDraft::TDraft(TMusicBox& music_box,
               int min_time,
               int max_time,
               int bar_len)
  : m_finalized(false), m_music_box(music_box),
    m_min_time(min_time), m_max_time(max_time), m_bar_len(bar_len),
    m_clock_width(max_time - min_time + 1),
    m_offset_mm(20 - convertClockToMM(min_time))
{
  init();
}

TDraft::~TDraft()
{}

void TDraft::init()
{
  m_graph_stream << "0 J "          // lineCap 0
                    "0 j "          // lineJoint 0
                 << std::endl;

  for (int n = 60; n <= 140; n += 2) {
    if ((n >= 90 && n <= 98) || (n >= 102 && n <= 110)) {
      m_graph_stream << "0.3 w 0 0 0 RG ";      // lineWidth 0.3, black
    }
    else {
      m_graph_stream << "0.2 w 0.7 0.7 1 RG ";  // lineWidth 0.2, light blue gray
    }

    // lines
    m_graph_stream << convertMMToPoint(10) << " "
                   << convertMMToPoint(n) << " m "
                   << convertMMToPoint(convertClockToMM(m_clock_width - 1) + 30) << " "
                   << convertMMToPoint(n) << " l S"
                   << std::endl;
  }
  m_graph_stream << std::endl;

  // bars
  for (int t = 0; t <= m_max_time; t += m_bar_len) {
    if (t < m_min_time) {
      continue;
    }
    double x = t / 60.0 + m_offset_mm;
    m_graph_stream << convertMMToPoint(x) << " "
                   << convertMMToPoint(60) << " m "
                   << convertMMToPoint(x) << " "
                   << convertMMToPoint(140) << " l S "
                   << std::endl;

    // add bar number
    if (t > m_min_time) {
      int bar_num = t / m_bar_len;
      int adj = (bar_num > 9) ? 1 : 0;
      m_graph_stream << "BT "
                        "/F1 6 Tf "
                     << convertMMToPoint(x - 2 - adj) << " "
                     << convertMMToPoint(142) << " Td "
                     << "(" << bar_num << ") Tj "
                     << "ET "
                     << std::endl;
    }
  }
  m_graph_stream << std::endl;

  // clefs
      m_graph_stream << "% drawing crefs\n"
                        "q 1 0 0 1 34.0533 260.9706 cm\n"
                        "0 0 m\n"
                        "1.8 1.414 3.583 2.951 4.678 5 c\n"
                        "6.216 7.749 6.788 11.314 5.264 14.175 c\n"
                        "3.579 17.226 -0.716 17.822 -3.612 16.055 c\n"
                        "-5.329 14.889 -6.211 12.72 -5.605 10.725 c\n"
                        "-5.346 9.858 -4.865 9.149 -3.971 8.861 c\n"
                        "-2.129 8.178 -0.305 9.646 -0.754 11.62 c\n"
                        "-0.987 12.975 -2.276 13.581 -3.55 13.295 c\n"
                        "-4.222 13.083 -4.516 13.49 -4.254 14.112 c\n"
                        "-3.641 15.647 -1.866 16.392 -0.294 16.239 c\n"
                        "2.304 15.946 2.731 12.867 2.822 10.747 c\n"
                        "2.822 3.515 -0.177 0.903 -5.81 -2.765 c\n"
                        "-6.449 -4.351 -0.382 -0.249 0 0 c\n"
                        "f\n"
                        "Q\n"
                        "q 1 0 0 1 36.0506 308.3863 cm\n"
                        "0 0 m\n"
                        "-1.358 9.853 7.104 9.838 3.291 2.965 c\n"
                        "1.82 0.707 -0.46 -0.616 -2.499 -2.352 c\n"
                        "-5.09 -4.726 -7.689 -7.683 -7.704 -11.396 c\n"
                        "-7.845 -16.747 -3.193 -19.848 1.82 -19.179 c\n"
                        "7.92 -18.493 9.739 -10.442 3.733 -8.141 c\n"
                        "0.472 -6.912 -3.551 -9.465 -3.13 -13.069 c\n"
                        "-2.946 -14.694 -2.058 -16.138 -0.408 -16.582 c\n"
                        "0.15 -16.787 0.51 -16.413 -0.156 -16.142 c\n"
                        "-1.311 -15.598 -1.853 -14.375 -1.64 -13.144 c\n"
                        "-1.267 -11.423 0.818 -10.715 2.367 -10.891 c\n"
                        "6.939 -11.686 6.521 -17.785 1.906 -18.348 c\n"
                        "-5.381 -19.506 -8.539 -12.709 -3.569 -7.511 c\n"
                        "-1.951 -5.612 0.339 -4.306 2.054 -2.522 c\n"
                        "4.596 0.201 5.87 4.17 4.766 7.802 c\n"
                        "4.547 8.703 3.172 11.992 2.064 11.036 c\n"
                        "0.604 9.865 -0.425 8.249 -0.736 6.394 c\n"
                        "-1.253 4.122 -1.055 1.812 -0.79 -0.477 c\n"
                        "-0.348 -4.195 3.204 -21.275 2.983 -24.696 c\n"
                        "2.825 -25.828 2.375 -26.781 1.289 -27.289 c\n"
                        "0.187 -27.773 -1.191 -27.895 -2.252 -27.264 c\n"
                        "-3.094 -26.458 -0.159 -26.874 -0.205 -24.568 c\n"
                        "-0.001 -21.75 -3.952 -21.36 -4.601 -23.919 c\n"
                        "-5.946 -29.125 3.068 -30.127 3.812 -25.245 c\n"
                        "4.359 -21.441 0.444 -4.081 0 0 c\n"
                        "f\n"
                        "Q\n"
                        "q 1 0 0 1 42.4392 273.7624 cm\n"
                        "0 0 m\n"
                        "-0.69 0.004 -1.201 0.513 -1.2 1.197 c\n"
                        "-1.2 1.877 -0.655 2.407 0.034 2.398 c\n"
                        "0.684 2.39 1.221 1.851 1.223 1.204 c\n"
                        "1.225 0.507 0.706 -0.004 0 0 c\n"
                        "f\n"
                        "Q\n"
                        "q 1 0 0 1 42.4813 268.1107 cm\n"
                        "0 0 m\n"
                        "-0.682 -0.002 -1.191 0.496 -1.194 1.168 c\n"
                        "-1.197 1.859 -0.688 2.376 -0.006 2.378 c\n"
                        "0.649 2.379 1.181 1.845 1.181 1.188 c\n"
                        "1.181 0.52 0.666 0.002 0 0 c\n"
                        "f\n"
                        "Q\n"
                     << std::endl;

      m_graph_stream << "% explaining texts\n"
                     << "BT q "
                        "/F1 8 Tf "
                     << convertMMToPoint(20) << " "
                     << convertMMToPoint(44) << " Td "
                     << "(A red note indicate it is the key which is not supported by the music box.) Tj "
                     << "Q ET \n"

                     << "BT q "
                        "/F1 8 Tf "
                     << convertMMToPoint(20) << " "
                     << convertMMToPoint(40) << " Td "
                     << "(An amber note indicate it doesn't have enough preceding space after the previous note at the same key.) Tj "
                     << "Q ET \n"
                     << std::endl;

      double cx = 17;
      double cy = 44.8;
      m_graph_stream << "q 0.1 w 0 J 0 j "
                     << "1 0 0 RG 1 0 0 rg "

                     // start
                     << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy + 1.0)) << " m "

                     // 1st arc 0-3 o'clock
                     << (convertMMToPoint(cx + 0.552)) << " " << (convertMMToPoint(cy + 1.0)) << " "
                     << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy + 0.552)) << " "
                     << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy)) << " c "

                     // 2nd arc 3-6 o'clock
                     << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy - 0.552)) << " "
                     << (convertMMToPoint(cx + 0.552)) << " " << (convertMMToPoint(cy - 1.0)) << " "
                     << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy - 1.0)) << " c "

                     // 3rd arc 6-9 o'clock
                     << (convertMMToPoint(cx - 0.552)) << " " << (convertMMToPoint(cy - 1.0)) << " "
                     << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy - 0.552)) << " "
                     << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy)) << " c "

                     // 4th arc 9-12 o'clock
                     << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy + 0.552)) << " "
                     << (convertMMToPoint(cx - 0.552)) << " " << (convertMMToPoint(cy + 1.0)) << " "
                     << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy + 1.0)) << " c "

                     // closing
                     << "b Q" << std::endl;

      cy = 40.8;
      m_graph_stream << "q 0.1 w 0 J 0 j "
                     << "0.8 0.5 0 RG 1 0.8 0 rg "

                     // start
                     << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy + 1.0)) << " m "

                     // 1st arc 0-3 o'clock
                     << (convertMMToPoint(cx + 0.552)) << " " << (convertMMToPoint(cy + 1.0)) << " "
                     << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy + 0.552)) << " "
                     << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy)) << " c "

                     // 2nd arc 3-6 o'clock
                     << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy - 0.552)) << " "
                     << (convertMMToPoint(cx + 0.552)) << " " << (convertMMToPoint(cy - 1.0)) << " "
                     << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy - 1.0)) << " c "

                     // 3rd arc 6-9 o'clock
                     << (convertMMToPoint(cx - 0.552)) << " " << (convertMMToPoint(cy - 1.0)) << " "
                     << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy - 0.552)) << " "
                     << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy)) << " c "

                     // 4th arc 9-12 o'clock
                     << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy + 0.552)) << " "
                     << (convertMMToPoint(cx - 0.552)) << " " << (convertMMToPoint(cy + 1.0)) << " "
                     << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy + 1.0)) << " c "

                     // closing
                     << "b Q" << std::endl;

}

void TDraft::Finalize()
{
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
        << convertMMToPoint(convertClockToMM(m_clock_width - 1) + 40)
        << " "
        << convertMMToPoint(200)
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
        << m_graph_stream.str().length()
        << " >>\n"
           "stream\n"
        << m_graph_stream.str()
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
           "  >>\n"
           "%%EOF\n";

  m_finalized = true;
}

double TDraft::keyToMM(int key, bool& sharp)
{
  int octave = key / 12;
  int key_in_octave = ((key % 12) + ((key % 12) > 4 ? 1 : 0)) / 2;
  sharp = ((key % 12) + ((key % 12) > 4 ? 1 : 0)) % 2;

  double mm = 58.0 + 7 * octave + key_in_octave;
//  std::cout << "key " << key << " -> octave " << octave << " "
//            << (sharp ? "#" : "") << key_in_octave << std::endl;
  return mm;
}

void TDraft::AddNote(int time, int key, int warning)
{
  bool sharp = false;
  double cx = convertClockToMM(time) + m_offset_mm;
  double cy = keyToMM(key, sharp);
  double lw = convertMMToPoint(1.8);

  m_graph_stream << "% time[" << time << "] " << TUtil::KeyName(key) << std::endl;

  m_graph_stream << "q ";

  // a circle (4 quarter arcs)
  m_graph_stream << "0.1 w 0 J 0 j "
                 << (!warning ? "0 0 0 RG 0.5 0.5 0.5 rg " :
                        (warning == 1 ? "0.8 0.5 0 RG 1 0.8 0 rg " : "1 0 0 RG 1 0 0 rg "))
                 // start
                 << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy + 1.0)) << " m "

                 // 1st arc 0-3 o'clock
                 << (convertMMToPoint(cx + 0.552)) << " " << (convertMMToPoint(cy + 1.0)) << " "
                 << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy + 0.552)) << " "
                 << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy)) << " c "

                 // 2nd arc 3-6 o'clock
                 << (convertMMToPoint(cx + 1.0)) << " " << (convertMMToPoint(cy - 0.552)) << " "
                 << (convertMMToPoint(cx + 0.552)) << " " << (convertMMToPoint(cy - 1.0)) << " "
                 << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy - 1.0)) << " c "

                 // 3rd arc 6-9 o'clock
                 << (convertMMToPoint(cx - 0.552)) << " " << (convertMMToPoint(cy - 1.0)) << " "
                 << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy - 0.552)) << " "
                 << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy)) << " c "

                 // 4th arc 9-12 o'clock
                 << (convertMMToPoint(cx - 1.0)) << " " << (convertMMToPoint(cy + 0.552)) << " "
                 << (convertMMToPoint(cx - 0.552)) << " " << (convertMMToPoint(cy + 1.0)) << " "
                 << (convertMMToPoint(cx)) << " " << (convertMMToPoint(cy + 1.0)) << " c "

                 // closing
                 << "b" << std::endl;

  if (sharp) {
    // draw 4 lines for a sharp (#)
    m_graph_stream << "0.5 w 0 J 0 j" << std::endl;
    m_graph_stream << convertMMToPoint(cx - 2.0) << " "
                   << convertMMToPoint(cy - 0.5) << " m "
                   << convertMMToPoint(cx - 1.0) << " "
                   << convertMMToPoint(cy - 0.4) << " l S"
                   << std::endl;
    m_graph_stream << convertMMToPoint(cx - 2.0) << " "
                   << convertMMToPoint(cy + 0.4) << " m "
                   << convertMMToPoint(cx - 1.0) << " "
                   << convertMMToPoint(cy + 0.5) << " l S"
                   << std::endl;
    m_graph_stream << convertMMToPoint(cx - 1.7) << " "
                   << convertMMToPoint(cy - 0.8) << " m "
                   << convertMMToPoint(cx - 1.7) << " "
                   << convertMMToPoint(cy + 0.7) << " l S"
                   << std::endl;
    m_graph_stream << convertMMToPoint(cx - 1.3) << " "
                   << convertMMToPoint(cy - 0.7) << " m "
                   << convertMMToPoint(cx - 1.3) << " "
                   << convertMMToPoint(cy + 0.8) << " l S"
                   << std::endl;
  }
  m_graph_stream << "Q" << std::endl;

  m_graph_stream << std::endl;
}
