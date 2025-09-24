#pragma once

#include "systems/math/vec2.h"

#include "color.h"
#include "shader.h"
#include "texture.h"

#include <GL/gl.h>
#include <GLES3/gl32.h>

#include <cassert>
#include <cstddef>

inline constexpr size_t MAX_BATCH_VERTICES = 1024;
inline constexpr size_t VERTEX_BUFFER_SIZE = MAX_BATCH_VERTICES * 4;
inline constexpr size_t MAX_INDEX_SIZE = MAX_BATCH_VERTICES * 6;

class SpriteBatcher
{
private:
    struct Vec3
    {
        float x;
        float y;
        float z;
    };
    struct Vec4
    {
        float x;
        float y;
        float z;
        float w;
    };

    struct Vertex
    {
        Vec2 pos;
        Vec2 textureCord;
        Vec4 color;
        GLuint textureID;
    };

    Shader m_spriteShader;

    bool m_initialized = false;

    GLuint m_quadVAO;
    GLuint m_quadVBO;
    GLuint m_quadEBO;

    Vertex m_vertices[VERTEX_BUFFER_SIZE];

    size_t m_batchIndex = 0;
    size_t m_indicesToRender = 0;

public:
    Matrix3x3 transform;

    void init();
    ~SpriteBatcher();

    void renderSprite(
        const Vec2& pos,
        const Vec2& scale,
        const Texture& texture,
        const Color& tint
    );

    void addSprite(
        const Vec2& pos,
        const Vec2& scale,
        const Texture& texture,
        const Color& tint
    );

    void beginBatch();
    void flushBatch(const Texture& texture);
};
