cmake_minimum_required(VERSION 3.15)

project(silly-game CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(SDL3 CONFIG COMPONENTS SDL3-shared)
if (NOT SDL3_FOUND)
    add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
endif()

if (DEFINED EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()

add_subdirectory(common)
add_subdirectory(client)
if (NOT DEFINED EMSCRIPTEN)
    add_subdirectory(server)
endif()
