file(GLOB_RECURSE COMMON_HEADERS src/*.h)
file(GLOB_RECURSE COMMON_SRC src/*.cpp)

add_library(common STATIC ${COMMON_SRC} ${COMMON_HEADERS})
target_link_libraries(common PRIVATE SDL3::SDL3)
