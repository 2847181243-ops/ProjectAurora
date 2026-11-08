/* Aurora Module ABI - the only contract between the Aurora Platform and a runtime module.
 *
 * Rules this header exists to enforce:
 *   1. Plain C ABI. No C++ types, no exceptions, no STL across the boundary.
 *      Swift binds it through the bridging header; Objective-C++ calls it directly.
 *   2. No module-to-module symbols. A module reaches the platform only through
 *      AuroraHostServices, handed over at create_instance().
 *   3. On iOS, modules are linked into the app at build time. Nothing in this
 *      design dlopen()s downloadable native code.
 */
#ifndef AURORA_MODULE_API_H
#define AURORA_MODULE_API_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AURORA_ABI_MAJOR 1u
#define AURORA_ABI_MINOR 0u

/* Every entry point returns AuroraStatus. A module never throws across the ABI and
 * never reports success for a partial operation. */
typedef enum AuroraStatus {
    AURORA_OK                 = 0,
    AURORA_E_INVALID_ARGUMENT = 1,
    AURORA_E_UNSUPPORTED      = 2,  /* operation exists, this module does not implement it */
    AURORA_E_UNSUPPORTED_GAME = 3,  /* format/platform not handled by this module */
    AURORA_E_MISSING_FIRMWARE = 4,  /* needs a BIOS/firmware image the user must supply */
    AURORA_E_IO               = 5,
    AURORA_E_OUT_OF_MEMORY    = 6,
    AURORA_E_INIT_FAILED      = 7,
    AURORA_E_STATE            = 8,  /* illegal in the module's current state */
    AURORA_E_TIMEOUT          = 9,
    AURORA_E_INTERNAL         = 10,
    AURORA_E_CANCELLED        = 11,
    AURORA_E_ABI_MISMATCH     = 12
} AuroraStatus;

/* Never NULL. */
const char* aurora_status_string(AuroraStatus status);

/* PLANNED and BUILDING come from the build pipeline, never from a running app.
 * They live here so the UI and the build report share one vocabulary. */
typedef enum AuroraModuleState {
    AURORA_STATE_DISCOVERED = 0, /* manifest known, no usable binary */
    AURORA_STATE_PLANNED    = 1,
    AURORA_STATE_BUILDING   = 2,
    AURORA_STATE_BUILT      = 3, /* compiled and linked, not yet started */
    AURORA_STATE_READY      = 4, /* every precondition met, can start a game */
    AURORA_STATE_RUNNING    = 5,
    AURORA_STATE_DISABLED   = 6, /* user turned it off; its data must survive */
    AURORA_STATE_FAILED     = 7,
    AURORA_STATE_DEPRECATED = 8
} AuroraModuleState;

const char* aurora_module_state_string(AuroraModuleState state);

typedef enum AuroraRuntimeKind {
    AURORA_KIND_GALGAME          = 0,
    AURORA_KIND_CONSOLE_EMULATOR = 1,
    AURORA_KIND_OTHER            = 2
} AuroraRuntimeKind;

const char* aurora_runtime_kind_string(AuroraRuntimeKind kind);

/* Declared capabilities. The platform treats them as claims: a JIT claim still
 * needs a runtime probe before the UI promises anything. */
typedef enum AuroraCapability {
    AURORA_CAP_NONE              = 0u,
    AURORA_CAP_SAVE_STATE        = 1u << 0,
    AURORA_CAP_GAME_SAVES        = 1u << 1,
    AURORA_CAP_CONTROLLER        = 1u << 2,
    AURORA_CAP_TOUCH             = 1u << 3,
    AURORA_CAP_KEYBOARD          = 1u << 4,
    AURORA_CAP_FAST_FORWARD      = 1u << 5,
    AURORA_CAP_AUDIO             = 1u << 6,
    AURORA_CAP_VIDEO_OUTPUT      = 1u << 7,
    AURORA_CAP_JIT_REQUIRED      = 1u << 8,
    AURORA_CAP_JIT_OPTIONAL      = 1u << 9,
    AURORA_CAP_SHADERS           = 1u << 10,
    AURORA_CAP_MULTIPLAYER       = 1u << 11,
    AURORA_CAP_REQUIRES_FIRMWARE = 1u << 12
} AuroraCapability;

typedef uint32_t AuroraCapabilityMask;

typedef enum AuroraStorageKind {
    AURORA_STORAGE_CONFIG = 0,
    AURORA_STORAGE_SAVES  = 1,
    AURORA_STORAGE_STATES = 2,
    AURORA_STORAGE_LOGS   = 3,
    AURORA_STORAGE_COUNT  = 4
} AuroraStorageKind;

const char* aurora_storage_kind_string(AuroraStorageKind kind);

typedef enum AuroraLogLevel {
    AURORA_LOG_DEBUG   = 0,
    AURORA_LOG_INFO    = 1,
    AURORA_LOG_WARNING = 2,
    AURORA_LOG_ERROR   = 3
} AuroraLogLevel;

