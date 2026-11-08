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
const char* aurora_bridge_last_error(void);
const char* aurora_bridge_state_name(int32_t state);

#ifdef __cplusplus
}
#endif

#endif /* AURORA_BRIDGE_H */
