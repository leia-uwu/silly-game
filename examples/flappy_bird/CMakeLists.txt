set(
    FLAPPY_SRC
    src/main.cpp
    src/game.cpp
)

add_executable(flappy_bird ${CLIENT_HEADERS} ${FLAPPY_SRC})

target_link_libraries(
    flappy_bird
    PRIVATE SDL3::SDL3
    util math render input
)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        flappy_bird
        PROPERTIES LINK_FLAGS "-sENVIRONMENT=web -sMODULARIZE=instance -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2"
    )
endif()
