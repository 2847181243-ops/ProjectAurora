# addon-sdl12-compat

sha: 64ee61155d4aae3498db4cba82a7f31f34966bf5
status: success
at: 2026-11-08T20:08:22Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7389:35: error: use of undeclared identifier 'GL_UNPACK_ROW_LENGTH'
addon.log- 7389 |         OpenGLFuncs.glPixelStorei(GL_UNPACK_ROW_LENGTH, VideoSurface12->pitch / VideoSurface12->format->BytesPerPixel);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~~
addon.log-/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:8711:12: warning: implicit conversion loses integer precision: 'unsigned long' to 'Uint32' (aka 'unsigned int') [-Wshorten-64-to-32]
--
addon.log:** BUILD FAILED **
addon.log-
addon.log-
addon.log-The following build commands failed:

## tails

### addon.log

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'sdl12_compat')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'sdl12_compat')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'SDL' from project 'sdl12_compat')
** BUILD FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c normal arm64 c com.apple.compilers.llvm.clang.1_0.compiler (in target 'SDL' from project 'sdl12_compat')
(1 failure)
GL_UNPACK_ROW_LENGTH
b/lib/Release-iphoneos/libSDLmain.a
