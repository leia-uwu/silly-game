set(
    FLAPPY_SRC
    src/main.cpp
    src/game.cpp
)

add_executable(flappy_bird ${FLAPPY_SRC})

target_link_libraries(
    flappy_bird
    PRIVATE SDL3::SDL3
    util math input render app
)

install(TARGETS flappy_bird DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        flappy_bird
        PROPERTIES LINK_FLAGS "-sENVIRONMENT=web -sMODULARIZE=instance -sFULL_ES3"
    )
endif()
