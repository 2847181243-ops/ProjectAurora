#ifndef AURORA_TEST_ENV_H
#define AURORA_TEST_ENV_H

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "aurora/builtin_modules.h"
#include "aurora/diagnostics.h"
#include "aurora/host.h"
#include "aurora/registry.h"
#include "aurora/storage.h"
#include "aurora/supervisor.h"

namespace aurora {
namespace test {

inline std::string readFile(const std::string& path) {
    std::ifstream stream(path);
    if (!stream.good()) return std::string();
    return std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
}

inline std::string shippedManifest(const char* file_name) {
    std::filesystem::path path(AURORA_DEFAULT_MANIFEST_DIR);
    path /= file_name;
    return readFile(path.generic_string());
}

inline std::string freshDirectory(const std::string& name) {
    std::filesystem::path path = std::filesystem::temp_directory_path();
    path /= "aurora-tests";
    path /= name;
    std::error_code code;
    std::filesystem::remove_all(path, code);
    std::filesystem::create_directories(path, code);
    return path.generic_string();
}

inline SemVer sdkVersion() {
    SemVer version;
    SemVer::parse("1.0.0", version);
    return version;
}

/* Registers the manifests shipped with the project. Failures are reported so a
 * broken manifest cannot quietly vanish from a test run. */
inline void loadShippedManifests(ModuleRegistry& registry) {
    const char* files[] = {
        "com.aurora.runtime.empty.json",
        "com.aurora.runtime.simulated.json",
        "com.aurora.runtime.faultinjector.json",
        "com.aurora.runtime.ppsspp.json",
    };
    for (const char* file : files) {
        const std::string text = shippedManifest(file);
        if (text.empty()) {
            std::printf("  FATAL cannot read shipped manifest %s\n", file);
            continue;
        }
        std::string error;
        registry.addManifestJson(text, file, error);
    }
}

inline int attachShippedBuiltins(ModuleRegistry& registry) {
    int attached = 0;
    for (std::size_t i = 0; i < aurora_builtin_module_count; ++i) {
        std::string error;
        if (registry.attachBuiltin(aurora_builtin_modules[i].entry, aurora_builtin_modules[i].module_id, error)) {
            ++attached;
        }
    }
    return attached;
}

struct Environment {
    std::string root;
    Diagnostics diagnostics;
    ModuleHost host;
    ModuleRegistry registry;
    ModuleSupervisor supervisor;

    explicit Environment(const std::string& name)
        : root(freshDirectory(name)),
          host(root, diagnostics),
          registry(sdkVersion()),
          supervisor(registry, host) {
        diagnostics.setClockSource(&ModuleHost::monotonicSeconds);
        registry.setFirmwareProbe([](const std::string&) { return false; });
        loadShippedManifests(registry);
        attachShippedBuiltins(registry);
        registry.evaluateAll();
    }
};

inline AuroraGameInfo gameInfo(const char* path, const char* format) {
    AuroraGameInfo info{};
    info.game_path = path;
    info.format = format;
    info.display_name = "Test Game";
    return info;
}

} // namespace test
} // namespace aurora

#endif
