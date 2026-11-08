# One target per module. Switching one off must not disturb the others.
#
# The two sample modules are always on. The fault injector is on for tests.

option(AURORA_MODULE_EMPTY "Build the empty sample module" ON)
option(AURORA_MODULE_SIMULATED "Build the simulated runtime module" ON)
option(AURORA_MODULE_FAULTINJECTOR "Build the fault injector used by the tests" ON)

if(AURORA_MODULE_EMPTY)
    aurora_add_module(aurora_module_empty
        "com.aurora.runtime.empty"
        "aurora_module_entry_empty"
        SOURCES EmptyModule/src/empty_module.cpp)
endif()

if(AURORA_MODULE_SIMULATED)
    aurora_add_module(aurora_module_simulated
        "com.aurora.runtime.simulated"
        "aurora_module_entry_simulated"
        SOURCES SimulatedRuntime/src/simulated_runtime.cpp)
endif()

if(AURORA_MODULE_FAULTINJECTOR)
    aurora_add_module(aurora_module_faultinjector
        "com.aurora.runtime.faultinjector"
        "aurora_module_entry_faultinjector"
        SOURCES FaultInjector/src/fault_injector.cpp)
endif()

aurora_emit_builtin_table("${CMAKE_BINARY_DIR}/generated/AuroraBuiltinModules.cpp")

add_library(aurora_builtin_modules STATIC "${CMAKE_BINARY_DIR}/generated/AuroraBuiltinModules.cpp")
target_link_libraries(aurora_builtin_modules PUBLIC aurora_runtime_api)

foreach(target aurora_module_empty aurora_module_simulated aurora_module_faultinjector)
    if(TARGET ${target})
        target_link_libraries(aurora_builtin_modules PUBLIC ${target})
    endif()
endforeach()
