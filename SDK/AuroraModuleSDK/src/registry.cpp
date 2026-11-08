#include "aurora/registry.h"

#include <algorithm>
#include <cstddef>

#include "aurora/state_machine.h"

namespace aurora {

namespace {
std::string firstError(const std::vector<ManifestIssue>& issues) {
    for (const auto& issue : issues) {
        if (issue.severity == "error") return issue.field + ": " + issue.message;
    }
    return std::string();
}
} // namespace

ModuleRegistry::ModuleRegistry(SemVer hostSdk) : host_sdk_(hostSdk) {}

bool ModuleRegistry::addManifestJson(const std::string& json_text,
                                     const std::string& source,
                                     std::string& error) {
    error.clear();
    ManifestParseResult parsed = parseManifest(json_text, source);

    ModuleEntry entry;
    entry.manifest = parsed.manifest;
    entry.manifest_valid = parsed.ok;
    entry.manifest_source = source;
    entry.issues = parsed.issues;

    if (parsed.ok) {
        for (const auto& existing : entries_) {
            if (existing.manifest.module_id == parsed.manifest.module_id) {
                error = "duplicate manifest for " + parsed.manifest.module_id +
                        " (" + existing.manifest_source + " and " + source + ")";
                return false;
            }
        }
    } else {
        entry.state = AURORA_STATE_FAILED;
        entry.last_error = "manifest rejected: " + firstError(parsed.issues);
        error = entry.last_error + " [" + source + "]";
    }

    entries_.push_back(std::move(entry));
    evaluateAll();
    return parsed.ok;
}

bool ModuleRegistry::attachBuiltin(AuroraModuleEntryFn entry,
                                   const char* expected_module_id,
                                   std::string& error) {
    error.clear();
    if (entry == nullptr) {
        error = "null module entry";
        return false;
    }
    const AuroraModuleVTable* vtable = entry();
    const AuroraStatus validation = aurora_vtable_validate(vtable);
    if (validation != AURORA_OK) {
        error = std::string("vtable rejected: ") + aurora_status_string(validation);
        return false;
    }
    const std::string module_id = vtable->module_id;
    if (expected_module_id != nullptr && module_id != expected_module_id) {
        error = "build table says '" + std::string(expected_module_id) +
                "' but the binary reports '" + module_id + "'";
        return false;
    }

    ModuleEntry* target = find(module_id);
    if (target == nullptr) {
        ModuleEntry fallback;
        fallback.manifest.module_id = module_id;
        fallback.manifest_valid = false;
        fallback.manifest_source = "<build table>";
        fallback.state = AURORA_STATE_FAILED;
        fallback.last_error = "module was linked in without a manifest";
        entries_.push_back(std::move(fallback));
        target = &entries_.back();
        error = "module '" + module_id + "' has no manifest";
    } else if (target->vtable != nullptr) {
        error = "module '" + module_id + "' was attached twice";
        return false;
    } else {
        target->compiled_in = true;
        target->vtable = vtable;
        if (target->manifest.version != vtable->module_version) {
            target->issues.push_back({"warning", "version",
                "binary reports '" + std::string(vtable->module_version) +
                "' but the manifest says '" + target->manifest.version + "'"});
        }
    }
    evaluateAll();
    return target->vtable != nullptr;
}

bool ModuleRegistry::setEnabled(const std::string& module_id, bool enabled, std::string& error) {
    error.clear();
    ModuleEntry* entry = find(module_id);
    if (entry == nullptr) {
        error = "unknown module: " + module_id;
        return false;
    }
    if (entry->state == AURORA_STATE_RUNNING) {
        error = "stop the module before changing its enabled flag";
        return false;
    }
    entry->user_enabled = enabled;
    evaluateAll();
    return true;
}

ModuleEntry* ModuleRegistry::find(const std::string& module_id) {
    for (auto& entry : entries_) {
        if (entry.manifest.module_id == module_id) return &entry;
    }
    return nullptr;
}

const ModuleEntry* ModuleRegistry::find(const std::string& module_id) const {
    for (const auto& entry : entries_) {
        if (entry.manifest.module_id == module_id) return &entry;
    }
    return nullptr;
}

std::size_t ModuleRegistry::countInState(AuroraModuleState state) const {
    std::size_t count = 0;
    for (const auto& entry : entries_) {
        if (entry.state == state) ++count;
    }
    return count;
}

void ModuleRegistry::applyDerivedState(ModuleEntry& entry, AuroraModuleState target) {
    if (entry.state == target) return;
    const std::vector<AuroraModuleState> path = transitionPath(entry.state, target);
    if (path.empty()) {
        entry.transition_error = std::string("no legal path from ") +
                                 aurora_module_state_string(entry.state) + " to " +
                                 aurora_module_state_string(target);
        return;
    }
    for (AuroraModuleState step : path) {
        applyTransition(entry.state, step);
    }
    entry.transition_error.clear();
}

void ModuleRegistry::evaluateAll() {
    std::vector<int> marks(entries_.size(), 0);
    for (std::size_t i = 0; i < entries_.size(); ++i) {
        std::vector<std::string> unmet;
        computeState(i, marks, unmet);
    }
}

AuroraModuleState ModuleRegistry::computeState(std::size_t index,
                                               std::vector<int>& marks,
                                               std::vector<std::string>& unmet) {
    if (marks[index] == 2) return entries_[index].state;
    if (marks[index] == 1) {
        unmet.push_back("dependency cycle");
        return AURORA_STATE_BUILT;
    }
    marks[index] = 1;

    ModuleEntry& entry = entries_[index];
    AuroraModuleState target = AURORA_STATE_DISCOVERED;

    if (!entry.manifest_valid) {
        target = AURORA_STATE_FAILED;
    } else if (entry.state == AURORA_STATE_RUNNING) {
        target = AURORA_STATE_RUNNING;
    } else if (!entry.user_enabled) {
        /* Disabling must work even for a module that failed earlier, otherwise the
         * toggle appears to do something and the row never changes. */
        target = AURORA_STATE_DISABLED;
    } else if (entry.state == AURORA_STATE_FAILED && !entry.last_error.empty()) {
        /* A runtime or probe failure is sticky until the user retries. Recomputing
         * preconditions must not silently paper over a crash. */
        target = AURORA_STATE_FAILED;
    } else if (!entry.compiled_in || entry.vtable == nullptr) {
        target = AURORA_STATE_DISCOVERED;
        unmet.push_back("not compiled into this build");
    } else {
        const Manifest& manifest = entry.manifest;
        std::vector<std::string> reasons;
        const Compatibility compatibility =
            checkSdkCompatibility(manifest.sdk_semver, manifest.min_sdk_semver, host_sdk_);
        if (compatibility != Compatibility::Compatible) {
            reasons.push_back(std::string("sdk:") + compatibilityToString(compatibility));
        }
        if ((manifest.capabilities & AURORA_CAP_REQUIRES_FIRMWARE) != 0u) {
            if (!firmware_probe_ || !firmware_probe_(manifest.module_id)) {
                reasons.push_back("firmware_missing");
            }
        }
        for (const auto& dependency_id : manifest.dependencies) {
            const ModuleEntry* dependency = find(dependency_id);
            if (dependency == nullptr) {
                reasons.push_back("dependency_missing:" + dependency_id);
                continue;
            }
            const std::size_t dependency_index =
                static_cast<std::size_t>(dependency - entries_.data());
            std::vector<std::string> dependency_unmet;
            const AuroraModuleState dependency_state =
                computeState(dependency_index, marks, dependency_unmet);
            if (dependency_state != AURORA_STATE_READY &&
                dependency_state != AURORA_STATE_RUNNING) {
                reasons.push_back("dependency_not_ready:" + dependency_id);
            }
        }
        if (reasons.empty()) {
            target = AURORA_STATE_READY;
        } else {
            target = AURORA_STATE_BUILT;
            unmet = std::move(reasons);
        }
    }

    marks[index] = 2;
    applyDerivedState(entry, target);
    entry.unmet = unmet;
    return entry.state;
}

} // namespace aurora
