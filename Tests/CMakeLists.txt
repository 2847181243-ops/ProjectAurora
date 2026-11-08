# SPDX-FileCopyrightText: 2026 Project Aurora contributors
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(aurora_test_support STATIC
    support/test_harness.cpp
    support/test_module.cpp
)
target_include_directories(aurora_test_support PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/support")
target_link_libraries(aurora_test_support PUBLIC aurora_sdk)

add_executable(aurora_core_tests
    support/test_main.cpp
    src/test_json.cpp
    src/test_manifest.cpp
    src/test_state_machine.cpp
    src/test_storage.cpp
    src/test_registry.cpp
    src/test_supervisor.cpp
    src/test_fault_isolation.cpp
    src/test_library.cpp
    src/test_audio.cpp
)
target_link_libraries(aurora_core_tests PRIVATE aurora_test_support aurora_sdk aurora_builtin_modules)
target_compile_definitions(aurora_core_tests PRIVATE
    AURORA_DEFAULT_MANIFEST_DIR="${PROJECT_SOURCE_DIR}/ModuleManifests")
if(MSVC)
    target_compile_options(aurora_core_tests PRIVATE /W4)
else()
    target_compile_options(aurora_core_tests PRIVATE -Wall -Wextra)
endif()

add_test(NAME aurora_core_tests COMMAND aurora_core_tests)
