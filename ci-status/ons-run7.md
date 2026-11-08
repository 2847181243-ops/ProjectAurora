# ons run 7

sha: 63312987205361d21b8c3baaef1322043c49acd0
status: failure
at: 2026-11-08T22:48:46Z

## errors

third_party/smpeg/video/MPEGvideo.cpp:272:74: error: too many arguments provided to function-like macro invocation
third_party/smpeg/video/MPEGvideo.cpp:272:19: error: assigning to 'SDL_Thread *' from incompatible type 'SDL_Thread *(int (*)(void *), void *)'
FAILED: smpeg/video/MPEGvideo.cpp
third_party/smpeg/audio/mpegtoraw.cpp:476:13: error: use of undeclared identifier 'SDL_MixAudioFormat'
third_party/smpeg/audio/mpegtoraw.cpp:482:9: error: use of undeclared identifier 'SDL_MixAudioFormat'
third_party/smpeg/audio/mpegtoraw.cpp:502:9: error: use of undeclared identifier 'SDL_MixAudioFormat'
FAILED: smpeg/audio/mpegtoraw.cpp
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

## compile summary
8 errors generated.
FAILED: ONScripterLabel_sound.cpp
compiled=35 failed=2
