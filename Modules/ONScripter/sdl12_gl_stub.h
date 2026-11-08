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

#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 0x8D40
#endif
#ifndef GL_READ_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER 0x8CA8
#endif
#ifndef GL_DRAW_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#endif
#ifndef GL_READ_FRAMEBUFFER_BINDING
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#endif
#ifndef GL_DRAW_FRAMEBUFFER_BINDING
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#endif
#ifndef GL_RENDERBUFFER
#define GL_RENDERBUFFER 0x8D41
#endif
#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0 0x8CE0
#endif

#ifndef GL_RGB8
#define GL_RGB8 0x8051
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0x8058
#endif
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_DEPTH24_STENCIL8
#define GL_DEPTH24_STENCIL8 0x88F0
#endif
#ifndef GL_DEPTH_STENCIL
#define GL_DEPTH_STENCIL 0x84F9
#endif
#ifndef GL_DEPTH_STENCIL_ATTACHMENT
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif

/* GL 1.1 state bits and enums the shim's attribute stack mentions. */
#ifndef GL_CURRENT_BIT
#define GL_CURRENT_BIT 0x00000001
#endif
#ifndef GL_POINT_BIT
#define GL_POINT_BIT 0x00000002
#endif
#ifndef GL_LINE_BIT
#define GL_LINE_BIT 0x00000004
#endif
#ifndef GL_POLYGON_BIT
#define GL_POLYGON_BIT 0x00000008
#endif
#ifndef GL_POLYGON_STIPPLE_BIT
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#endif
#ifndef GL_PIXEL_MODE_BIT
#define GL_PIXEL_MODE_BIT 0x00000020
#endif
#ifndef GL_LIGHTING_BIT
#define GL_LIGHTING_BIT 0x00000040
#endif
#ifndef GL_FOG_BIT
#define GL_FOG_BIT 0x00000080
#endif
#ifndef GL_ACCUM_BUFFER_BIT
#define GL_ACCUM_BUFFER_BIT 0x00000200
#endif
#ifndef GL_VIEWPORT_BIT
#define GL_VIEWPORT_BIT 0x00000800
#endif
#ifndef GL_TRANSFORM_BIT
#define GL_TRANSFORM_BIT 0x00001000
#endif
#ifndef GL_ENABLE_BIT
#define GL_ENABLE_BIT 0x00002000
#endif
#ifndef GL_HINT_BIT
#define GL_HINT_BIT 0x00008000
#endif
#ifndef GL_EVAL_BIT
#define GL_EVAL_BIT 0x00010000
#endif
#ifndef GL_LIST_BIT
#define GL_LIST_BIT 0x00020000
#endif
#ifndef GL_TEXTURE_BIT
#define GL_TEXTURE_BIT 0x00040000
#endif
#ifndef GL_SCISSOR_BIT
#define GL_SCISSOR_BIT 0x00080000
#endif
#ifndef GL_ALL_ATTRIB_BITS
#define GL_ALL_ATTRIB_BITS 0xFFFFFFFF
#endif
#ifndef GL_CLIENT_PIXEL_STORE_BIT
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#endif
#ifndef GL_CLIENT_VERTEX_ARRAY_BIT
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#endif
#ifndef GL_CLIENT_ALL_ATTRIB_BITS
#define GL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF
#endif
#ifndef GL_FOG
#define GL_FOG 0x0B60
#endif
#ifndef GL_LIGHTING
#define GL_LIGHTING 0x0B50
#endif
#ifndef GL_ALPHA_TEST
#define GL_ALPHA_TEST 0x0BC0
#endif
#ifndef GL_LINE_SMOOTH
#define GL_LINE_SMOOTH 0x0B20
#endif
#ifndef GL_POLYGON_SMOOTH
#define GL_POLYGON_SMOOTH 0x0B41
#endif
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#endif /* AURORA_SDL12_GL_STUB_H */
