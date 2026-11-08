# sdl-spike

<<<<<<< Updated upstream
sha: 2a35992ae8ff4f401b7b2a4f117344f625903c25
status: running
at: 2026-11-08T17:27:47Z
=======
sha: 78452883bbc939e8a5b6e5fa78cc9c279bc7005c
status: failure
at: 2026-11-08T17:28:54Z

## errors

compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:407:54: error: unknown type name 'GLenum'; did you mean 'enum'?
compat.log-  407 | typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
compat.log-      |                                                      ^~~~~~
compat.log-      |                                                      enum
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:407:93: error: unknown type name 'GLsizei'
compat.log-  407 | typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
compat.log-      |                                                                                             ^
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:407:108: error: unknown type name 'GLenum'; did you mean 'enum'?
compat.log-  407 | typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
compat.log-      |                                                                                                            ^~~~~~
compat.log-      |                                                                                                            enum
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:47: error: unknown type name 'GLenum'; did you mean 'enum'?
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                               ^~~~~~
compat.log-      |                                               enum
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:62: error: unknown type name 'GLint'; did you mean 'GLuint'?
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                              ^~~~~
compat.log-      |                                                              GLuint
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:75: error: unknown type name 'GLint'; did you mean 'GLuint'?
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                           ^~~~~
compat.log-      |                                                                           GLuint
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:97: error: unknown type name 'GLsizei'
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                                                 ^
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:112: error: unknown type name 'GLsizei'
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                                                                ^
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:128: error: unknown type name 'GLsizei'
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                                                                                ^
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:143: error: unknown type name 'GLint'; did you mean 'GLuint'?
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                                                                                               ^~~~~
compat.log-      |                                                                                                                                               GLuint
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:157: error: unknown type name 'GLenum'; did you mean 'enum'?
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                                                                                                             ^~~~~~
compat.log-      |                                                                                                                                                             enum
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:408:172: error: unknown type name 'GLenum'; did you mean 'enum'?
compat.log-  408 | typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                                                                                                                            ^~~~~~
compat.log-      |                                                                                                                                                                            enum
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:409:50: error: unknown type name 'GLenum'; did you mean 'enum'?
compat.log-  409 | typedef void (APIENTRYP PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                  ^~~~~~
compat.log-      |                                                  enum
--
compat.log:/Users/runner/work/ProjectAurora/ProjectAurora/prefix/include/SDL2/SDL_opengl_glext.h:409:65: error: unknown type name 'GLint'; did you mean 'GLuint'?
compat.log-  409 | typedef void (APIENTRYP PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
compat.log-      |                                                                 ^~~~~

## tails

### compat.log
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/libtool -static -arch_only arm64 -D -syslibroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -L/Users/runner/work/ProjectAurora/ProjectAurora/b/compat/Release-iphoneos -filelist /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDLmain.build/Release-iphoneos/Objects-normal/arm64/SDLmain.LinkFileList -dependency_info /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDLmain.build/Release-iphoneos/Objects-normal/arm64/SDLmain_libtool_dependency_info.dat -o /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/Release-iphoneos/libSDLmain.a
libtool: warning: 'SDL_dummy_main.o' has no symbols

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'sdl12_compat')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'sdl12_compat')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'SDL' from project 'sdl12_compat')
** BUILD FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/b/compat/build/SDL.build/Release-iphoneos/Objects-normal/arm64/SDL12_compat.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/sdl12-compat/src/SDL12_compat.c normal arm64 c com.apple.compilers.llvm.clang.1_0.compiler (in target 'SDL' from project 'sdl12_compat')
(1 failure)

### image.log
note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'SDL2_image')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'webpdemux' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'webp' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'tiff' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'avif' from project 'SDL2_image')
** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/b/image/external/libwebp/Release-iphoneos/libwebpdemux.1.0.3.dylib normal (in target 'webpdemux' from project 'SDL2_image')
(1 failure)

### mixer.log
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'xmp_shared' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'wavpack' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'opusfile' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'opus' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ogg' from project 'SDL2_mixer')
** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/b/mixer/external/wavpack/Release-iphoneos/libwavpack.1.2.5.dylib normal (in target 'wavpack' from project 'SDL2_mixer')
(1 failure)

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
    cd /Users/runner/work/ProjectAurora/ProjectAurora/third_party/SDL_ttf
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/libtool -static -arch_only arm64 -D -syslibroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -L/Users/runner/work/ProjectAurora/ProjectAurora/b/ttf/Release-iphoneos -filelist /Users/runner/work/ProjectAurora/ProjectAurora/b/ttf/build/SDL2_ttf.build/Release-iphoneos/Objects-normal/arm64/SDL2_ttf.LinkFileList -dependency_info /Users/runner/work/ProjectAurora/ProjectAurora/b/ttf/build/SDL2_ttf.build/Release-iphoneos/Objects-normal/arm64/SDL2_ttf_libtool_dependency_info.dat -o /Users/runner/work/ProjectAurora/ProjectAurora/b/ttf/Release-iphoneos/libSDL2_ttf.a

PhaseScriptExecution Generate\ CMakeFiles/ALL_BUILD /Users/runner/work/ProjectAurora/ProjectAurora/b/ttf/build/SDL2_ttf.build/Release-iphoneos/ALL_BUILD.build/Script-39AA40A43ED84945F6CEE86A.sh (in target 'ALL_BUILD' from project 'SDL2_ttf')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/third_party/SDL_ttf
    /bin/sh -c /Users/runner/work/ProjectAurora/ProjectAurora/b/ttf/build/SDL2_ttf.build/Release-iphoneos/ALL_BUILD.build/Script-39AA40A43ED84945F6CEE86A.sh
Build all projects

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'SDL2_ttf')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2_ttf')
** BUILD SUCCEEDED **

>>>>>>> Stashed changes
