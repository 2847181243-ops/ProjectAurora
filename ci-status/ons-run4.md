# ons run 4

sha: 4cfd43fde39baee5a56e9c18466a6b60cfc8f20a
status: failure
at: 2026-11-08T21:45:25Z

## errors

third_party/iosons/smpeg/video/MPEGvideo.cpp:272:19: error: no matching function for call to 'SDL_CreateThread'
FAILED: smpeg/video/MPEGvideo.cpp
prefix/include/SDL2/SDL_stdinc.h:192:14: error: redefinition of 'SDL_bool'
prefix/include/SDL2/SDL_stdinc.h:441:5: error: redefinition of enumerator 'DUMMY_ENUM_VALUE'
prefix/include/SDL2/SDL_stdinc.h:442:3: error: typedef redefinition with different types ('enum SDL_DUMMY_ENUM' vs 'enum SDL_DUMMY_ENUM')
prefix/include/SDL2/SDL_stdinc.h:547:29: error: declaration of 'isdigit' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:550:29: error: declaration of 'isspace' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:555:29: error: declaration of 'toupper' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:556:29: error: declaration of 'tolower' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:574:23: error: expected unqualified-id
prefix/include/SDL2/SDL_stdinc.h:574:23: error: while loop outside of a function
prefix/include/SDL2/SDL_stdinc.h:635:31: error: redefinition of 'SDL_ltoa' as different kind of symbol
prefix/include/SDL2/SDL_stdinc.h:635:44: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:635:56: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:635:57: error: use of undeclared identifier 'str'
prefix/include/SDL2/SDL_stdinc.h:635:66: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:636:31: error: redefinition of 'SDL_ultoa' as different kind of symbol
prefix/include/SDL2/SDL_stdinc.h:636:50: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:636:66: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:636:67: error: use of undeclared identifier 'str'
prefix/include/SDL2/SDL_stdinc.h:636:76: error: expected '(' for function-style cast or type construction
fatal error: too many errors emitted, stopping now [-ferror-limit=]
FAILED: AnimationInfo.cpp
prefix/include/SDL2/SDL_stdinc.h:192:14: error: redefinition of 'SDL_bool'
prefix/include/SDL2/SDL_stdinc.h:441:5: error: redefinition of enumerator 'DUMMY_ENUM_VALUE'
prefix/include/SDL2/SDL_stdinc.h:442:3: error: typedef redefinition with different types ('enum SDL_DUMMY_ENUM' vs 'enum SDL_DUMMY_ENUM')
prefix/include/SDL2/SDL_stdinc.h:547:29: error: declaration of 'isdigit' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:550:29: error: declaration of 'isspace' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:555:29: error: declaration of 'toupper' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:556:29: error: declaration of 'tolower' has a different language linkage
prefix/include/SDL2/SDL_stdinc.h:574:23: error: expected unqualified-id
prefix/include/SDL2/SDL_stdinc.h:574:23: error: while loop outside of a function
prefix/include/SDL2/SDL_stdinc.h:635:31: error: redefinition of 'SDL_ltoa' as different kind of symbol
prefix/include/SDL2/SDL_stdinc.h:635:44: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:635:56: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:635:57: error: use of undeclared identifier 'str'
prefix/include/SDL2/SDL_stdinc.h:635:66: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:636:31: error: redefinition of 'SDL_ultoa' as different kind of symbol
prefix/include/SDL2/SDL_stdinc.h:636:50: error: expected '(' for function-style cast or type construction
prefix/include/SDL2/SDL_stdinc.h:636:66: error: expected '(' for function-style cast or type construction

## compile summary
20 errors generated.
FAILED: ons_engine_onscripter.cpp
compiled=15 failed=24
