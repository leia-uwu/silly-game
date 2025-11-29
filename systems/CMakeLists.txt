#
# Util
#

add_library(
    util
    STATIC
    src/util/formatter.cpp
    src/util/ticker.cpp
)

target_sources(
    util
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/util/formatter.h
        include/util/ticker.h
)

#
# Math
#

add_library(
    math
    STATIC
    src/math/collision.cpp
    src/math/shape.cpp
)

target_sources(
    math
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/math/gmath.h
        include/math/vec2.h
        include/math/matrix.h
        include/math/collision.h
        include/math/grid.h
)

#
# Render
#

add_library(
    render
    STATIC
    src/render/shader.cpp
    src/render/texture.cpp
    src/render/resources.cpp
    src/render/batcher.cpp
    src/render/renderer.cpp
)

target_link_libraries(render SDL3::SDL3)
if (NOT DEFINED EMSCRIPTEN)
    target_link_libraries(render OpenGL::GL)
endif()

target_sources(
    render
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/render/color.h
        include/render/shader.h
        include/render/texture.h
        include/render/resources.h
        include/render/batcher.h
        include/render/renderer.h
)

#
# Input
#

add_library(
    input
    STATIC
    src/input/inputManager.cpp
)
target_link_libraries(input SDL3::SDL3)

target_sources(
    input
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/input/inputManager.h
)

#
# App
#

add_library(
    app
    STATIC
    src/app/app.cpp
)
target_link_libraries(app SDL3::SDL3)

target_sources(
    app
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/app/app.h
)

#
# NET
#

add_library(
    net
    STATIC
    src/net/bytestream.cpp
)

target_sources(
    net
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/net/bytestream.h
)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
