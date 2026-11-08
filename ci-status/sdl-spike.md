# sdl-spike

sha: bd6ad85f55db0c9d724907425e1cf790196d59b4
status: failure
at: 2026-11-08T16:56:49Z

## errors

compat.log:CMake Error at CMakeLists.txt:60 (message):
compat.log-  Could not find SDL2 headers
compat.log-
compat.log-
--
image.log:CMake Error at /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:290 (message):
image.log-  Could NOT find PrivateSDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR)
image.log-  (Required is at least version "2.0.9")
image.log-Call Stack (most recent call first):
--
mixer.log:CMake Error at /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:290 (message):
mixer.log-  Could NOT find PrivateSDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR)
mixer.log-  (Required is at least version "2.0.9")
mixer.log-Call Stack (most recent call first):
--
ttf.log:CMake Error at /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:290 (message):
ttf.log-  Could NOT find PrivateSDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR)
ttf.log-  (Required is at least version "2.0.10")
ttf.log-Call Stack (most recent call first):

## tails

### compat.log
-- Detecting C compile features - done
-- The OBJC compiler identification is AppleClang 21.0.0.21000101
-- Detecting OBJC compiler ABI info
-- Detecting OBJC compiler ABI info - done
-- Check for working OBJC compiler: /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang - skipped
-- Could NOT find SDL2 (missing: SDL2_DIR)
-- Could NOT find SDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR) 
CMake Error at CMakeLists.txt:60 (message):
  Could not find SDL2 headers


-- Configuring incomplete, errors occurred!

### image.log
-- Using private SDL2 find module
CMake Error at /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:290 (message):
  Could NOT find PrivateSDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR)
  (Required is at least version "2.0.9")
Call Stack (most recent call first):
  /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:654 (_FPHSA_FAILURE_MESSAGE)
  cmake/FindPrivateSDL2.cmake:33 (find_package_handle_standard_args)
  cmake/PrivateSdlFunctions.cmake:63 (find_package)
  CMakeLists.txt:193 (sdl_find_sdl2)


-- Configuring incomplete, errors occurred!

### mixer.log
-- Using private SDL2 find module
CMake Error at /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:290 (message):
  Could NOT find PrivateSDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR)
  (Required is at least version "2.0.9")
Call Stack (most recent call first):
  /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:654 (_FPHSA_FAILURE_MESSAGE)
  cmake/FindPrivateSDL2.cmake:33 (find_package_handle_standard_args)
  cmake/PrivateSdlFunctions.cmake:63 (find_package)
  CMakeLists.txt:213 (sdl_find_sdl2)


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
-- Using private SDL2 find module
CMake Error at /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:290 (message):
  Could NOT find PrivateSDL2 (missing: SDL2_LIBRARY SDL2_INCLUDE_DIR)
  (Required is at least version "2.0.10")
Call Stack (most recent call first):
  /opt/homebrew/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:654 (_FPHSA_FAILURE_MESSAGE)
  cmake/FindPrivateSDL2.cmake:33 (find_package_handle_standard_args)
  cmake/PrivateSdlFunctions.cmake:63 (find_package)
  CMakeLists.txt:85 (sdl_find_sdl2)


-- Configuring incomplete, errors occurred!
