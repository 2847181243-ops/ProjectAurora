// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

/* Plain C surface for Swift.
 *
 * The UI never sees a C++ type; the C++ layer never learns UIKit exists. Rows are
 * exposed as one string per field because fixed-size char arrays in a struct
 * arrive in Swift as unreadable tuples.
 */
#ifndef AURORA_BRIDGE_H
#define AURORA_BRIDGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t aurora_bridge_init(const char* storage_root, const char* manifest_directory);
void aurora_bridge_shutdown(void);

int32_t aurora_bridge_module_count(void);
const char* aurora_bridge_module_id(int32_t index);
const char* aurora_bridge_module_name(int32_t index);
const char* aurora_bridge_module_platform(int32_t index);
const char* aurora_bridge_module_detail(int32_t index);
int32_t aurora_bridge_module_state(int32_t index);
int32_t aurora_bridge_module_enabled(int32_t index);
int32_t aurora_bridge_module_compiled(int32_t index);
int32_t aurora_bridge_module_frames(int32_t index);

int32_t aurora_bridge_set_enabled(const char* module_id, int32_t enabled);
int32_t aurora_bridge_start(const char* module_id, const char* game_path, const char* format);
void aurora_bridge_tick(double delta_seconds);
int32_t aurora_bridge_stop(const char* module_id);
int32_t aurora_bridge_reset_failure(const char* module_id);

const uint8_t* aurora_bridge_frame_data(void);
int32_t aurora_bridge_frame_width(void);
int32_t aurora_bridge_frame_height(void);
int32_t aurora_bridge_frames_presented(void);
const char* aurora_bridge_running_module(void);
/* Library ------------------------------------------------------------------ */
/* Games live in <storage_root>/Games. Scanning is cheap and safe to repeat. */
const char* aurora_bridge_games_root(void);
int32_t aurora_bridge_library_scan(void);
int32_t aurora_bridge_library_count(void);
const char* aurora_bridge_library_title(int32_t index);
const char* aurora_bridge_library_module(int32_t index);
const char* aurora_bridge_library_state(int32_t index);
const char* aurora_bridge_library_detail(int32_t index);
/* Copies a picked folder or file into the games root. Returns 0 on success. */
int32_t aurora_bridge_library_import(const char* source_path);

/* Input -------------------------------------------------------------------- */
/* Normalised (0..1) touch position inside the video view, fed through the
 * default profile so the module sees buttons and frame-pixel coordinates. */
void aurora_bridge_set_touch(int32_t active, float x, float y);

/* Display ------------------------------------------------------------------ */
/* 0 fit, 1 fill, 2 integer, 3 stretch. Mirrors Aurora's display policy. */
void aurora_bridge_compute_viewport(int32_t frame_width, int32_t frame_height,
                                    int32_t view_width, int32_t view_height,
                                    int32_t mode,
                                    int32_t* out_x, int32_t* out_y,
                                    int32_t* out_width, int32_t* out_height);

const char* aurora_bridge_last_error(void);
const char* aurora_bridge_state_name(int32_t state);

#ifdef __cplusplus
}
#endif

#endif /* AURORA_BRIDGE_H */
