#include "MIDIReader.h"
#include "Validator.h"
#include "Draft.h"
#include "FinalPrint.h"
#include "Util.h"
#include <iostream>
#include <fstream>
#include <libgen.h>

static std::string note_name(int note)
{
  static const char* names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  std::string str(names[note % 12]);
  str += " (";
  str += std::to_string(note / 12 - 1);
  str += ") : ";
  str += std::to_string(note);

  return str;
}

void seq_dump(std::map<int, std::set<int> >& m)
{
  std::map<int, std::set<int> >::iterator it1;
  for (it1 = m.begin(); it1 != m.end(); ++it1)
  {
    std::set<int>::iterator it2;
    for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
    {
      std::cout << "time [" << it1->first << "] : key [" << note_name(*it2) << "]" << std::endl;
    }
  }
}

int test()
{
#if 0
  const char input[] =
  {
    0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x02, 0x01, 0xe0, 0x4d, 0x54,
    0x72, 0x6b, 0x00, 0x00, 0x00, 0x0b, 0x00, 0xff, 0x51, 0x03, 0x07, 0xa1, 0x20, 0x00, 0xff, 0x2f,
    0x00, 0x4d, 0x54, 0x72, 0x6b, 0x00, 0x00, 0x00, 0x7b, 0x00, 0xff, 0x03, 0x0c, 0x4e, 0x79, 0x6c,
    0x6f, 0x6e, 0x20, 0x47, 0x75, 0x69, 0x74, 0x61, 0x72, 0x00, 0xc0, 0x18, 0x00, 0xff, 0x58, 0x04,
    0x04, 0x02, 0x18, 0x08, 0x00, 0xff, 0x59, 0x02, 0x00, 0x00, 0x00, 0xb0, 0x65, 0x00, 0x00, 0xb0,
    0x64, 0x00, 0x00, 0xb0, 0x06, 0x06, 0x8d, 0x10, 0x90, 0x45, 0x4c, 0x81, 0x70, 0x80, 0x45, 0x40,
    0x81, 0x70, 0x90, 0x45, 0x4c, 0x81, 0x70, 0x80, 0x45, 0x40, 0x00, 0x90, 0x48, 0x4c, 0x81, 0x70,
    0x80, 0x48, 0x40, 0x00, 0x90, 0x45, 0x4c, 0x81, 0x70, 0x80, 0x45, 0x40, 0x00, 0x90, 0x47, 0x4c,
    0x81, 0x70, 0x80, 0x47, 0x40, 0x00, 0x90, 0x48, 0x4c, 0x81, 0x70, 0x80, 0x48, 0x40, 0x00, 0x90,
    0x4a, 0x4c, 0x81, 0x70, 0x80, 0x4a, 0x40, 0x00, 0x90, 0x43, 0x4c, 0x81, 0x70, 0x80, 0x43, 0x40,
    0x00, 0xff, 0x2f, 0x00, 0x0a
  };
  TMIDIReader seq(input, sizeof(input) - 1);

  TValidator validator;
  bool valid = validator.Validate(seq.getSequence(), 30, NULL);
  std::cout << "The MIDI data is " << (valid ?  "valid" : "invalid") << " as an input for 30 notes music box." << std::endl;

#endif

  return 0;
}

#define EMidi   0
#define ECsvSeq 1

static std::string
getStrOption(const std::string& key, const char** begin, const char** end) {
  std::string str;
  const char** it = std::find(begin, end, key);
  if (it != end && ++it != end && *it)
  {
    str = *it;
  }
  return str;
}

static int
getIntOption(const std::string& key, const char** begin, const char** end, int default_value) {
  std::string value = getStrOption(key, begin, end);
  return !value.empty() ? atoi(value.c_str()) : default_value;
}

static bool
findOption(const std::string& key, const char** begin, const char** end) {
  return (std::find(begin, end, key) != end);
}

static void
showUsage(const char* cmd) {
  std::cout << "Usage: " << cmd << " [options...]\n"
               "Options:\n"
               " -i FILE     Input MIDI file or input Sequence CSV file\n"
               " -s FILE     Output Sequence CSV file\n"
               " -v FILE     Output Validation result PDF file\n"
               " -o FILE     Output Final Print PDF file\n"
               " -k INTEGER  Key offset to shift note number (e.g. '-k -24' for 2 octave down)\n"
               " -t INTEGER  Timing stretch ratio (e.g. '-t 200' to convert 1/8 note into 1/4 note)\n"
               "             Default: 100 (%)\n"
               " -m TEXT     Text to be printed on Final Print (1st line)\n"
               " -n TEXT     Text to be printed on Final Print (2nd line)\n"
               " -p INTEGER  Offset of print location of the text specified by '-m' (1st line)\n"
               "             Deafult: 5 (mm)\n"
               " -q INTEGER  Offset of print location of the text specified by '-n' (2nd line)\n"
               "             Default: 30 (mm)\n"
               " -b INTEGER  The number of MIDI ticks in a single bar\n"
               "             E.g. 1920 for 480 bpqn with 4/4 time tune\n"
               "             Default: 1920\n"
               " -f          Force creation of the Final Print even if Validation found the issues\n"
               " -h          This help text\n";
}

