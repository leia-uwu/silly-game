file(GLOB_RECURSE CLIENT_HEADERS src/*.h)
file(GLOB_RECURSE CLIENT_SRC src/*.cpp)

add_executable(game_client ${CLIENT_HEADERS} ${CLIENT_SRC})

target_link_libraries(game_client PRIVATE SDL3::SDL3 common)

if (DEFINED EMSCRIPTEN)
    add_custom_command(TARGET game_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/../${CMAKE_BUILD_TYPE}/game_client.wasm"
        "${CMAKE_CURRENT_SOURCE_DIR}/public/game_client.wasm"
    )
endif()
