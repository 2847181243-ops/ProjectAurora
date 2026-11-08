/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * sdl12-compat includes SDL_opengl.h and SDL_opengl_glext.h unconditionally, but
 * SDL_opengl.h defines nothing on iOS ("No OpenGL on iOS"), so the glext header
 * runs into undefined GL types.
 *
 * ONScripter never uses OpenGL: it blits into SDL surfaces. Supplying the base GL
 * typedefs is enough to compile the shim; its GL paths stay dead code on iOS.
 */
#ifndef AURORA_SDL12_GL_STUB_H
#define AURORA_SDL12_GL_STUB_H

#include <stddef.h>

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef long long GLint64;
typedef unsigned long long GLuint64;
typedef char GLchar;

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif
#ifndef APIENTRY
#define APIENTRY GLAPIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#endif /* AURORA_SDL12_GL_STUB_H */
