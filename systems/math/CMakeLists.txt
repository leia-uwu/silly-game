set(
    MATH_SRC
    collision.cpp
    shape.cpp
)

add_library(math STATIC ${MATH_SRC})
set_target_properties(
    math
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
