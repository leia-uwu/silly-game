cmake_minimum_required(VERSION 3.24)

project(silly-game C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BUILD_TESTING)
    include(CTest)
    enable_testing()
endif()

include(FindPkgConfig)
include(FetchContent)
# cmake not printing anything for what could take more than a minute is cringe
# idk why its quiet by default...
set(FETCHCONTENT_QUIET OFF)
# so it doesn't redownload all the deps if the build directory is nuked
set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_deps")

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)
if (NOT SDL3_FOUND)
    FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        release-3.2.16
        GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(SDL)
endif()

add_subdirectory(systems)
add_subdirectory(client)

add_subdirectory(examples)

# if (NOT DEFINED EMSCRIPTEN AND NOT DEFINED WIN32)
    # add_subdirectory(server)
# endif()

