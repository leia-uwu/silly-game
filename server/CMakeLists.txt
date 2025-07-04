pkg_check_modules(LIBUV QUIET IMPORTED_TARGET libuv)
if (LIBUV_FOUND)
    add_library(LIBUV ALIAS PkgConfig::LIBUV)
else()
    FetchContent_Declare(
        LIBUV
        GIT_REPOSITORY https://github.com/libuv/libuv
        GIT_TAG        5152db2cbfeb5582e9c27c5ea1dba2cd9e10759b # v1.51.0
        FIND_PACKAGE_ARGS NAMES LIBUV
    )
    FetchContent_MakeAvailable(LIBUV)
    add_library(LIBUV ALIAS uv)
endif()

set(
    SERVER_SRC
    src/main.cpp
    src/game.cpp
)

add_executable(
    game_server
    ${SERVER_SRC}
)

target_link_libraries(
    game_server
    PRIVATE common LIBUV
)
