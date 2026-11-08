# ons run 8

sha: a212acf4889d86855f5beacebf4cafde693d2062
status: failure
at: 2026-11-08T22:57:46Z

## errors

loslib.c:143:14: error: 'system' is unavailable: not available on iOS
xcodebuild: error: The project 'SDL/Xcode-iOS/SDL/SDL.xcodeproj' does not contain a target named 'SDL'.
FAILED: SDL
xcodebuild: error: 'SDL_image/Xcode-iOS/SDL_image/SDL_image.xcodeproj' does not exist.
FAILED: SDL_image
xcodebuild: error: 'SDL_ttf/Xcode-iOS/SDL_ttf/SDL_ttf.xcodeproj' does not exist.
FAILED: SDL_ttf
xcodebuild: error: 'SDL_mixer/Xcode-iOS/SDL_mixer/SDL_mixer.xcodeproj' does not exist.
FAILED: SDL_mixer
/Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/ONScripter/LUAHandler.h:27:10: fatal error: 'lua.hpp' file not found
/Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/ONScripter/LUAHandler.h:27:10: fatal error: 'lua.hpp' file not found
** BUILD FAILED **

## tails

### lua_smpeg.log
  143 |   int stat = system(cmd);
      |              ^
/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/include/_stdlib.h:208:6: note: 'system' has been explicitly marked unavailable here
  208 | int      system(const char *) __DARWIN_ALIAS_C(system);
      |          ^
1 error generated.
make[3]: *** [loslib.o] Error 1
make[2]: *** [macosx] Error 2
make[1]: *** [macosx] Error 2
make: *** [/Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/Release-iphoneos/lib/liblua.a] Error 2

### sdl_projects.log
Build settings from command line:
    ARCHS = arm64
    CODE_SIGNING_ALLOWED = NO
    CODE_SIGNING_REQUIRED = NO
    IPHONEOS_DEPLOYMENT_TARGET = 16.0
    SDKROOT = iphoneos26.5

2026-11-08 22:57:37.665 xcodebuild[4818:17446] Writing error result bundle to /var/folders/h5/t85nd63d71x27_8gl5gw3l_w0000gn/T/ResultBundle_2026-08-11_22-57-0037.xcresult
xcodebuild: error: 'SDL_mixer/Xcode-iOS/SDL_mixer/SDL_mixer.xcodeproj' does not exist.
FAILED: SDL_mixer

### ons_build.log
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -ivfsstatcache /var/folders/h5/t85nd63d71x27_8gl5gw3l_w0000gn/C/com.apple.DeveloperTools/26.6-17F113/Xcode/SDKStatCaches.noindex/iphoneos26.5-23F81a-688ef53f1462e2c8f657fdc38a81448fee66e23052a0157c865c76b268046726.sdkstatcache -target arm64-apple-ios16.0 -fmessage-length\=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit\=0 -fno-color-diagnostics -fmodules-prune-interval\=86400 -fmodules-prune-after\=345600 -fbuild-session-file\=/var/folders/h5/t85nd63d71x27_8gl5gw3l_w0000gn/C/org.llvm.clang/ModuleCache.noindex/Session.modulevalidation -fmodules-validate-once-per-build-session -Wnon-modular-include-in-framework-module -Werror\=non-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Werror\=return-type -Wdocumentation -Wunreachable-code -Werror\=deprecated-objc-isa-usage -Werror\=objc-root-class -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wno-non-literal-null-conversion -Wno-objc-literal-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wno-c++11-extensions -Wno-implicit-fallthrough -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -fstrict-aliasing -Wdeprecated-declarations -Winvalid-offsetof -Wno-sign-conversion -Winfinite-recursion -Wmove -Wno-comma -Wno-block-capture-autoreleasing -Wno-strict-prototypes -Wno-range-loop-analysis -Wno-semicolon-before-method-body @/Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/build/ONScripter.build/Release-iphoneos/ONScripter.build/Objects-normal/arm64/82b82416624d2658e5098eb0a28c15c5-common-args.resp -MMD -MT dependencies -MF /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/build/ONScripter.build/Release-iphoneos/ONScripter.build/Objects-normal/arm64/SarReader.d --serialize-diagnostics /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/build/ONScripter.build/Release-iphoneos/ONScripter.build/Objects-normal/arm64/SarReader.dia -c /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/ONScripter/SarReader.cpp -o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/build/ONScripter.build/Release-iphoneos/ONScripter.build/Objects-normal/arm64/SarReader.o

/Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/ONScripter.xcodeproj: warning: DEFINES_MODULE was set, but no umbrella header could be found to generate the module map (in target 'ONScripter' from project 'ONScripter')
** BUILD FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/build/ONScripter.build/Release-iphoneos/ONScripter.build/Objects-normal/arm64/ScriptParser_command.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/ONScripter/ScriptParser_command.cpp normal arm64 c++ com.apple.compilers.llvm.clang.1_0.compiler (in target 'ONScripter' from project 'ONScripter')
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/build/ONScripter.build/Release-iphoneos/ONScripter.build/Objects-normal/arm64/ScriptParser.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/port/ONScripter/ScriptParser.cpp normal arm64 c++ com.apple.compilers.llvm.clang.1_0.compiler (in target 'ONScripter' from project 'ONScripter')
(2 failures)
