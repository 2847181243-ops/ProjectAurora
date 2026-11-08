/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * sdl12-compat includes SDL_opengl.h + SDL_opengl_glext.h unconditionally, but on
 * iOS SDL_opengl.h defines nothing ("No OpenGL on iOS") and its glext companion
 * then runs into undefined types and enums.
 *
 * Instead of restating every GL symbol, this header claims the SDL guards and
 * includes the OpenGL ES 2 headers that iOS actually ships. The shim's GL paths
 * stay dead code: ONScripter blits into SDL surfaces and never asks for a GL
 * context.
 */
#ifndef AURORA_SDL12_GL_STUB_H
#define AURORA_SDL12_GL_STUB_H

#define SDL_opengl_h_ 1
#define __glext_h_ 1
#define __gl_glext_h_ 1

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#endif /* AURORA_SDL12_GL_STUB_H */
