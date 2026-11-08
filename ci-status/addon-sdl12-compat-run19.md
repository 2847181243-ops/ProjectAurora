# addon-sdl12-compat

sha: 2d30de03e134445c7fa0a0e7a6cbd7ce259e7ff2
status: failure
at: 2026-11-08T18:59:42Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6128:54: error: use of undeclared identifier 'GL_VERSION'
addon.log- 6128 |     version = (const char *) OpenGLFuncs.glGetString(GL_VERSION);
addon.log-      |                                                      ^~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6155:59: error: use of undeclared identifier 'GL_SCISSOR_TEST'
addon.log- 6155 |     const GLboolean has_scissor = OpenGLFuncs.glIsEnabled(GL_SCISSOR_TEST);
addon.log-      |                                                           ^~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6158:31: error: use of undeclared identifier 'GL_SCISSOR_TEST'
addon.log- 6158 |         OpenGLFuncs.glDisable(GL_SCISSOR_TEST);  /* scissor test affects framebuffer_blit */
addon.log-      |                               ^~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6165:35: error: use of undeclared identifier 'GL_COLOR_BUFFER_BIT'
addon.log- 6165 |                                   GL_COLOR_BUFFER_BIT, GL_NEAREST);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6165:56: error: use of undeclared identifier 'GL_NEAREST'
addon.log- 6165 |                                   GL_COLOR_BUFFER_BIT, GL_NEAREST);
addon.log-      |                                                        ^~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6168:30: error: use of undeclared identifier 'GL_SCISSOR_TEST'
addon.log- 6168 |         OpenGLFuncs.glEnable(GL_SCISSOR_TEST);
addon.log-      |                              ^~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6284:25: error: use of undeclared identifier 'GL_COLOR_BUFFER_BIT'
addon.log- 6284 |     OpenGLFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
addon.log-      |                         ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6284:47: error: use of undeclared identifier 'GL_DEPTH_BUFFER_BIT'
addon.log- 6284 |     OpenGLFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
addon.log-      |                                               ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6305:115: error: use of undeclared identifier 'GL_RGBA8'
addon.log- 6305 |     OpenGLFuncs.glRenderbufferStorageMultisample(GL_RENDERBUFFER, OpenGLLogicalScalingSamples, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                                                                   ^~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6305:126: error: use of undeclared identifier 'GL_RGB8'
addon.log- 6305 |     OpenGLFuncs.glRenderbufferStorageMultisample(GL_RENDERBUFFER, OpenGLLogicalScalingSamples, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                                                                              ^~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6343:79: error: use of undeclared identifier 'GL_RGBA8'
addon.log- 6343 |         OpenGLFuncs.glRenderbufferStorage(GL_RENDERBUFFER, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                               ^~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6343:90: error: use of undeclared identifier 'GL_RGB8'
addon.log- 6343 |         OpenGLFuncs.glRenderbufferStorage(GL_RENDERBUFFER, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                                          ^~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6379:25: error: use of undeclared identifier 'GL_COLOR_BUFFER_BIT'
addon.log- 6379 |     OpenGLFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
addon.log-      |                         ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6379:47: error: use of undeclared identifier 'GL_DEPTH_BUFFER_BIT'

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
