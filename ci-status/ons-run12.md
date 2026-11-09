# ons run 12

sha: 532c6e0cdc29b966c4d891cacaa13dd442a9014c
status: failure
at: 2026-11-09T00:05:07Z

## errors

Modules/ONScripter/smpeg/aurora_smpeg.mm:54:23: error: no matching function for call to 'AudioFileOpenWithCallbacks'
FAILED: aurora_smpeg.mm
third_party/onscripter/ONScripterLabel_sound.cpp:695:11: error: cannot initialize a variable of type 'char *' with an rvalue of type 'const char *'
third_party/onscripter/ONScripterLabel_sound.cpp:799:13: error: use of undeclared identifier 'SMPEG_loop'
FAILED: ONScripterLabel_sound.cpp

## stack tail
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'sdl12_compat')
** BUILD SUCCEEDED **

--- installed headers ---
SDL
SDL2
--- installed libraries ---
cmake
libSDL-1.2.a
libSDL.dylib
libSDL2_test.a
libSDL2.a
libSDL2main.a
libSDLmain.a
pkgconfig

## summary
2 errors generated.
FAILED: ONScripterLabel_sound.cpp
compiled=36 failed=1
