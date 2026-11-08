# ons run 3

sha: 0cef6799a46c4dc140d6a472f16e1349dd45888f
status: failure
at: 2026-11-08T21:29:09Z

## errors

third_party/iosons/smpeg/audio/hufftable.cpp:553:8: error: constant expression evaluates to -1 which cannot be narrowed to type 'unsigned int' [-Wc++11-narrowing]
third_party/iosons/smpeg/audio/hufftable.cpp:553:13: error: constant expression evaluates to -1 which cannot be narrowed to type 'unsigned int' [-Wc++11-narrowing]
third_party/iosons/smpeg/audio/hufftable.cpp:557:8: error: constant expression evaluates to -1 which cannot be narrowed to type 'unsigned int' [-Wc++11-narrowing]
third_party/iosons/smpeg/audio/hufftable.cpp:557:13: error: constant expression evaluates to -1 which cannot be narrowed to type 'unsigned int' [-Wc++11-narrowing]
third_party/iosons/smpeg/audio/hufftable.cpp:567:8: error: constant expression evaluates to -1 which cannot be narrowed to type 'unsigned int' [-Wc++11-narrowing]
third_party/iosons/smpeg/audio/hufftable.cpp:567:13: error: constant expression evaluates to -1 which cannot be narrowed to type 'unsigned int' [-Wc++11-narrowing]
FAILED: smpeg/audio/hufftable.cpp
third_party/onscripter/ONScripterLabel.h:434:5: error: unknown type name 'SDL_keysym'; did you mean 'SDL_Keysym'?
third_party/onscripter/ONScripterLabel.h:434:25: error: unknown type name 'SDL_keysym'; did you mean 'SDL_Keysym'?
third_party/onscripter/ONScripterLabel.h:576:5: error: unknown type name 'SDLKey'
third_party/onscripter/ONScripterLabel.h:993:5: error: unknown type name 'SDL_CD'
third_party/onscripter/LUAHandler.cpp:31:14: error: unknown type name 'lua_State'
third_party/onscripter/LUAHandler.cpp:34:5: error: unknown type name 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:34:23: error: use of undeclared identifier 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:34:34: error: expected expression
third_party/onscripter/LUAHandler.cpp:47:17: error: unknown type name 'lua_State'
third_party/onscripter/LUAHandler.cpp:50:5: error: unknown type name 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:50:23: error: use of undeclared identifier 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:50:34: error: expected expression
third_party/onscripter/LUAHandler.cpp:69:14: error: unknown type name 'lua_State'
third_party/onscripter/LUAHandler.cpp:72:5: error: unknown type name 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:72:23: error: use of undeclared identifier 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:72:34: error: expected expression
third_party/onscripter/LUAHandler.cpp:85:17: error: unknown type name 'lua_State'
third_party/onscripter/LUAHandler.cpp:88:5: error: unknown type name 'LUAHandler'
third_party/onscripter/LUAHandler.cpp:88:23: error: use of undeclared identifier 'LUAHandler'
fatal error: too many errors emitted, stopping now [-ferror-limit=]
FAILED: LUAHandler.cpp
third_party/onscripter/MadWrapper.cpp:25:10: fatal error: 'mad.h' file not found
FAILED: MadWrapper.cpp
third_party/onscripter/ONScripterLabel.h:434:5: error: unknown type name 'SDL_keysym'; did you mean 'SDL_Keysym'?
third_party/onscripter/ONScripterLabel.h:434:25: error: unknown type name 'SDL_keysym'; did you mean 'SDL_Keysym'?
third_party/onscripter/ONScripterLabel.h:576:5: error: unknown type name 'SDLKey'
third_party/onscripter/ONScripterLabel.h:993:5: error: unknown type name 'SDL_CD'
third_party/onscripter/onscripter.cpp:205:17: error: cannot initialize object parameter of type 'ScriptParser' with an expression of type 'ONScripterLabel'
third_party/onscripter/onscripter.cpp:210:17: error: cannot initialize object parameter of type 'ScriptParser' with an expression of type 'ONScripterLabel'
third_party/onscripter/onscripter.cpp:249:17: error: cannot initialize object parameter of type 'ScriptParser' with an expression of type 'ONScripterLabel'
third_party/onscripter/onscripter.cpp:263:17: error: cannot initialize object parameter of type 'ScriptParser' with an expression of type 'ONScripterLabel'
third_party/onscripter/onscripter.cpp:323:17: error: cannot initialize object parameter of type 'ScriptParser' with an expression of type 'ONScripterLabel'
third_party/onscripter/onscripter.cpp:328:13: error: cannot initialize object parameter of type 'ScriptParser' with an expression of type 'ONScripterLabel'

## compile summary
6 errors generated.
FAILED: ons_engine_onscripter.cpp
compiled=20 failed=20
