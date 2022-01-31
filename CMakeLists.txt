cmake_minimum_required(VERSION 3.1)

project(MusicBoxSheetMaker CXX)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=native")


add_executable(MusicBoxSheetMaker
  main.cpp
  Util.cpp
  MusicBox.cpp
  MIDIReader.cpp
  Validator.cpp
  Draft.cpp
  FinalPrint.cpp)
