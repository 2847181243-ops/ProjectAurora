# addon-sdl12-compat

sha: 45dc3d1834953c350ca26a23150dad97f78ede4b
status: failure
at: 2026-11-08T19:42:16Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7371:34: error: use of undeclared identifier 'GL_ALL_ATTRIB_BITS'
addon.log- 7371 |         OpenGLFuncs.glPushAttrib(GL_ALL_ATTRIB_BITS);
addon.log-      |                                  ^~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7372:40: error: use of undeclared identifier 'GL_CLIENT_PIXEL_STORE_BIT'
addon.log- 7372 |         OpenGLFuncs.glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
addon.log-      |                                        ^~~~~~~~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7375:31: error: use of undeclared identifier 'GL_FOG'
addon.log- 7375 |         OpenGLFuncs.glDisable(GL_FOG);
addon.log-      |                               ^~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7376:31: error: use of undeclared identifier 'GL_ALPHA_TEST'
addon.log- 7376 |         OpenGLFuncs.glDisable(GL_ALPHA_TEST);
addon.log-      |                               ^~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7383:31: error: use of undeclared identifier 'GL_TEXTURE_ENV'
addon.log- 7383 |         OpenGLFuncs.glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
addon.log-      |                               ^~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7383:47: error: use of undeclared identifier 'GL_TEXTURE_ENV_MODE'
addon.log- 7383 |         OpenGLFuncs.glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
addon.log-      |                                               ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7383:68: error: use of undeclared identifier 'GL_MODULATE'
addon.log- 7383 |         OpenGLFuncs.glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
addon.log-      |                                                                    ^~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7389:35: error: use of undeclared identifier 'GL_UNPACK_ROW_LENGTH'
addon.log- 7389 |         OpenGLFuncs.glPixelStorei(GL_UNPACK_ROW_LENGTH, VideoSurface12->pitch / VideoSurface12->format->BytesPerPixel);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7394:34: error: use of undeclared identifier 'GL_PROJECTION'
addon.log- 7394 |         OpenGLFuncs.glMatrixMode(GL_PROJECTION);
addon.log-      |                                  ^~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7400:34: error: use of undeclared identifier 'GL_MODELVIEW'
addon.log- 7400 |         OpenGLFuncs.glMatrixMode(GL_MODELVIEW);
addon.log-      |                                  ^~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:7463:42: error: use of undeclared identifier 'GL_PROJECTION'
addon.log- 7463 |                 OpenGLFuncs.glMatrixMode(GL_PROJECTION);
addon.log-      |                                          ^~~~~~~~~~~~~
addon.log-/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:8711:12: warning: implicit conversion loses integer precision: 'unsigned long' to 'Uint32' (aka 'unsigned int') [-Wshorten-64-to-32]
--
addon.log:** BUILD FAILED **
addon.log-
addon.log-
addon.log-The following build commands failed:

## tails

### addon.log
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/libtool -static -arch_only arm64 -D -syslibroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -L/Users/runner/work/ProjectAurora/ProjectAurora/b/lib/Release-iphoneos -filelist /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/build/SDLmain.build/Release-iphoneos/Objects-normal/arm64/SDLmain.LinkFileList -dependency_info /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/build/SDLmain.build/Release-iphoneos/Objects-normal/arm64/SDLmain_libtool_dependency_info.dat -o /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/Release-iphoneos/libSDLmain.a
libtool: warning: 'SDL_dummy_main.o' has no symbols

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'sdl12_compat')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'sdl12_compat')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'SDL' from project 'sdl12_compat')
** BUILD FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c normal arm64 c com.apple.compilers.llvm.clang.1_0.compiler (in target 'SDL' from project 'sdl12_compat')
(1 failure)
