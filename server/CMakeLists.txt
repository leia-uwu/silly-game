pkg_check_modules(libuv QUIET IMPORTED_TARGET libuv)

if (libuv_FOUND)
    add_library(libuv ALIAS PkgConfig::libuv)
else()
    FetchContent_Declare(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv
        GIT_TAG        v1.51.0
        GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(libuv)
    add_library(libuv ALIAS uv_a)
endif()

find_package(ZLIB QUIET)
if (NOT ZLIB_FOUND)
    FetchContent_Declare(
        zlib
        GIT_REPOSITORY "https://github.com/madler/zlib.git"
        GIT_TAG v1.3.1
        GIT_SHALLOW ON
        GIT_SUBMODULES ""
    )
    FetchContent_MakeAvailable(zlib)
endif()

# thanks to https://stackoverflow.com/a/75848101
FetchContent_Declare(
    uSockets
    GIT_REPOSITORY https://github.com/uNetworking/uSockets
    GIT_TAG v0.8.8
    GIT_SHALLOW ON
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(uSockets)
file(GLOB_RECURSE SOURCES ${usockets_SOURCE_DIR}/src/*.c)
add_library(uSockets ${SOURCES})

target_include_directories(uSockets PUBLIC ${usockets_SOURCE_DIR}/src)
target_compile_definitions(uSockets PRIVATE LIBUS_NO_SSL LIBUS_USE_LIBUV)

FetchContent_Declare(
    uWebSockets
    GIT_REPOSITORY https://github.com/uNetworking/uWebSockets
    GIT_TAG v20.74.0
    GIT_SHALLOW ON
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(uWebSockets)

add_library(uWebSockets INTERFACE)
target_include_directories(uWebSockets INTERFACE ${uwebsockets_SOURCE_DIR}/src/)
target_link_libraries(uWebSockets INTERFACE uSockets ${ZLIB_LIBRARIES} libuv)

set(
    SERVER_SRC
    src/main.cpp
    src/game.cpp
)

add_executable(game_server ${SERVER_SRC})

target_link_libraries(
    game_server
    PRIVATE libuv uWebSockets
    util math net
)
