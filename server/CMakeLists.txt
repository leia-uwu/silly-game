file(GLOB_RECURSE SERVER_HEADERS src/*.h)
file(GLOB_RECURSE SERVER_SRC src/*.cpp)

add_executable(game_server ${SERVER_HEADERS} ${SERVER_SRC})
target_link_libraries(game_server common)
