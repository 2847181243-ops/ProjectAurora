# ons run 13

sha: 94046ffcdfbb2662414b82809149c59b7702e6c4
status: failure
at: 2026-11-09T00:12:24Z

## errors

Modules/ONScripter/smpeg/aurora_smpeg.mm:55:23: error: no matching function for call to 'AudioFileOpenWithCallbacks'
FAILED: aurora_smpeg.mm

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
1 error generated.
FAILED: aurora_smpeg.mm
compiled=37 failed=0
