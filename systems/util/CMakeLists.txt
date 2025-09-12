set(
    UTIL_SRC
    formatter.cpp
)

add_library(util STATIC ${UTIL_SRC})
set_target_properties(
    util
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
