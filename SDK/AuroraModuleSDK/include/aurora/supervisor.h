/* Module supervisor: the only code that drives module lifecycles.
 *
 * What it can do: catch every error a module reports, tear the instance down,
 * keep user data, mark the entry FAILED and keep serving the other modules.
 * What it cannot do: survive a SIGSEGV inside a native module. All modules share
 * the app process, so a hard crash takes Aurora down with them. That limit is
 * documented in the fault-isolation spec instead of being papered over.
 */
#ifndef AURORA_SUPERVISOR_H
#define AURORA_SUPERVISOR_H

#include <map>
#include <string>

#include "aurora/host.h"
#include "aurora/registry.h"

namespace aurora {

struct RuntimeInstance {
    std::string module_id = "";
    HostBinding* binding = nullptr;
    void* handle = nullptr;
    bool started = false;
    AuroraModuleInstanceStatus status{};
};

/* Fatal: the module itself is broken and a retry needs a reset.
 * Everything else leaves the module READY so the user can try another game. */
bool isFatalStatus(AuroraStatus status);

class ModuleSupervisor {
public:
    ModuleSupervisor(ModuleRegistry& registry, ModuleHost& host);
    ~ModuleSupervisor();

    ModuleSupervisor(const ModuleSupervisor&) = delete;
    ModuleSupervisor& operator=(const ModuleSupervisor&) = delete;

    AuroraStatus start(const std::string& module_id,
                       const AuroraGameInfo& game,
                       std::string& error);
    AuroraStatus tick(const std::string& module_id, double delta_seconds, std::string& error);
    AuroraStatus pause(const std::string& module_id, std::string& error);
    AuroraStatus resume(const std::string& module_id, std::string& error);
    AuroraStatus stop(const std::string& module_id, std::string& error);
    void shutdownAll();

    bool isRunning(const std::string& module_id) const;
    const RuntimeInstance* instance(const std::string& module_id) const;

    /* Clears a sticky FAILED flag after the user asks for a retry. */
    bool resetFailure(const std::string& module_id, std::string& error);

private:
    AuroraStatus fail(const std::string& module_id,
                      AuroraStatus status,
                      const std::string& message,
                      std::string& error);
    void teardown(const std::string& module_id);

    ModuleRegistry& registry_;
    ModuleHost& host_;
    std::map<std::string, RuntimeInstance> instances_;
};

} // namespace aurora

#endif