int main(int argc, const char* argv[])
{
  int mode = EMidi;
  std::string input_file;
  std::string sequence_out_file;
  std::string validation_pdf_file;
  std::string output_pdf_file;
  int key_offset;
  std::string text1;
  std::string text2;
  int text1_offset;
  int text2_offset;
  int bar_len;
  int timing_strech_ratio;
  bool force_output = false;

  input_file          = getStrOption("-i", argv, argv + argc);
  sequence_out_file   = getStrOption("-s", argv, argv + argc);
  validation_pdf_file = getStrOption("-v", argv, argv + argc);
  output_pdf_file     = getStrOption("-o", argv, argv + argc);
  key_offset          = getIntOption("-k", argv, argv + argc, 0);
  text1               = getStrOption("-m", argv, argv + argc);
  text2               = getStrOption("-n", argv, argv + argc);
  text1_offset        = getIntOption("-p", argv, argv + argc, 5);
  text2_offset        = getIntOption("-q", argv, argv + argc, 30);
  bar_len             = getIntOption("-b", argv, argv + argc, 480 * 4);
  timing_strech_ratio = getIntOption("-t", argv, argv + argc, 100);
  force_output        = findOption("-f", argv, argv + argc);
  

  if (findOption("-h", argv, argv + argc) || input_file.empty()) {
    char buf[1024];
    showUsage(basename_r(argv[0], buf));
    return -1;
  }

  if (timing_strech_ratio < 1) {
    timing_strech_ratio = 1;
  }

  std::string filename(input_file);
  if (filename.find_last_of(".") != std::string::npos) {
    if (filename.substr(filename.find_last_of(".")) == ".csv") {
      mode = ECsvSeq;
    }
    filename = filename.substr(0, filename.find_last_of("."));
  }

  if (sequence_out_file.empty()) {
    sequence_out_file = filename + ".csv";
  }
  if (validation_pdf_file.empty()) {
    validation_pdf_file = filename + "_validation.pdf";
  }  
  if (output_pdf_file.empty()) {
    output_pdf_file = filename + "_final.pdf";
  }

  TMusicBox music_box = TMusicBox30();

  std::map<int, std::set<int> > sequence;

  if (mode == EMidi) {
    std::ifstream ifs(input_file.c_str());
    if (ifs.fail()) {
      std::cerr << "Failed to open file : " << input_file << std::endl;
      return -1;
    }
    std::cout << "Reading MIDI file : " << input_file << std::endl;
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    ifs.seekg(0);

    char *data = new char[size];
    ifs.read(data, size);

    TMIDIReader seq(data, size, key_offset, timing_strech_ratio);
    sequence = seq.getSequence();

    TUtil::ExportSequence(sequence, sequence_out_file.c_str(), bar_len);
    std::cout << "Exported the sequence file : "
              << sequence_out_file << std::endl;
  }
  else {
    std::cout << "Reading Sequence file : " << input_file << std::endl;
    sequence = TUtil::ImportSequence(input_file.c_str());
  }

  TValidator validator(music_box);

  bool valid = validator.Validate(sequence, bar_len, validation_pdf_file.c_str());
  std::cerr << "The data is " << (valid ?  "valid" : "invalid")
            << " as an input for " << music_box.NumKeys()
            << " notes music box." << std::endl;

  validator.ShowHistogram(sequence, 30);
  if (!valid && !force_output) {
    std::cout << "Final print PDF is not created. "
              << "Check the above analysis and the Validation PDF file : "
              << validation_pdf_file
              << std::endl;
    return -1;
  }
  
  // TODO consolidate text1, text2, ... into a config struct
  TFinalPrint final_print(sequence,
                          music_box,
                          text1.c_str(), text2.c_str(),
                          text1_offset, text2_offset,
                          bar_len);
  final_print.Save(output_pdf_file.c_str());
  std::cout << "Final print PDF is created : "
            << output_pdf_file
            << std::endl;

  return 0;
}
