# addon-sdl12-compat

sha: 6ebe50e9bc51184472ed8fea3ce11750e9afcc37
status: failure
at: 2026-11-08T19:18:28Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6161:35: error: use of undeclared identifier 'GL_READ_FRAMEBUFFER'
addon.log- 6161 |     OpenGLFuncs.glBindFramebuffer(GL_READ_FRAMEBUFFER, OpenGLLogicalScalingFBO);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6162:35: error: use of undeclared identifier 'GL_DRAW_FRAMEBUFFER'
addon.log- 6162 |     OpenGLFuncs.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OpenGLLogicalScalingMultisampleFBO);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6170:35: error: use of undeclared identifier 'GL_READ_FRAMEBUFFER'
addon.log- 6170 |     OpenGLFuncs.glBindFramebuffer(GL_READ_FRAMEBUFFER, OpenGLCurrentReadFBO);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6171:35: error: use of undeclared identifier 'GL_DRAW_FRAMEBUFFER'
addon.log- 6171 |     OpenGLFuncs.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OpenGLCurrentDrawFBO);
addon.log-      |                                   ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6187:20: error: use of undeclared identifier 'GL_READ_FRAMEBUFFER'
addon.log- 6187 |     if ((target == GL_READ_FRAMEBUFFER) || (target == GL_FRAMEBUFFER)) {
addon.log-      |                    ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6196:20: error: use of undeclared identifier 'GL_DRAW_FRAMEBUFFER'
addon.log- 6196 |     if ((target == GL_DRAW_FRAMEBUFFER) || (target == GL_FRAMEBUFFER)) {
addon.log-      |                    ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6205:39: error: use of undeclared identifier 'GL_READ_FRAMEBUFFER'
addon.log- 6205 |         OpenGLFuncs.glBindFramebuffer(GL_READ_FRAMEBUFFER, OpenGLCurrentReadFBO);
addon.log-      |                                       ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6206:39: error: use of undeclared identifier 'GL_DRAW_FRAMEBUFFER'
addon.log- 6206 |         OpenGLFuncs.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OpenGLCurrentDrawFBO);
addon.log-      |                                       ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6305:115: error: use of undeclared identifier 'GL_RGBA8'
addon.log- 6305 |     OpenGLFuncs.glRenderbufferStorageMultisample(GL_RENDERBUFFER, OpenGLLogicalScalingSamples, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                                                                   ^~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6305:126: error: use of undeclared identifier 'GL_RGB8'
addon.log- 6305 |     OpenGLFuncs.glRenderbufferStorageMultisample(GL_RENDERBUFFER, OpenGLLogicalScalingSamples, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                                                                              ^~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6310:100: error: use of undeclared identifier 'GL_DEPTH24_STENCIL8'
addon.log- 6310 |         OpenGLFuncs.glRenderbufferStorageMultisample(GL_RENDERBUFFER, OpenGLLogicalScalingSamples, GL_DEPTH24_STENCIL8, w, h);
addon.log-      |                                                                                                    ^~~~~~~~~~~~~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6343:79: error: use of undeclared identifier 'GL_RGBA8'
addon.log- 6343 |         OpenGLFuncs.glRenderbufferStorage(GL_RENDERBUFFER, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                               ^~~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6343:90: error: use of undeclared identifier 'GL_RGB8'
addon.log- 6343 |         OpenGLFuncs.glRenderbufferStorage(GL_RENDERBUFFER, (alpha_size > 0) ? GL_RGBA8 : GL_RGB8, w, h);
addon.log-      |                                                                                          ^~~~~~~
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL12_compat.c:6348:64: error: use of undeclared identifier 'GL_DEPTH24_STENCIL8'

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
