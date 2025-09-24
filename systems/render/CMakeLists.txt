
if (NOT DEFINED EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
endif()

set(
    RENDER_SRC
    batcher.cpp
    renderer.cpp
    shader.cpp
    texture.cpp
    resources.cpp
)

add_library(render STATIC ${RENDER_SRC})
set_target_properties(
    render
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)

if (NOT DEFINED EMSCRIPTEN)
    target_link_libraries(render math SDL3::SDL3 OpenGL::GL)
else()
    target_link_libraries(render math SDL3::SDL3)
endif()
