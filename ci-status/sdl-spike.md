# sdl-spike

sha: a5863085b015c0d251f245117195dae83fecefd4
status: failure
at: 2026-11-08T17:06:00Z

## errors

compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/sdl12-compat/src/SDL12_compat_objc.m:25:10: fatal error: 'Cocoa/Cocoa.h' file not found
compat.log-   25 | #include <Cocoa/Cocoa.h>
compat.log-      |          ^~~~~~~~~~~~~~~
compat.log-1 error generated.
--
compat.log:** BUILD FAILED **
compat.log-
compat.log-
compat.log-The following build commands failed:
--
image.log:CMake Error at CMakeLists.txt:397 (add_subdirectory):
image.log-  The source directory
image.log-
image.log-    /Users/runner/work/ProjectAurora/ProjectAurora/third_party/SDL_image/external/dav1d
--
image.log:CMake Error at cmake/PrivateSdlFunctions.cmake:263 (message):
image.log-  No cmake project for libavif found in external/libavif.
image.log-
image.log-  Run the download script in the external folder, or re-configure with
--
mixer.log:CMake Error at cmake/PrivateSdlFunctions.cmake:263 (message):
mixer.log-  No cmake project for ogg found in external/ogg.
mixer.log-
mixer.log-  Run the download script in the external folder, or re-configure with
--
ttf.log:CMake Error at CMakeLists.txt:277 (message):
ttf.log-  No freetype sources found.  Install a freetype development package or run
ttf.log-  the download script in the external folder.
ttf.log-

## tails

### compat.log
    
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c -ivfsstatcache /var/folders/h5/t85nd63d71x27_8gl5gw3l_w0000gn/C/com.apple.DeveloperTools/26.6-17F113/Xcode/SDKStatCaches.noindex/iphoneos26.5-23F81a-688ef53f1462e2c8f657fdc38a81448fee66e23052a0157c865c76b268046726.sdkstatcache -target arm64-apple-ios16.0 -fmessage-length\=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit\=0 -fno-color-diagnostics -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-return-type -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wno-unused-variable -Wunused-value -Wno-empty-body -Wno-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wno-constant-conversion -Wno-int-conversion -Wno-bool-conversion -Wno-enum-conversion -Wno-float-conversion -Wno-non-literal-null-conversion -Wno-objc-literal-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -Wno-implicit-fallthrough -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -fstrict-aliasing -Wdeprecated-declarations -Wno-sign-conversion -Wno-infinite-recursion -Wno-comma -Wno-block-capture-autoreleasing -Wno-strict-prototypes -Wno-semicolon-before-method-body -Wall -Wdeclaration-after-statement -Werror\=declaration-after-statement @/Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDL.build/Release-iphoneos/Objects-normal/arm64/7187679823f38a2a940e0043cdf9d637-common-args.resp -MMD -MT dependencies -MF /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat.d --serialize-diagnostics /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat.dia -c /Users/runner/work/ProjectAurora/ProjectAurora/third_party/sdl12-compat/src/SDL12_compat.c -o /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat.o

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'sdl12_compat')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'sdl12_compat')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'SDL' from project 'sdl12_compat')
** BUILD FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat_objc.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/sdl12-compat/src/SDL12_compat_objc.m normal arm64 objective-c com.apple.compilers.llvm.clang.1_0.compiler (in target 'SDL' from project 'sdl12_compat')
(1 failure)

### image.log

-- SDL2_image: Using vendored libavif
CMake Error at cmake/PrivateSdlFunctions.cmake:263 (message):
  No cmake project for libavif found in external/libavif.

  Run the download script in the external folder, or re-configure with
  -DSDL2IMAGE_VENDORED=OFF to use system packages.
Call Stack (most recent call first):
  CMakeLists.txt:411 (sdl_check_project_in_subfolder)


-- Configuring incomplete, errors occurred!

### mixer.log
-- Configuring SDL2_mixer 2.9.0
-- Using vendored libogg
CMake Error at cmake/PrivateSdlFunctions.cmake:263 (message):
  No cmake project for ogg found in external/ogg.

  Run the download script in the external folder, or re-configure with
  -DSDL2MIXER_VENDORED=OFF to use system packages.
Call Stack (most recent call first):
  CMakeLists.txt:367 (sdl_check_project_in_subfolder)


-- Configuring incomplete, errors occurred!

### sdl.log
-- Installing: /Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_config.h
-- Installing: /Users/runner/work/ProjectAurora/ProjectAurora/prefix/share/licenses/SDL2/LICENSE.txt
-- Installing: /Users/runner/work/ProjectAurora/ProjectAurora/prefix/lib/pkgconfig/sdl2.pc
-- Installing: /Users/runner/work/ProjectAurora/ProjectAurora/prefix/bin/sdl2-config
-- Installing: /Users/runner/work/ProjectAurora/ProjectAurora/prefix/share/aclocal/sdl2.m4

note: Run script build phase 'Generate CMakeFiles/sdl_headers_copy' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'sdl_headers_copy' from project 'SDL2')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'install' from project 'SDL2')
note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'SDL2')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2')
** BUILD SUCCEEDED **


### ttf.log
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring SDL2_ttf 2.25.0
-- Looking for __GLIBC__
-- Looking for __GLIBC__ - not found
-- SDL2_ttf: Using vendored freetype library
CMake Error at CMakeLists.txt:277 (message):
  No freetype sources found.  Install a freetype development package or run
  the download script in the external folder.


-- Configuring incomplete, errors occurred!
