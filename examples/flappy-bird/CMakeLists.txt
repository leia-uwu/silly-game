set(
    FLAPPY_SRC
    src/main.cpp
    src/game.cpp
)

if (DEFINED EMSCRIPTEN)
    # output the client wasm and js bundles into a consistent directory vite can find
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/wasm/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/wasm/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/wasm/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/wasm/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/wasm/")

    set(CMAKE_EXECUTABLE_SUFFIX ".js" CACHE INTERNAL "")
endif()

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
