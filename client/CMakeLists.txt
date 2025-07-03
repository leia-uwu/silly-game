set(
    CLIENT_SRC
    src/main.cpp
    src/game.cpp
    src/inputManager.cpp
    src/renderer.cpp
)

add_executable(game_client ${CLIENT_HEADERS} ${CLIENT_SRC})

target_link_libraries(game_client PRIVATE SDL3::SDL3 common)

target_include_directories(game_client PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../common/")

if (DEFINED EMSCRIPTEN)
    set_target_properties(game_client PROPERTIES LINK_FLAGS "-sENVIRONMENT=web -sMODULARIZE=instance")

    add_custom_command(TARGET game_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/../${CMAKE_BUILD_TYPE}/game_client.wasm"
        "${CMAKE_CURRENT_SOURCE_DIR}/public/"
    )
    add_custom_command(TARGET game_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/../${CMAKE_BUILD_TYPE}/game_client.js"
        "${CMAKE_CURRENT_SOURCE_DIR}/public/"
    )
endif()
