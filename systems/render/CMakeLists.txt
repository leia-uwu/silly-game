set(
    RENDER_SRC
    renderer.cpp
)

add_library(render STATIC ${RENDER_SRC})
set_target_properties(
    render
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
target_link_libraries(render math)
