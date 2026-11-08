/* Headless host. Same SDK, same registry, no display.
 *
 * This binary is the reason the architecture claim is testable: the platform half
 * of Aurora runs without UIKit, so module registration, lifecycle and failure
 * handling can be exercised on a build machine long before an iPhone is plugged in.
 */
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "aurora/builtin_modules.h"
#include "aurora/diagnostics.h"
#include "aurora/host.h"
#include "aurora/registry.h"
#include "aurora/storage.h"
#include "aurora/supervisor.h"

namespace {

void printUsage() {
    std::printf(
        "aurora-cli - headless Aurora platform host\n"
        "\n"
        "usage:\n"
        "  aurora-cli list [--manifests DIR] [--data DIR]\n"
        "  aurora-cli run <module-id> [--frames N] [--format F] [--out FILE.ppm]\n"
        "                [--manifests DIR] [--data DIR] [--disable ID]\n"
        "\n"
        "  --manifests  directory holding module manifests (default: build-time path)\n"
        "  --data       AuroraData root (default: ./AuroraData)\n");
}

std::string joinPath(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    std::filesystem::path left(a);
    left /= b;
    return left.generic_string();
}

struct Options {
    std::string manifests = AURORA_DEFAULT_MANIFEST_DIR;
    std::string data = "AuroraData";
    std::string out;
    std::string format = "sim";
    std::string disable;
    int frames = 60;
    std::string command;
    std::string moduleId;
};

bool parseOptions(int argc, char** argv, Options& options) {
    if (argc < 2) return false;
    options.command = argv[1];
    for (int i = 2; i < argc; ++i) {
        const std::string arg = argv[i];
        const bool hasValue = i + 1 < argc;
        if (arg == "--manifests" && hasValue) { options.manifests = argv[++i]; }
        else if (arg == "--data" && hasValue) { options.data = argv[++i]; }
        else if (arg == "--out" && hasValue) { options.out = argv[++i]; }
        else if (arg == "--format" && hasValue) { options.format = argv[++i]; }
        else if (arg == "--disable" && hasValue) { options.disable = argv[++i]; }
        else if (arg == "--frames" && hasValue) { options.frames = std::atoi(argv[++i]); }
        else if (arg.rfind("--", 0) == 0) { std::printf("unknown option: %s\n", arg.c_str()); return false; }
        else if (options.moduleId.empty()) { options.moduleId = arg; }
        else { std::printf("unexpected argument: %s\n", arg.c_str()); return false; }
    }
    return true;
}

bool loadManifests(aurora::ModuleRegistry& registry, const std::string& directory) {
    std::error_code code;
    if (!std::filesystem::is_directory(directory, code)) {
        std::printf("manifest directory not found: %s\n", directory.c_str());
        return false;
    }
    std::vector<std::filesystem::path> paths;
    for (const auto& entry : std::filesystem::directory_iterator(directory, code)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;
        paths.push_back(entry.path());
    }
    std::sort(paths.begin(), paths.end());
    for (const auto& path : paths) {
        std::ifstream stream(path);
        const std::string text((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        std::string error;
        const bool ok = registry.addManifestJson(text, path.filename().generic_string(), error);
        if (!ok) std::printf("manifest problem [%s]: %s\n", path.filename().generic_string().c_str(), error.c_str());
    }
    return true;
}

int attachBuiltins(aurora::ModuleRegistry& registry) {
    int attached = 0;
    for (std::size_t i = 0; i < aurora_builtin_module_count; ++i) {
        std::string error;
        if (registry.attachBuiltin(aurora_builtin_modules[i].entry, aurora_builtin_modules[i].module_id, error)) {
            ++attached;
        } else if (!error.empty() && error.find("no manifest") == std::string::npos) {
            std::printf("link problem [%s]: %s\n", aurora_builtin_modules[i].module_id, error.c_str());
        }
    }
    return attached;
}

void printTable(const aurora::ModuleRegistry& registry) {
    std::printf("%-11s %-36s %-24s %s\n", "STATE", "MODULE", "NAME", "DETAIL");
    for (const auto& entry : registry.entries()) {
        std::string detail;
        if (!entry.last_error.empty()) {
            detail = entry.last_error;
        } else if (!entry.unmet.empty()) {
            for (std::size_t i = 0; i < entry.unmet.size(); ++i) {
                if (i != 0) detail += ", ";
                detail += entry.unmet[i];
            }
        } else {
            detail = "-";
        }
        std::printf("%-11s %-36s %-24s %s\n",
                    aurora_module_state_string(entry.state),
                    entry.manifest.module_id.empty() ? "(invalid)" : entry.manifest.module_id.c_str(),
                    entry.manifest.name.empty() ? "-" : entry.manifest.name.c_str(),
                    detail.c_str());
    }
}

bool writePpm(const std::string& path, const aurora::HostBinding& binding) {
    if (!binding.surface_enabled || binding.surface_pixels.empty()) return false;
    std::ofstream out(path, std::ios::binary);
    if (!out.good()) return false;
    out << "P6\n" << binding.surface_width << " " << binding.surface_height << "\n255\n";
    for (std::uint32_t y = 0; y < binding.surface_height; ++y) {
        for (std::uint32_t x = 0; x < binding.surface_width; ++x) {
            const std::size_t index =
                (static_cast<std::size_t>(y) * binding.surface_width + x) * 4u;
            const char rgb[3] = {
                static_cast<char>(binding.surface_pixels[index + 0]),
                static_cast<char>(binding.surface_pixels[index + 1]),
                static_cast<char>(binding.surface_pixels[index + 2]),
            };
            out.write(rgb, 3);
        }
    }
    return out.good();
}

} // namespace

int main(int argc, char** argv) {
    Options options;
    if (!parseOptions(argc, argv, options)) {
        printUsage();
        return 2;
    }

    aurora::SemVer hostSdk;
    aurora::SemVer::parse("1.0.0", hostSdk);
    aurora::Diagnostics diagnostics;
    diagnostics.setClockSource(&aurora::ModuleHost::monotonicSeconds);

    std::string storageRoot = joinPath(options.data, "");
    aurora::ModuleHost host(storageRoot, diagnostics);
    aurora::ModuleRegistry registry(hostSdk);
    registry.setFirmwareProbe([](const std::string&) { return false; });

    if (!loadManifests(registry, options.manifests)) return 2;
    const int attached = attachBuiltins(registry);
    registry.evaluateAll();

    if (!options.disable.empty()) {
        std::string error;
        if (!registry.setEnabled(options.disable, false, error)) {
            std::printf("disable failed: %s\n", error.c_str());
        }
    }

    if (options.command == "list") {
        std::printf("host SDK 1.0.0, %d module(s) linked, %zu catalog entr(ies)\n",
                    attached, registry.entries().size());
        printTable(registry);
        return 0;
    }

    if (options.command == "run") {
        if (options.moduleId.empty()) {
            std::printf("run needs a module id\n");
            return 2;
        }
        if (options.frames > 0) host.setDefaultSurface(320, 240);

        aurora::ModuleSupervisor supervisor(registry, host);
        AuroraGameInfo game{};
        game.game_path = "/library/demo.sim";
        game.format = options.format.c_str();
        game.display_name = "Demo";

        std::string error;
        const AuroraStatus started = supervisor.start(options.moduleId, game, error);
        if (started != AURORA_OK) {
            std::printf("start failed (%s): %s\n", aurora_status_string(started), error.c_str());
            printTable(registry);
            return 1;
        }

        for (int i = 0; i < options.frames; ++i) {
            if (supervisor.tick(options.moduleId, 1.0 / 60.0, error) != AURORA_OK) {
                std::printf("tick failed: %s\n", error.c_str());
                break;
            }
        }

        const aurora::RuntimeInstance* instance = supervisor.instance(options.moduleId);
        if (instance != nullptr) {
            std::printf("frames presented: %lld\n",
                        static_cast<long long>(instance->status.frames_presented));
            if (!options.out.empty() && instance->binding != nullptr) {
                const bool written = writePpm(options.out, *instance->binding);
                std::printf("frame written: %s\n", written ? options.out.c_str() : "failed");
            }
        }

        if (supervisor.stop(options.moduleId, error) != AURORA_OK) {
            std::printf("stop reported: %s\n", error.c_str());
        }
        printTable(registry);
        std::printf("\n-- diagnostics --\n%s", diagnostics.format().c_str());
        return 0;
    }

    printUsage();
    return 2;
}
