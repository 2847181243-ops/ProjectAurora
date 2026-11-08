cmake_minimum_required(VERSION 3.20)
project(ProjectAurora VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Zig ships llvm-ar as a subcommand, not as a standalone ar.exe. When CMake is
# driven with zig as the compiler (the Windows development setup), the archive
# rules have to say so. Xcode and CI never take this path.
if(CMAKE_CXX_COMPILER MATCHES "zig")
    set(CMAKE_AR "${CMAKE_CXX_COMPILER}" CACHE FILEPATH "zig provides llvm-ar as a subcommand" FORCE)
    set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> ar rcs <TARGET> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_APPEND   "<CMAKE_AR> ar r <TARGET> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_FINISH   "cd .")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> ar rcs <TARGET> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> ar r <TARGET> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_FINISH "cd .")
    message(STATUS "using zig's built-in archiver")
endif()

option(AURORA_BUILD_TESTS "Build the host test suite" ON)
option(AURORA_BUILD_CLI "Build the headless CLI host" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/BuildSystem")
include(AuroraModules)

add_subdirectory(SDK)
add_subdirectory(Modules)

if(AURORA_BUILD_CLI)
    add_subdirectory(Applications/Aurora-CLI)
endif()

if(AURORA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()
