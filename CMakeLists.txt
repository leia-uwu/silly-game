cmake_minimum_required(VERSION 3.24)

project(silly-game CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)

include(FetchContent)
# cmake not printing anything for what could take more than a minute is cringe
# idk why its quiet by default...
set(FETCHCONTENT_QUIET OFF)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)

if (NOT SDL3_FOUND)
    FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        c9a6709bd21750f1ad9597be21abace78c6378c9 # release-3.2.16
    )
    FetchContent_MakeAvailable(SDL)
endif()

if (DEFINED EMSCRIPTEN)
    # output the client wasm and js bundles into a consistent directory vite can find
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/client/wasm/$<CONFIGURATION>")
    set(CMAKE_EXECUTABLE_SUFFIX ".js" CACHE INTERNAL "")
endif()

add_subdirectory(common)
add_subdirectory(client)
if (NOT DEFINED EMSCRIPTEN)
    add_subdirectory(server)
endif()

