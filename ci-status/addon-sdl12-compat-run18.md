# addon-sdl12-compat

sha: eacc00ac44fc298edd69be8af87f0a380b6aa6c4
status: failure
at: 2026-11-08T18:49:02Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:407:54: error: unknown type name 'GLenum'; did you mean 'enum'?
addon.log-  407 | typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
addon.log-      |                                                      ^~~~~~
addon.log-      |                                                      enum
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:407:93: error: unknown type name 'GLsizei'
addon.log-  407 | typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
addon.log-      |                                                                                             ^
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:407:108: error: unknown type name 'GLenum'; did you mean 'enum'?
addon.log-  407 | typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
addon.log-      |                                                                                                            ^~~~~~
addon.log-      |                                                                                                            enum
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:47: error: unknown type name 'GLenum'; did you mean 'enum'?
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                               ^~~~~~
addon.log-      |                                               enum
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:62: error: unknown type name 'GLint'; did you mean 'GLuint'?
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                                              ^~~~~
addon.log-      |                                                              GLuint
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:75: error: unknown type name 'GLint'; did you mean 'GLuint'?
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                                                           ^~~~~
addon.log-      |                                                                           GLuint
--
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:97: error: unknown type name 'GLsizei'
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                                                                                 ^
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:112: error: unknown type name 'GLsizei'
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                                                                                                ^
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:128: error: unknown type name 'GLsizei'
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                                                                                                                ^
addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/stack/prefix/include/SDL2/SDL_opengl_glext.h:408:143: error: unknown type name 'GLint'; did you mean 'GLuint'?
addon.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
addon.log-      |                                                                                                                                               ^~~~~

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
