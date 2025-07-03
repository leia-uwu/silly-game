set(
    COMMON_SRC
    src/vec2.cpp
    src/bytestream.cpp
    src/collision.cpp
    src/chalk.cpp
)

add_library(common STATIC ${COMMON_SRC})
set_target_properties(common PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
