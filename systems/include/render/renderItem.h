#pragma once

#include "math/vec2.h"
#include "render/batcher.h"
#include "render/color.h"
#include "render/renderer.h"

#include "math/matrix.h"

#include <glad/gl.h>

#include <SDL3/SDL_render.h>

#include <cassert>
#include <vector>

class RenderItem
{

private:
    std::vector<RenderItem*> m_children;

public:
    Vec2F position;
    Vec2F scale = {1, 1};
    float rotation = 0;

    [[nodiscard]] Matrix3x3 getMatrix() const
    {
        return {position, rotation, scale};
    }

    RenderItem& addChild(RenderItem* child)
    {
        assert(child != nullptr);

        m_children.push_back(child);
        return *this;
    };

    RenderItem& addChild(std::initializer_list<RenderItem*>&& children)
    {
        for (auto* child : children) {
            addChild(child);
        }
        return *this;
    }

    RenderItem& removeChild(RenderItem* child)
    {
        for (auto it = m_children.begin(); it != m_children.end(); ++it) {
            if (*it == child) {
                m_children.erase(it);
                break;
            }
        }

        return *this;
    }

    RenderItem& clear()
    {
        m_children.clear();

        return *this;
    }

    void renderChildren(const Matrix3x3& transform, Renderer& renderer) const
    {
        for (const RenderItem* child : m_children) {
            auto childT = Matrix3x3::mul(transform, child->getMatrix());
            child->renderChildren(childT, renderer);
            child->render(childT, renderer);
        }
    }

    virtual void render(const Matrix3x3& transform, Renderer& renderer) const { };

    virtual ~RenderItem() = default;
};

class SpriteItem : public RenderItem
{
public:
    std::string textureId;

    uint32_t width;
    uint32_t height;

    Vec2U frameOffset;

    Vec2F center = {0.5, 0.5};

    Color tint = 0xffffff;

    float alpha = 1;

    void setTexture(const std::string& id)
    {
        textureId = id;
    }

    void render(const Matrix3x3& transform, Renderer& renderer) const override
    {
        const auto& texture = renderer.resources().getTexture(textureId);

        renderer.batcher().addBatchable(RenderBatcher::TextureBatchable{
            texture,
            tint,
            Matrix3x3{transform}.mulScale({width / 2.F, height / 2.F}),
            frameOffset,
        });
    }
};
