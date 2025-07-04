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
# so it doesn't redownload all the deps if the build directory is nuked
set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_deps")

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(common)
add_subdirectory(client)

if (NOT DEFINED EMSCRIPTEN)
    add_subdirectory(server)
endif()

