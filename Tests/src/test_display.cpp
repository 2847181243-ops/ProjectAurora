// SPDX-License-Identifier: GPL-3.0-or-later
#include "aurora/display.h"
#include "test_harness.h"

using namespace aurora;

AURORA_TEST(display_fit_letterboxes_without_cropping) {
    DisplaySettings settings;
    settings.mode = ScaleMode::Fit;
    const Viewport viewport = computeViewport(320, 240, 480, 320, settings);
    CHECK_EQ(viewport.height, 320);
    CHECK_EQ(viewport.width, 426);
    CHECK_EQ(viewport.x, 27);
    CHECK_EQ(viewport.y, 0);
}

AURORA_TEST(display_integer_mode_snaps_to_whole_pixels) {
    DisplaySettings settings;
    settings.mode = ScaleMode::Integer;
    const Viewport viewport = computeViewport(320, 240, 960, 720, settings);
    CHECK_EQ(viewport.width, 960);
    CHECK_EQ(viewport.height, 720);
    CHECK_EQ(viewport.x, 0);
    CHECK_EQ(viewport.y, 0);

    const Viewport smaller = computeViewport(320, 240, 700, 500, settings);
    CHECK_EQ(smaller.width, 640);
    CHECK_EQ(smaller.height, 480);
    CHECK_EQ(smaller.x, 30);
    CHECK_EQ(smaller.y, 10);
}

AURORA_TEST(display_stretch_and_fill_use_the_whole_view) {
    DisplaySettings stretch;
    stretch.mode = ScaleMode::Stretch;
    const Viewport stretched = computeViewport(320, 240, 480, 320, stretch);
    CHECK_EQ(stretched.width, 480);
    CHECK_EQ(stretched.height, 320);

    DisplaySettings fill;
    fill.mode = ScaleMode::Fill;
    const Viewport filled = computeViewport(320, 240, 480, 320, fill);
    CHECK_EQ(filled.width, 480);
    CHECK_EQ(filled.height, 320);
}

AURORA_TEST(display_rejects_impossible_sizes) {
    DisplaySettings settings;
    const Viewport viewport = computeViewport(0, 240, 480, 320, settings);
    CHECK_EQ(viewport.width, 0);
    CHECK_EQ(viewport.height, 0);
}

AURORA_TEST(display_mode_names_round_trip) {
    ScaleMode mode = ScaleMode::Fit;
    CHECK(scaleModeFromName("integer", mode));
    CHECK_EQ(mode, ScaleMode::Integer);
    CHECK(!scaleModeFromName("bogus", mode));
}
