# SPDX-FileCopyrightText: 2026 Project Aurora contributors
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(aurora_runtime_api STATIC
    AuroraRuntimeAPI/src/aurora_runtime_api.c
)
target_include_directories(aurora_runtime_api PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/AuroraRuntimeAPI/include"
)

add_library(aurora_sdk STATIC
    AuroraModuleSDK/src/json.cpp
    AuroraModuleSDK/src/version.cpp
    AuroraModuleSDK/src/manifest.cpp
    AuroraModuleSDK/src/state_machine.cpp
    AuroraModuleSDK/src/registry.cpp
    AuroraModuleSDK/src/host.cpp
    AuroraModuleSDK/src/supervisor.cpp
    AuroraStorageAPI/src/storage.cpp
    AuroraDiagnosticsAPI/src/diagnostics.cpp
    AuroraLibrary/src/library.cpp
)
target_include_directories(aurora_sdk PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/AuroraModuleSDK/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/AuroraRuntimeAPI/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/AuroraStorageAPI/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/AuroraDiagnosticsAPI/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/AuroraLibrary/include"
)
target_link_libraries(aurora_sdk PUBLIC aurora_runtime_api)
