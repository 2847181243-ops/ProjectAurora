/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * sdl12-compat includes SDL_opengl.h + SDL_opengl_glext.h unconditionally, but on
 * iOS SDL_opengl.h defines nothing ("No OpenGL on iOS").
 *
 * This header claims the SDL guards, pulls in the OpenGL ES 2 headers iOS ships,
 * and fills in the handful of desktop-GL names the shim still mentions. The shim's
 * GL paths stay dead code on iOS: ONScripter blits into SDL surfaces and never
 * asks for a GL context.
 */
#ifndef AURORA_SDL12_GL_STUB_H
#define AURORA_SDL12_GL_STUB_H

#define SDL_opengl_h_ 1
#define __glext_h_ 1
#define __gl_glext_h_ 1

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

/* Desktop-only types that the ES headers do not define. */
typedef double GLdouble;
typedef double GLclampd;

/* Desktop-only enums the shim references; values are the GL 1.1 ones. */
#ifndef GL_QUADS
#define GL_QUADS 0x0007
#endif
#ifndef GL_POLYGON
#define GL_POLYGON 0x0009
#endif
#ifndef GL_TEXTURE_1D
#define GL_TEXTURE_1D 0x0DE0
#endif
#ifndef GL_PROXY_TEXTURE_2D
#define GL_PROXY_TEXTURE_2D 0x8064
#endif
#ifndef GL_CLAMP
#define GL_CLAMP 0x2900
#endif
#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER 0x812D
#endif
#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#endif /* AURORA_SDL12_GL_STUB_H */
