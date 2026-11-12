// SPDX-License-Identifier: GPL-3.0-or-later
/* Live2D stubs.
 *
 * The Live2D Cubism SDK is proprietary and is not shipped with this build, but
 * parts of the engine still carry code paths that reference these two symbols
 * when KRKR2_LIVE2D is defined. Providing inert definitions keeps the engine
 * linkable; without the SDK there is never a Live2D model to render.
 */
#if defined(AURORA_WITH_KRKR)

/* Layout must match the declaration in krkrgles.cpp: GLuint, GLsizei, GLsizei. */
struct Live2DRenderTarget {
    unsigned int fbo;
    int width;
    int height;
};

Live2DRenderTarget g_live2dRenderTarget = {0, 0, 0};

extern "C" void TVPRegisterKrkrLive2DPluginAnchor() {}

#endif /* AURORA_WITH_KRKR */
