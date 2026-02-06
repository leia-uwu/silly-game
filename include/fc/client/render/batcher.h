/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/color.h"
#include "fc/client/render/shader.h"
#include "fc/client/render/texture.h"
#include "fc/core/math/matrix.h"
#include "fc/core/math/vec2.h"

#include <cassert>
#include <cstddef>

class RenderBatcher
{
public:
    Matrix3x3 transform;

    void init();
    ~RenderBatcher();

    struct Vertex
    {
        Vec2F pos;
        Vec2F textureCord;
        uint32_t color;
    };

    class Batchable
    {
    public:
        Texture texture;

        Batchable(const Texture& texture);
        [[nodiscard]] virtual size_t batchSize() const = 0;
        [[nodiscard]] virtual size_t indices() const = 0;

    private:
        virtual void addToBatcher(RenderBatcher& batcher) const = 0;
        friend class RenderBatcher;
    };

    class TextureBatchable : public Batchable
    {
    public:
        Color tint;

        Matrix3x3 transform;

        Vec2U frameOffset;

        TextureBatchable(
            const Texture& texture,
            const Color& tint,
            const Matrix3x3& transform,
            const Vec2U& frameOffset
        ) : Batchable(texture),
            tint(tint),
            transform(transform),
            frameOffset(frameOffset) { };

        [[nodiscard]] size_t batchSize() const override;
        [[nodiscard]] size_t indices() const override;

    private:
        void addToBatcher(RenderBatcher& batcher) const override;
    };

    void addVertice(const Vertex& vert);
    void addIndice(uint32_t i);

    [[nodiscard]] uint32_t indiceOffset() const
    {
        return m_indicesOffset;
    }

    void incrementIndiceOffset(uint32_t i)
    {
        m_indicesOffset += i;
    }

    void addBatchable(const Batchable& batchable);

    void beginBatch();
    void flushBatch();

private:
    static constexpr size_t MAX_BATCH_VERTICES = 4096;
    static constexpr size_t MAX_INDEX_SIZE = MAX_BATCH_VERTICES * 6;

    Shader m_spriteShader;

    GLuint m_whiteTexture;

    bool m_initialized = false;

    GLuint m_quadVAO;
    GLuint m_quadVBO;
    GLuint m_quadEBO;

    Vertex m_vertices[MAX_BATCH_VERTICES];
    GLuint m_indices[MAX_INDEX_SIZE];

    size_t m_batchIndex = 0;
    size_t m_indicesIndex = 0;
    size_t m_indicesOffset = 0;

    Texture m_lastTexture;

    void addSprite(
        const Vec2F& pos,
        const Vec2F& scale,
        const Color& tint
    );
};
