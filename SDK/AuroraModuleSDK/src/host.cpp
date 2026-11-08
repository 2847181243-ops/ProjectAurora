// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aurora/host.h"

#include <chrono>
#include <cstring>

namespace aurora {

namespace {
bool sameId(const char* a, const std::string& b) {
    if (a == nullptr) return false;
    return b == a;
}
} // namespace

ModuleHost::ModuleHost(std::string storageRoot, Diagnostics& diagnostics)
    : storage_(std::move(storageRoot)), diagnostics_(diagnostics) {}

ModuleHost::~ModuleHost() = default;

double ModuleHost::monotonicSeconds() {
    using clock = std::chrono::steady_clock;
    static const clock::time_point origin = clock::now();
    const auto now = clock::now();
    return std::chrono::duration<double>(now - origin).count();
}

HostBinding* ModuleHost::createBinding(const std::string& moduleId) {
    auto binding = std::make_unique<HostBinding>();
    binding->host = this;
    binding->module_id = moduleId;
    binding->services.struct_size = sizeof(AuroraHostServices);
    binding->services.abi_major = AURORA_ABI_MAJOR;
    binding->services.abi_minor = AURORA_ABI_MINOR;
    binding->services.host_context = binding.get();
    binding->services.storage_directory = &ModuleHost::onStorageDirectory;
    binding->services.write_log = &ModuleHost::onWriteLog;
    binding->services.acquire_frame = &ModuleHost::onAcquireFrame;
    binding->services.present_frame = &ModuleHost::onPresentFrame;
    binding->services.poll_input = &ModuleHost::onPollInput;
    binding->services.monotonic_seconds = &ModuleHost::onMonotonicSeconds;
    HostBinding* raw = binding.get();
    bindings_.push_back(std::move(binding));
    if (default_surface_width_ > 0 && default_surface_height_ > 0) {
        enableSurface(*raw, default_surface_width_, default_surface_height_);
    }
    return raw;
}

void ModuleHost::releaseBinding(HostBinding* binding) {
    for (auto it = bindings_.begin(); it != bindings_.end(); ++it) {
        if (it->get() == binding) {
            bindings_.erase(it);
            return;
        }
    }
}

void ModuleHost::setDefaultSurface(uint32_t width, uint32_t height) {
    default_surface_width_ = width;
    default_surface_height_ = height;
}

void ModuleHost::enableSurface(HostBinding& binding, uint32_t width, uint32_t height) {
    binding.surface_enabled = width > 0 && height > 0;
    binding.surface_width = width;
    binding.surface_height = height;
    binding.surface_pixels.assign(static_cast<std::size_t>(width) * height * 4u, 0u);
    binding.owns_frame = false;
    binding.current_frame = AuroraFrame{};
}

void ModuleHost::setInput(HostBinding& binding, const AuroraInputState& input) {
    binding.input = input;
}

AuroraStatus ModuleHost::onStorageDirectory(void* context, const char* module_id,
                                            AuroraStorageKind kind, const char* relative_path,
                                            char* out_utf8, std::size_t out_capacity) {
    auto* binding = static_cast<HostBinding*>(context);
    if (binding == nullptr || out_utf8 == nullptr || out_capacity == 0) return AURORA_E_INVALID_ARGUMENT;
    if (!sameId(module_id, binding->module_id)) {
        binding->violation = "storage request for another module: ";
        binding->violation += module_id ? module_id : "(null)";
        binding->host->diagnostics().record(AURORA_LOG_ERROR, binding->module_id, binding->violation);
        return AURORA_E_INVALID_ARGUMENT;
    }
    if (kind < 0 || kind >= AURORA_STORAGE_COUNT) return AURORA_E_INVALID_ARGUMENT;

    std::string error;
    ModuleHost* host = binding->host;
    if (!host->storage().ensureBucket(binding->module_id, kind, error)) {
        binding->violation = "storage bucket unavailable: " + error;
        return AURORA_E_IO;
    }
    const std::string path = host->storage().resolve(
        binding->module_id, kind, relative_path ? relative_path : "", error);
    if (path.empty()) {
        binding->violation = "storage path rejected: " + error;
        binding->host->diagnostics().record(AURORA_LOG_ERROR, binding->module_id, binding->violation);
        return AURORA_E_INVALID_ARGUMENT;
    }
    if (path.size() + 1 > out_capacity) {
        binding->violation = "storage path buffer too small";
        binding->host->diagnostics().record(AURORA_LOG_ERROR, binding->module_id, binding->violation);
        return AURORA_E_INVALID_ARGUMENT;
    }
    std::memcpy(out_utf8, path.c_str(), path.size() + 1);
    return AURORA_OK;
}

void ModuleHost::onWriteLog(void* context, const char* module_id, AuroraLogLevel level,
                            const char* message) {
    auto* binding = static_cast<HostBinding*>(context);
    if (binding == nullptr) return;
    if (!sameId(module_id, binding->module_id)) {
        binding->violation = "log call claimed another module";
        binding->host->diagnostics().record(AURORA_LOG_ERROR, binding->module_id, binding->violation);
        return;
    }
    binding->host->diagnostics().record(level, binding->module_id, message ? message : "");
}

AuroraStatus ModuleHost::onAcquireFrame(void* context, uint32_t width, uint32_t height,
                                        AuroraFrame* out_frame) {
    auto* binding = static_cast<HostBinding*>(context);
    if (binding == nullptr || out_frame == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!binding->surface_enabled) return AURORA_E_UNSUPPORTED;
    /* Each system has its own native resolution, so the module states the size it
     * wants and the host reallocates. A fixed platform-side size would force every
     * module to letterbox into somebody else's aspect ratio. */
    if (width == 0 || height == 0 || width > 4096 || height > 2400) return AURORA_E_INVALID_ARGUMENT;
    if (width != binding->surface_width || height != binding->surface_height ||
        binding->surface_pixels.size() != static_cast<std::size_t>(width) * height * 4u) {
        binding->surface_width = width;
        binding->surface_height = height;
        binding->surface_pixels.assign(static_cast<std::size_t>(width) * height * 4u, 0u);
    }

    AuroraFrame frame{};
    frame.pixels = binding->surface_pixels.data();
    frame.width = binding->surface_width;
    frame.height = binding->surface_height;
    frame.stride_bytes = binding->surface_width * 4u;
    frame.format = AURORA_PIXEL_RGBA8888;
    binding->current_frame = frame;
    binding->owns_frame = true;
    *out_frame = frame;
    return AURORA_OK;
}

AuroraStatus ModuleHost::onPresentFrame(void* context, const AuroraFrame* frame) {
    auto* binding = static_cast<HostBinding*>(context);
    if (binding == nullptr || frame == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!binding->surface_enabled) return AURORA_E_UNSUPPORTED;
    if (frame->pixels != binding->surface_pixels.data()) return AURORA_E_INVALID_ARGUMENT;
    if (frame->width != binding->surface_width || frame->height != binding->surface_height) {
        return AURORA_E_INVALID_ARGUMENT;
    }
    return AURORA_OK;
}

AuroraStatus ModuleHost::onPollInput(void* context, AuroraInputState* out_state) {
    auto* binding = static_cast<HostBinding*>(context);
    if (binding == nullptr || out_state == nullptr) return AURORA_E_INVALID_ARGUMENT;
    *out_state = binding->input;
    binding->last_input_poll_seconds = monotonicSeconds();
    return AURORA_OK;
}

double ModuleHost::onMonotonicSeconds(void*) {
    return monotonicSeconds();
}

} // namespace aurora
