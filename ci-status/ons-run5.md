# ons run 5

sha: f3fe008abcaba02f411c90eb554dc7b6752f0980
status: failure
at: 2026-11-08T22:10:47Z

## errors

third_party/iosons/smpeg/video/MPEGvideo.cpp:272:19: error: no matching function for call to 'SDL_CreateThread'
FAILED: smpeg/video/MPEGvideo.cpp
third_party/onscripter/MadWrapper.cpp:25:10: fatal error: 'mad.h' file not found
FAILED: MadWrapper.cpp
third_party/onscripter/ONScripterLabel_command.cpp:2111:13: error: no matching function for call to 'SMPEG_setdisplay'
third_party/onscripter/ONScripterLabel_command.cpp:2153:13: error: no matching function for call to 'SMPEG_setdisplay'
FAILED: ONScripterLabel_command.cpp
third_party/onscripter/ONScripterLabel_sound.cpp:365:22: error: no matching function for call to 'SMPEG_new_rwops'
third_party/onscripter/ONScripterLabel_sound.cpp:694:26: error: no matching function for call to 'SMPEG_new_rwops'
third_party/onscripter/ONScripterLabel_sound.cpp:738:9: error: no matching function for call to 'SMPEG_setdisplay'
third_party/onscripter/ONScripterLabel_sound.cpp:740:13: error: use of undeclared identifier 'SMPEG_scaleXY'
third_party/onscripter/ONScripterLabel_sound.cpp:741:13: error: use of undeclared identifier 'SMPEG_move'
third_party/onscripter/ONScripterLabel_sound.cpp:747:13: error: use of undeclared identifier 'SMPEG_scaleXY'
third_party/onscripter/ONScripterLabel_sound.cpp:748:13: error: use of undeclared identifier 'SMPEG_move'
third_party/onscripter/ONScripterLabel_sound.cpp:838:29: error: no matching function for call to 'SMPEG_setdisplay'
FAILED: ONScripterLabel_sound.cpp
third_party/onscripter/X11_messagebox.cpp:34:10: fatal error: 'X11/Xlib.h' file not found
FAILED: X11_messagebox.cpp
Modules/ONScripter/src/ons_engine_onscripter.cpp:28:5: error: use of undeclared identifier 'SDL_zero'
FAILED: ons_engine_onscripter.cpp

## compile summary
1 error generated.
FAILED: ons_engine_onscripter.cpp
compiled=34 failed=5
