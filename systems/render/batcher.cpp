#include "batcher.h"

#include <glad/gl.h>

#ifndef __EMSCRIPTEN__
static const char* FRAGMENT_SHADER =
    "#version 330\n"
    "layout (location = 0) out vec4 o_color;\n"
    "in vec4 v_color;\n"
    "in vec2 v_textureCord;\n"
    "flat in uint v_textureID;\n"
    "uniform sampler2D u_texture;\n"
    "void main()\n"
    "{\n"
    "    o_color = texture(u_texture, v_textureCord) * v_color;\n"
    "}\n";

static const char* VERTEX_SHADER =
    "#version 330\n"
    "layout (location = 0) in vec2 a_pos;\n"
    "layout (location = 1) in vec2 a_textureCord;\n"
    "layout (location = 2) in vec4 a_color;\n"
    "layout (location = 3) in uint a_textureID;\n"
    "out vec4 v_color;\n"
    "out vec2 v_textureCord;\n"
    "flat out uint v_textureID;\n"
    "uniform mat3 u_transform;\n"
    "void main()\n"
    "{\n"
    "    v_color = a_color;\n"
    "    v_textureCord = a_textureCord;\n"
    "    v_textureID = a_textureID;\n"
    "    gl_Position = mat4(u_transform) * vec4(a_pos.x, a_pos.y, 0.0, 1.0);\n"
    "}\n";
// keeping those in sync will surely be fun...
#else
static const char* FRAGMENT_SHADER =
    "precision highp float;\n"
    "varying vec4 v_color;\n"
    "varying vec2 v_textureCord;\n"
    "varying float v_textureID;\n"
    "uniform sampler2D u_texture;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D(u_texture, v_textureCord) * v_color;\n"
    "}\n";

static const char* VERTEX_SHADER =
    "precision highp float;\n"
    "attribute vec2 a_pos;\n"
    "attribute vec2 a_textureCord;\n"
    "attribute vec4 a_color;\n"
    "attribute float a_textureID;\n"
    "varying vec4 v_color;\n"
    "varying vec2 v_textureCord;\n"
    "varying float v_textureID;\n"
    "uniform mat3 u_transform;\n"
    "void main()\n"
    "{\n"
    "    v_color = a_color;\n"
    "    v_textureCord = a_textureCord;\n"
    "    v_textureID = a_textureID;\n"
    "    gl_Position = mat4(u_transform) * vec4(a_pos.x, a_pos.y, 0.0, 1.0);\n"
    "}\n";
#endif

void RenderBatcher::init()
{
    assert(m_initialized == false);
    m_initialized = true;

    m_spriteShader.compile(VERTEX_SHADER, FRAGMENT_SHADER, nullptr);

    uint32_t indices[MAX_INDEX_SIZE];
    uint32_t offset = 0;
    for (size_t i = 0; i < MAX_INDEX_SIZE; i += 6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &m_quadEBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    size_t attribIndex = 0;
#define DEFINE_VERTEX_ATTRIB(size, type, key)                           \
    glVertexAttribPointer(                                              \
        attribIndex,                                                    \
        size,                                                           \
        type,                                                           \
        GL_FALSE,                                                       \
        sizeof(Vertex), /* NOLINTNEXTLINE(performance-no-int-to-ptr) */ \
        (const void*)offsetof(Vertex, key)                              \
    );                                                                  \
    glEnableVertexAttribArray(attribIndex);                             \
    attribIndex++;

    DEFINE_VERTEX_ATTRIB(2, GL_FLOAT, pos)

    DEFINE_VERTEX_ATTRIB(2, GL_FLOAT, textureCord)

    DEFINE_VERTEX_ATTRIB(4, GL_FLOAT, color)

    DEFINE_VERTEX_ATTRIB(1, GL_UNSIGNED_INT, textureID)
}

RenderBatcher::~RenderBatcher()
{
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteBuffers(1, &m_quadEBO);
}

void RenderBatcher::renderSprite(
    const Vec2& pos,
    const Vec2& scale,
    const Texture& texture,
    const Color& tint
)
{
    if (m_batchIndex + 4 >= VERTEX_BUFFER_SIZE || m_lastTexture.id != texture.id) {
        flushBatch();
        beginBatch();
        m_lastTexture = texture;
    }

    addSprite(pos, scale, texture, tint);
}

void RenderBatcher::addSprite(
    const Vec2& pos,
    const Vec2& scale,
    const Texture& texture,
    const Color& tint
    // float rotation
)
{
    Vec4 color = {
        .x = tint.normalizedR(),
        .y = tint.normalizedG(),
        .z = tint.normalizedB(),
        .w = tint.normalizedA()
    };

    m_vertices[m_batchIndex] = {
        .pos = {pos.x, pos.y},
        .textureCord = {0.F, 0.F},
        .color = color,
        .textureID = texture.id,
    };

    m_vertices[m_batchIndex + 1] = {
        .pos = {pos.x + scale.x, pos.y},
        .textureCord = {1.F, 0.F},
        .color = color,
        .textureID = texture.id,
    };

    m_vertices[m_batchIndex + 2] = {
        .pos = {pos.x + scale.x, pos.y + scale.y},
        .textureCord = {1.F, 1.F},
        .color = color,
        .textureID = texture.id,
    };

    m_vertices[m_batchIndex + 3] = {
        .pos = {pos.x, pos.y + scale.y},
        .textureCord = {0.F, 1.F},
        .color = color,
        .textureID = texture.id,
    };

    m_batchIndex += 4;
    m_indicesToRender += 6;
}

void RenderBatcher::beginBatch()
{
    m_batchIndex = 0;
    m_indicesToRender = 0;
}

void RenderBatcher::flushBatch()
{
    if (m_batchIndex == 0 || m_indicesToRender == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_batchIndex * sizeof(Vertex), m_vertices);

    // FIXME: more than 1 texture lol
    m_lastTexture.bind();
    glActiveTexture(GL_TEXTURE0);

    m_spriteShader.use();
    m_spriteShader.setInt("u_texture", 0);
    m_spriteShader.setMatrix3("u_transform", transform);

    glBindVertexArray(m_quadVAO);
    glDrawElements(GL_TRIANGLES, m_indicesToRender, GL_UNSIGNED_INT, nullptr);
}
