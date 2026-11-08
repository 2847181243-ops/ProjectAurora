// SPDX-License-Identifier: GPL-3.0-or-later
#include "aurora/display.h"

#include <algorithm>
#include <cstring>

namespace aurora {

const char* scaleModeName(ScaleMode mode) {
    switch (mode) {
        case ScaleMode::Fit: return "fit";
        case ScaleMode::Fill: return "fill";
        case ScaleMode::Integer: return "integer";
        case ScaleMode::Stretch: return "stretch";
    }
    return "fit";
}

bool scaleModeFromName(const char* name, ScaleMode& out) {
    if (name == nullptr) return false;
    if (std::strcmp(name, "fit") == 0) { out = ScaleMode::Fit; return true; }
    if (std::strcmp(name, "fill") == 0) { out = ScaleMode::Fill; return true; }
    if (std::strcmp(name, "integer") == 0) { out = ScaleMode::Integer; return true; }
    if (std::strcmp(name, "stretch") == 0) { out = ScaleMode::Stretch; return true; }
    return false;
}

Viewport computeViewport(int frame_width, int frame_height,
                         int view_width, int view_height,
                         const DisplaySettings& settings) {
    Viewport viewport;
    if (frame_width <= 0 || frame_height <= 0 || view_width <= 0 || view_height <= 0) {
        return viewport;
    }

    if (settings.mode == ScaleMode::Stretch) {
        viewport.width = view_width;
        viewport.height = view_height;
        return viewport;
    }

    const double frame_aspect = static_cast<double>(frame_width) / frame_height;
    const double view_aspect = static_cast<double>(view_width) / view_height;

    if (settings.mode == ScaleMode::Fill) {
        viewport.width = view_width;
        viewport.height = view_height;
        return viewport;
    }

    if (settings.mode == ScaleMode::Integer) {
        const int factor = std::max(1, static_cast<int>(
            std::min(static_cast<double>(view_width) / frame_width,
                     static_cast<double>(view_height) / frame_height)));
        viewport.width = frame_width * factor;
        viewport.height = frame_height * factor;
        /* Never overflow the view even if the frame is bigger than it. */
        if (viewport.width > view_width || viewport.height > view_height) {
            viewport.width = view_width;
            viewport.height = static_cast<int>(view_width / frame_aspect);
        }
        viewport.x = (view_width - viewport.width) / 2;
        viewport.y = (view_height - viewport.height) / 2;
        return viewport;
    }

    /* Fit: letterbox, never crop. */
    if (frame_aspect > view_aspect) {
        viewport.width = view_width;
        viewport.height = static_cast<int>(view_width / frame_aspect);
    } else {
        viewport.height = view_height;
        viewport.width = static_cast<int>(view_height * frame_aspect);
    }
    viewport.x = (view_width - viewport.width) / 2;
    viewport.y = (view_height - viewport.height) / 2;
    return viewport;
}

} // namespace aurora
