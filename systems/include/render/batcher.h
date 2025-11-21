#pragma once

#include "render/color.h"
#include "render/shader.h"
#include "render/texture.h"

#include <cassert>
#include <cstddef>

class RenderBatcher
{
public:
    Matrix3x3 transform;

    void init();
    ~RenderBatcher();

    void renderSprite(
        const Vec2& pos,
        const Vec2& scale,
        const Texture& texture,
        const Color& tint
    );

    void beginBatch();
    void flushBatch();

private:
    static constexpr size_t MAX_BATCH_VERTICES = 1024;
    static constexpr size_t VERTEX_BUFFER_SIZE = MAX_BATCH_VERTICES * 4;
    static constexpr size_t MAX_INDEX_SIZE = MAX_BATCH_VERTICES * 6;

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
    };

    Shader m_spriteShader;

    bool m_initialized = false;

    GLuint m_quadVAO;
    GLuint m_quadVBO;
    GLuint m_quadEBO;

    Vertex m_vertices[VERTEX_BUFFER_SIZE];

    size_t m_batchIndex = 0;
    size_t m_indicesToRender = 0;

    Texture m_lastTexture;

    void addSprite(
        const Vec2& pos,
        const Vec2& scale,
        const Color& tint
    );
};
