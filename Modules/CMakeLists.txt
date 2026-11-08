# One target per module, driven by modules.list.
#
# Turning a module off is a cache option; leaving it out of modules.list is a
# source change. Both produce the same effect: the generated build table has no
# entry, so the platform reports the module as DISCOVERED instead of pretending.

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/modules.list" AURORA_MODULE_LINES)

foreach(line IN LISTS AURORA_MODULE_LINES)
    string(STRIP "${line}" line)
    if(line STREQUAL "" OR line MATCHES "^#")
        continue()
    endif()
    string(REPLACE "|" ";" parts "${line}")
    list(GET parts 0 module_id)
    list(GET parts 1 entry_symbol)
    list(GET parts 2 module_dir)

    string(TOUPPER "${module_dir}" option_suffix)
    option(AURORA_MODULE_${option_suffix} "Build the ${module_dir} module" ON)
    if(NOT AURORA_MODULE_${option_suffix})
        message(STATUS "module skipped by option: ${module_id}")
        continue()
    endif()

    file(GLOB module_sources "${CMAKE_CURRENT_SOURCE_DIR}/${module_dir}/src/*.cpp")
    if(NOT module_sources)
        message(FATAL_ERROR "module ${module_dir} lists no sources")
    endif()

    string(REPLACE "." "_" target_suffix "${module_id}")
    aurora_add_module(aurora_module_${target_suffix}
        "${module_id}"
        "${entry_symbol}"
        SOURCES ${module_sources})
endforeach()

aurora_emit_builtin_table("${CMAKE_BINARY_DIR}/generated/AuroraBuiltinModules.cpp")

add_library(aurora_builtin_modules STATIC "${CMAKE_BINARY_DIR}/generated/AuroraBuiltinModules.cpp")
target_link_libraries(aurora_builtin_modules PUBLIC aurora_runtime_api)

get_property(module_targets GLOBAL PROPERTY AURORA_MODULE_TARGETS)
foreach(target IN LISTS module_targets)
    target_link_libraries(aurora_builtin_modules PUBLIC ${target})
endforeach()
