set(
    INPUT_SRC
    inputManager.cpp
)

add_library(input STATIC ${INPUT_SRC})
set_target_properties(
    input
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
target_link_libraries(input math)

