# addon-sdl12-compat

sha: 50bad248810c25566db6137c560f50b2935b72ad
status: failure
at: 2026-11-08T19:11:25Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL20_syms.h:385:31: error: unknown type name 'GLdouble'; did you mean 'double'?
addon.log-  385 | OPENGL_SYM(Core,void,glOrtho,(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f),(a,b,c,d,e,f),)
addon.log-      |                               ^~~~~~~~
addon.log-      |                               double
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL20_syms.h:385:43: error: unknown type name 'GLdouble'; did you mean 'double'?
addon.log-  385 | OPENGL_SYM(Core,void,glOrtho,(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f),(a,b,c,d,e,f),)
addon.log-      |                                           ^~~~~~~~
addon.log-      |                                           double
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL20_syms.h:385:55: error: unknown type name 'GLdouble'; did you mean 'double'?
addon.log-  385 | OPENGL_SYM(Core,void,glOrtho,(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f),(a,b,c,d,e,f),)
addon.log-      |                                                       ^~~~~~~~
addon.log-      |                                                       double
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL20_syms.h:385:67: error: unknown type name 'GLdouble'; did you mean 'double'?
addon.log-  385 | OPENGL_SYM(Core,void,glOrtho,(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f),(a,b,c,d,e,f),)
addon.log-      |                                                                   ^~~~~~~~
addon.log-      |                                                                   double
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL20_syms.h:385:79: error: unknown type name 'GLdouble'; did you mean 'double'?
addon.log-  385 | OPENGL_SYM(Core,void,glOrtho,(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f),(a,b,c,d,e,f),)
addon.log-      |                                                                               ^~~~~~~~
addon.log-      |                                                                               double
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/src/SDL20_syms.h:385:91: error: unknown type name 'GLdouble'; did you mean 'double'?
addon.log-  385 | OPENGL_SYM(Core,void,glOrtho,(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f),(a,b,c,d,e,f),)
addon.log-      |                                                                                           ^~~~~~~~
addon.log-      |                                                                                           double
--
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
