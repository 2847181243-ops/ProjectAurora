/* Module registry: the platform's single source of truth about what exists.
 *
 * Three inputs are fused, and the platform never trusts one alone:
 *   1. manifests          - what a module claims
 *   2. build record       - what the toolchain actually compiled and linked
 *   3. runtime probe      - what actually happened when we called it
 * A manifest that declares "status": "ready" cannot produce READY here. Only a
 * linked, enabled, SDK-compatible module with satisfied dependencies can.
 */
#ifndef AURORA_REGISTRY_H
#define AURORA_REGISTRY_H

#include <functional>
#include <string>
#include <vector>

#include "aurora/manifest.h"
#include "aurora/module_api.h"

namespace aurora {

struct ModuleEntry {
    Manifest manifest;
    bool manifest_valid = false;
    std::string manifest_source;
    std::vector<ManifestIssue> issues;

    bool compiled_in = false;
    const AuroraModuleVTable* vtable = nullptr;

    bool user_enabled = true;

    AuroraModuleState state = AURORA_STATE_DISCOVERED;
    std::vector<std::string> unmet;      /* why it is not READY */
    std::string last_error;              /* why it is FAILED */
    std::string transition_error;        /* last refused state change */
};

class ModuleRegistry {
public:
    explicit ModuleRegistry(SemVer hostSdk);

    const SemVer& hostSdk() const { return host_sdk_; }

    /* Registers one manifest. A bad manifest does not throw the app off course:
     * it becomes a visible FAILED entry with the parsed issues attached. */
    bool addManifestJson(const std::string& json_text, const std::string& source, std::string& error);

    /* Links a compiled-in module. |expected_module_id| comes from the generated
     * build table; a mismatch means the build table and the binary disagree. */
    bool attachBuiltin(AuroraModuleEntryFn entry,
                       const char* expected_module_id,
                       std::string& error);

    bool setEnabled(const std::string& module_id, bool enabled, std::string& error);

    void setFirmwareProbe(std::function<bool(const std::string& module_id)> probe) {
        firmware_probe_ = std::move(probe);
    }

    /* Recomputes every derived state. Independent modules are evaluated in
     * isolation: one broken entry cannot stop the loop. */
    void evaluateAll();

    std::vector<ModuleEntry>& entries() { return entries_; }
    const std::vector<ModuleEntry>& entries() const { return entries_; }
    ModuleEntry* find(const std::string& module_id);
    const ModuleEntry* find(const std::string& module_id) const;

    std::size_t countInState(AuroraModuleState state) const;

private:
    enum class ComputeResult { Answer, InProgress };

    AuroraModuleState computeState(std::size_t index,
                                   std::vector<int>& marks,
                                   std::vector<std::string>& unmet);
    void applyDerivedState(ModuleEntry& entry, AuroraModuleState target);

    SemVer host_sdk_;
    std::vector<ModuleEntry> entries_;
    std::function<bool(const std::string&)> firmware_probe_;
};

} // namespace aurora

#endif
