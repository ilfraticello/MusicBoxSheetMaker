# MusicBoxSheetMaker

MusicBoxSheetMaker would assist you to create the music box sheet, which is inserted into a hand-crank music box.
Because of functional constraints of the music box, it is not easy to translate an arbitrary song or melody into the valid music box sheet. So this program would assist you with the valid music box sheet making.

The program would 
  1. Read the MIDI file,
  2. Validate the data and convert it into an interim CSV file, which you can edit manually,
  3. Create a printable PDF, which can be used as a template to make holes.

Finally you need to make the holes by yourself, and this is what the program cannot assist.


# DEMO

Assume we have a sample MIDI file, as an input.

```
% ./MusicBoxSheetMaker -i ../samples/sample_song.mid
```

The above command would create the following files in ../samples/
 - `sample_song.csv`
 - `sample_song_validation.pdf`
 - `sample_song_final.pdf`


# Hot to build

```bash
% git clone https://...
% cd build
% cmake ..
% make
```


# Installation

Copy the `MusicBoxSheetMaker` file into /usr/local/bin/ or wherever you want, if needed.


# Usage


To see the help
```bash
% ./MusicBoxSheetMaker -h
```

Futher details TBD
