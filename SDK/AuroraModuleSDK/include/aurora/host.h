// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

/* The platform side of the ABI: implements AuroraHostServices for one module.
 *
 * Isolation detail that matters: every module gets its own binding, carrying its
 * own module id, its own frame surface and its own input snapshot. A module that
 * asks for another module's storage directory is refused at the callback, not
 * merely documented as forbidden.
 */
#ifndef AURORA_HOST_H
#define AURORA_HOST_H

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "aurora/diagnostics.h"
#include "aurora/module_api.h"
#include "aurora/storage.h"

namespace aurora {

class ModuleHost;

struct HostBinding {
    ModuleHost* host = nullptr;
    std::string module_id;
    AuroraHostServices services{};

    bool surface_enabled = false;
    uint32_t surface_width = 0;
    uint32_t surface_height = 0;
    std::vector<std::uint8_t> surface_pixels;

    AuroraInputState input{};
    double last_input_poll_seconds = 0.0;

    bool owns_frame = false;
    AuroraFrame current_frame{};

    /* Audio bookkeeping. The host may have no output device (tests, CLI); the
     * module must not care, so submissions are still counted and recorded. */
    std::uint64_t audio_frames_received = 0;
    float audio_peak = 0.0f;
    AuroraAudioSpec audio_spec_seen{};

    /* Set when the module tried something the boundary forbids, so the platform
     * can report a concrete violation instead of a mystery failure. */
    std::string violation;
};

class ModuleHost {
public:
    ModuleHost(std::string storageRoot, Diagnostics& diagnostics);
    ~ModuleHost();

    ModuleHost(const ModuleHost&) = delete;
    ModuleHost& operator=(const ModuleHost&) = delete;

    /* Returns a binding owned by the host. One per module instance. */
    HostBinding* createBinding(const std::string& moduleId);
    void releaseBinding(HostBinding* binding);

    /* Applied to every binding created afterwards. Zero means headless. */
    void setDefaultSurface(uint32_t width, uint32_t height);

    /* Frame surface is opt-in: a headless host (unit tests, CLI without --frames)
     * answers AURORA_E_UNSUPPORTED, which modules must tolerate. */
    void enableSurface(HostBinding& binding, uint32_t width, uint32_t height);
    void setInput(HostBinding& binding, const AuroraInputState& input);

    const StorageLayout& storage() const { return storage_; }
    StorageLayout& storage() { return storage_; }
    Diagnostics& diagnostics() { return diagnostics_; }

    static double monotonicSeconds();

    /* Optional sink: the iOS bridge pushes submitted frames into AVAudioEngine,
     * tests read them back. Called on the module's thread. */
    using AudioSink = std::function<void(const AuroraAudioBuffer&)>;
    void setAudioSink(AudioSink sink) { audio_sink_ = std::move(sink); }
    void setDefaultAudioSpec(const AuroraAudioSpec& spec) { audio_spec_ = spec; }
    const AuroraAudioSpec& defaultAudioSpec() const { return audio_spec_; }

    std::size_t bindingCount() const { return bindings_.size(); }

private:
    static AuroraStatus onStorageDirectory(void* context, const char* module_id,
                                           AuroraStorageKind kind, const char* relative_path,
                                           char* out_utf8, std::size_t out_capacity);
    static void onWriteLog(void* context, const char* module_id, AuroraLogLevel level,
                           const char* message);
    static AuroraStatus onAcquireFrame(void* context, uint32_t width, uint32_t height,
                                       AuroraFrame* out_frame);
    static AuroraStatus onPresentFrame(void* context, const AuroraFrame* frame);
    static AuroraStatus onPollInput(void* context, AuroraInputState* out_state);
    static double onMonotonicSeconds(void* context);
    static AuroraStatus onAudioSpec(void* context, AuroraAudioSpec* out_spec);
    static AuroraStatus onAudioSubmit(void* context, const AuroraAudioBuffer* buffer);

    StorageLayout storage_;
    Diagnostics& diagnostics_;
    uint32_t default_surface_width_ = 0;
    uint32_t default_surface_height_ = 0;
    AuroraAudioSpec audio_spec_{48000u, 2u, AURORA_AUDIO_F32};
    AudioSink audio_sink_;
    std::vector<std::unique_ptr<HostBinding>> bindings_;
};

} // namespace aurora

#endif
