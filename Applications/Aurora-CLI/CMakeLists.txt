# SPDX-FileCopyrightText: 2026 Project Aurora contributors
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(aurora-cli main.cpp)
target_link_libraries(aurora-cli PRIVATE aurora_sdk aurora_builtin_modules)
target_compile_definitions(aurora-cli PRIVATE
    AURORA_DEFAULT_MANIFEST_DIR="${PROJECT_SOURCE_DIR}/ModuleManifests")
if(MSVC)
    target_compile_options(aurora-cli PRIVATE /W4)
else()
    target_compile_options(aurora-cli PRIVATE -Wall -Wextra)
endif()
