set(
    COMMON_SRC
    src/math/gmath.h
    src/math/vec2.cpp
    src/math/collision.cpp
    src/math/shape.cpp
    src/bytestream.cpp
    src/formatter.cpp
)

add_library(common STATIC ${COMMON_SRC})
set_target_properties(
    common
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
