find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)
if (NOT SDL3_FOUND)
    FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        c9a6709bd21750f1ad9597be21abace78c6378c9 # release-3.2.16
    )
    FetchContent_MakeAvailable(SDL)
endif()

set(
    CLIENT_SRC
    src/main.cpp
    src/game.cpp
    src/inputManager.cpp
    src/renderer.cpp
)

add_executable(game_client ${CLIENT_HEADERS} ${CLIENT_SRC})

target_link_libraries(
    game_client
    PRIVATE SDL3::SDL3 common
)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        game_client
        PROPERTIES LINK_FLAGS "-sENVIRONMENT=web -sMODULARIZE=instance"
    )
    # output the client wasm and js bundles into a consistent directory vite can find
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/client/wasm/$<CONFIGURATION>")
    set(CMAKE_EXECUTABLE_SUFFIX ".js" CACHE INTERNAL "")
endif()