typedef enum AuroraPixelFormat {
    AURORA_PIXEL_RGBA8888 = 0,
    AURORA_PIXEL_BGRA8888 = 1
} AuroraPixelFormat;

typedef struct AuroraFrame {
    void* pixels;             /* host-owned buffer, width * height * 4 bytes */
    uint32_t width;
    uint32_t height;
    uint32_t stride_bytes;
    AuroraPixelFormat format;
} AuroraFrame;

typedef enum AuroraButton {
    AURORA_BTN_A      = 1u << 0,
    AURORA_BTN_B      = 1u << 1,
    AURORA_BTN_X      = 1u << 2,
    AURORA_BTN_Y      = 1u << 3,
    AURORA_BTN_UP     = 1u << 4,
    AURORA_BTN_DOWN   = 1u << 5,
    AURORA_BTN_LEFT   = 1u << 6,
    AURORA_BTN_RIGHT  = 1u << 7,
    AURORA_BTN_START  = 1u << 8,
    AURORA_BTN_SELECT = 1u << 9,
    AURORA_BTN_L1     = 1u << 10,
    AURORA_BTN_R1     = 1u << 11
} AuroraButton;

typedef struct AuroraInputState {
    int16_t axis_left_x;
    int16_t axis_left_y;
    int16_t axis_right_x;
    int16_t axis_right_y;
    uint32_t buttons;       /* AuroraButton mask */
    int32_t touch_x;
    int32_t touch_y;
    uint8_t touch_active;
    uint8_t reserved[3];
} AuroraInputState;

typedef struct AuroraGameInfo {
    const char* game_path;
    const char* format;       /* lowercase extension, no dot: "iso", "xp3" */
    const char* display_name; /* nullable */
} AuroraGameInfo;

typedef struct AuroraModuleInstanceStatus {
    AuroraModuleState state;
    int64_t frames_presented;
    int64_t frames_dropped;
    double average_fps;
    const char* name;         /* borrowed, valid until the next call on the instance */
} AuroraModuleInstanceStatus;

/* ---- Platform -> module -----------------------------------------------------
 * Everything a module needs from the outside world arrives through this table.
 * The platform owns it; the module borrows it for the lifetime of an instance.
 * Host functions validate their arguments and return errors instead of aborting. */
typedef struct AuroraHostServices {
    uint32_t struct_size;
    uint32_t abi_major;
    uint32_t abi_minor;
    void* host_context;

    /* Absolute directory for (module_id, kind). relative_path may be NULL or "".
     * Returns AURORA_E_INVALID_ARGUMENT when the module tries to leave its tree. */
    AuroraStatus (*storage_directory)(void* context,
                                      const char* module_id,
                                      AuroraStorageKind kind,
                                      const char* relative_path,
                                      char* out_utf8,
                                      size_t out_capacity);

    void (*write_log)(void* context,
                      const char* module_id,
                      AuroraLogLevel level,
                      const char* message);

    /* Video out. The host owns the surface, the module only writes pixels.
     * A headless host (unit tests, CLI) returns AURORA_E_UNSUPPORTED, and modules
     * must degrade instead of failing. */
    AuroraStatus (*acquire_frame)(void* context,
                                  uint32_t desired_width,
                                  uint32_t desired_height,
                                  AuroraFrame* out_frame);
    AuroraStatus (*present_frame)(void* context, const AuroraFrame* frame);

    AuroraStatus (*poll_input)(void* context, AuroraInputState* out_state);

    /* Seconds since an arbitrary fixed point. Never jumps. */
    double (*monotonic_seconds)(void* context);
} AuroraHostServices;

/* ---- Module -> platform ---------------------------------------------------- */
typedef struct AuroraModuleVTable {
    uint32_t struct_size;
    uint32_t abi_major;
    uint32_t abi_minor;
    const char* module_id;      /* must equal the manifest module_id */
    const char* module_version; /* informational, the manifest is authoritative */

    AuroraStatus (*create_instance)(const AuroraHostServices* host, void** out_instance);
    AuroraStatus (*load_game)(void* instance, const AuroraGameInfo* game);
    AuroraStatus (*start)(void* instance);
    AuroraStatus (*pause)(void* instance);
    AuroraStatus (*resume)(void* instance);
    AuroraStatus (*stop)(void* instance);
    AuroraStatus (*query_status)(void* instance, AuroraModuleInstanceStatus* out_status);
    void (*release_instance)(void* instance);

    /* Optional manual step, for deterministic tests and headless capture.
     * NULL means the module drives its own loop between start() and stop(). */
    AuroraStatus (*tick)(void* instance, double delta_seconds);
} AuroraModuleVTable;

typedef const AuroraModuleVTable* (*AuroraModuleEntryFn)(void);

/* One exported symbol per module, with a module-specific name, so two statically
 * linked modules cannot collide. The build system generates the list. */
#define AURORA_MODULE_ENTRY(name) \
    extern "C" const AuroraModuleVTable* aurora_module_entry_##name(void)

AuroraStatus aurora_vtable_validate(const AuroraModuleVTable* vtable);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AURORA_MODULE_API_H */
