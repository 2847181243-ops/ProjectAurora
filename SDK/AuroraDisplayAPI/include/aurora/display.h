// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_DISPLAY_H
#define AURORA_DISPLAY_H

#include <cstdint>

namespace aurora {

enum class ScaleMode {
    Fit,      /* whole frame visible, letterboxed */
    Fill,     /* fills the view, crops the overflow */
    Integer,  /* integer scale factor, no shimmering pixels */
    Stretch   /* distort to fill; only useful for debugging */
};

const char* scaleModeName(ScaleMode mode);
bool scaleModeFromName(const char* name, ScaleMode& out);

struct DisplaySettings {
    ScaleMode mode = ScaleMode::Fit;
    bool linear_filter = false;   /* nearest by default: pixel art */
    int rotation_degrees = 0;     /* 0/90/180/270 */
};

struct Viewport {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

/* Computes where the frame lands inside a drawable. Pure function: the same
 * inputs always give the same rectangle, which is what makes it testable and
 * keeps the Metal view dumb. */
Viewport computeViewport(int frame_width, int frame_height,
                         int view_width, int view_height,
                         const DisplaySettings& settings);

} // namespace aurora

#endif
