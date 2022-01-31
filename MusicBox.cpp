#include "MusicBox.h"

TMusicBox30::TMusicBox30()
{
  m_keys = {
    // Note number 60 (middle C) is C4 in GM specification
    // but it is C3 in Yamaha XG, and here we take the latter.
    // Note number should be adjusted when reading MIDI by key offset.
    53, 55,                                          // 2: F,G
    60, 62, 64, 65, 67, 69, 70, 71,                  // 3: C,D,E,F,G,A,A#,B
    72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,  // 4: C,C#,D,D#,E,F,F#,G,G#,A,A#,B
    84, 85, 86, 87, 88, 89, 91, 93                   // 5: C,C#,D,D#,E,F,F#,G,G#,A,A#,B
  };
}
