#include "render/batcher.h"

#include <cstdint>
#include <glad/gl.h>

static const char* FRAGMENT_SHADER = R"(#version 300 es
precision highp float;
layout (location = 0) out vec4 o_color;
in vec4 v_color;
in vec2 v_textureCord;
uniform sampler2D u_texture;
void main()
{
    o_color = texture(u_texture, v_textureCord) * v_color;
}
)";

static const char* VERTEX_SHADER = R"(#version 300 es
precision highp float;
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_textureCord;
layout (location = 2) in uint a_color;
out vec4 v_color;
out vec2 v_textureCord;
uniform mat3 u_transform;
void main()
{
    float r = float((a_color >> 24) & 255u) / 255.0;
    float g = float((a_color >> 16) & 255u) / 255.0;
    float b = float((a_color >> 8 ) & 255u) / 255.0;
    float a = float((a_color      ) & 255u) / 255.0;
    v_color = vec4(r, g, b, a);
    v_textureCord = a_textureCord;
    gl_Position = mat4(u_transform) * vec4(a_pos.x, a_pos.y, 0.0, 1.0);
}
)";

void RenderBatcher::init()
{
    assert(m_initialized == false);
    m_initialized = true;

    m_spriteShader.compile(VERTEX_SHADER, FRAGMENT_SHADER, nullptr);

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &m_quadEBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), nullptr, GL_DYNAMIC_DRAW);

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

    glVertexAttribIPointer(
        attribIndex,
        1,
        GL_UNSIGNED_INT,
        sizeof(Vertex), /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
        (const void*)offsetof(Vertex, color)
    );
    glEnableVertexAttribArray(attribIndex);
    attribIndex++;
}

RenderBatcher::~RenderBatcher()
{
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteBuffers(1, &m_quadEBO);
}

void RenderBatcher::beginBatch()
{
    m_batchIndex = 0;
    m_indicesIndex = 0;
    m_indicesOffset = 0;
}

void RenderBatcher::flushBatch()
{
    if (m_batchIndex == 0 || m_indicesIndex == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_batchIndex * sizeof(Vertex), m_vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadEBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indicesIndex * sizeof(GLuint), m_indices);

    // FIXME: more than 1 texture lol
    glActiveTexture(GL_TEXTURE0);
    m_lastTexture.bind();

    m_spriteShader.use();
    m_spriteShader.setInt("u_texture", 0);
    m_spriteShader.setMatrix3("u_transform", transform);

    glBindVertexArray(m_quadVAO);
    glDrawElements(GL_TRIANGLES, m_indicesIndex, GL_UNSIGNED_INT, nullptr);
}

void RenderBatcher::addVertice(const Vertex& vert)
{
    assert((m_batchIndex + 1) < MAX_BATCH_VERTICES);

    m_vertices[m_batchIndex] = vert;
    m_batchIndex++;
}

void RenderBatcher::addIndice(uint32_t i)
{
    assert((m_indicesIndex + 1) < MAX_INDEX_SIZE);

    m_indices[m_indicesIndex] = i;
    m_indicesIndex++;
}

void RenderBatcher::addBatchable(const Batchable& batchable)
{
    size_t oldBatchIndex = m_batchIndex;
    size_t oldIndiceIndex = m_indicesIndex;

    if (m_batchIndex + batchable.batchSize() >= MAX_BATCH_VERTICES || m_lastTexture.id != batchable.texture.id) {
        flushBatch();
        beginBatch();
        oldBatchIndex = 0;
        oldIndiceIndex = 0;
        m_lastTexture = batchable.texture;
    }

    batchable.addToBatcher(*this);

    // to make sure addToBatcher and batchSize are in sync
    assert(m_batchIndex == (oldBatchIndex + batchable.batchSize()));
    assert(m_indicesIndex == (oldIndiceIndex + batchable.indices()));
}

RenderBatcher::Batchable::Batchable(const Texture& texture) : texture(texture)
{
}

RenderBatcher::TextureBatchable::TextureBatchable(const Vec2F& pos, const Vec2F& scale, const Texture& texture, const Color& tint, float rotation) :
    Batchable(texture),
    pos(pos),
    scale(scale),
    tint(tint),
    rotation(rotation)
{
}

[[nodiscard]] size_t RenderBatcher::TextureBatchable::batchSize() const
{
    return 4;
}

[[nodiscard]] size_t RenderBatcher::TextureBatchable::indices() const
{
    return 6;
}

void RenderBatcher::TextureBatchable::addToBatcher(RenderBatcher& batcher) const
{
    uint32_t color = tint.RGBAHex();

    batcher.addVertice({
        .pos = {pos.x, pos.y},
        .textureCord = {0.F, 0.F},
        .color = color,
    });

    batcher.addVertice({
        .pos = {pos.x + scale.x, pos.y},
        .textureCord = {1.F, 0.F},
        .color = color,
    });

    batcher.addVertice({
        .pos = {pos.x + scale.x, pos.y + scale.y},
        .textureCord = {1.F, 1.F},
        .color = color,
    });

    batcher.addVertice({
        .pos = {pos.x, pos.y + scale.y},
        .textureCord = {0.F, 1.F},
        .color = color,
    });

    size_t offset = batcher.indiceOffset();

    batcher.addIndice(0 + offset);
    batcher.addIndice(1 + offset);
    batcher.addIndice(2 + offset);

    batcher.addIndice(2 + offset);
    batcher.addIndice(3 + offset);
    batcher.addIndice(0 + offset);

    batcher.incrementIndiceOffset(4);
}
