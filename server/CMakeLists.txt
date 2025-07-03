set(
    SERVER_SRC
    src/main.cpp
    src/game.cpp
)

add_executable(game_server ${SERVER_HEADERS} ${SERVER_SRC})
target_link_libraries(game_server common)
