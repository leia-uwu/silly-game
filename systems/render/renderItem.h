#pragma once

#include "renderer.h"
#include "systems/math/matrix.h"
#include "systems/math/shape.h"
#include "systems/render/color.h"

#include <SDL3/SDL_render.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <vector>

class RenderItem
{

private:
    std::vector<RenderItem*> m_children;

    RenderItem* m_parent = nullptr;

public:
    Vec2 pos;
    Vec2 scale = {1, 1};
    float rot = 0;

    [[nodiscard]] RenderItem* parent()
    {
        return m_parent;
    }

    [[nodiscard]] Matrix3x3 getMatrix() const
    {
        return {pos, rot, scale};
    }

    RenderItem& addChild(RenderItem* child)
    {
        assert(child != nullptr);

        // don't add it twice :)
        if (child->m_parent == this) {
            return *this;
        }

        if (child->m_parent != nullptr) {
            child->m_parent->removeChild(child);
        }

        child->m_parent = this;
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
        assert(child->m_parent == this);

        m_children.erase(std::ranges::find(m_children, child));
        child->m_parent = nullptr;

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

    virtual void destroy(bool destroyChildren)
    {
        if (destroyChildren) {
            for (auto* child : m_children) {
                child->destroy(true);
            }
            m_children.clear();
        }
        if (m_parent != nullptr) {
            m_parent->removeChild(this);
        }
        delete this;
    }
};

class CircleItem : public RenderItem
{
public:
    float rad;
    Color color;

    void render(const Matrix3x3& transform, Renderer& renderer) const override
    {
        renderer.setColor(color);

        auto mScale = transform.scale();
        renderer.drawEllipse(
            transform.translation(),
            rad * mScale.x,
            rad * mScale.y
        );
    }
};

class RectItem : public RenderItem
{
public:
    float width;
    float height;
    Color color;

    void render(const Matrix3x3& transform, Renderer& renderer) const override
    {
        renderer.setColor(color);

        auto mScale = transform.scale();
        renderer.drawRect(
            transform.translation(),
            width * mScale.x,
            height * mScale.y
        );
    }
};

class SpriteItem : public RenderItem
{
public:
    std::string textureId;

    float width;
    float height;

    Vec2 center = {0.5, 0.5};

    Vec2 framePos;
    // float frameWidth;
    // float frameHeight;

    Color tint = 0xffffff;

    float alpha = 1;

    void setTexture(const std::string& id)
    {
        textureId = id;
    }

    void render(const Matrix3x3& transform, Renderer& renderer) const override
    {
        SDL_Texture* texture = renderer.getTexture(textureId);

        float srcWidth, srcHeight;
        SDL_GetTextureSize(texture, &srcWidth, &srcHeight);

        SDL_FRect srcRect = {
            .x = framePos.x,
            .y = framePos.y,
            .w = srcWidth,
            .h = srcHeight,
        };

        Vec2 scale = transform.scale();

        float dstWidth = width * scale.x;
        float dstHeight = height * scale.y;

        auto t = transform.translation();
        SDL_FRect dstRect = {
            .x = t.x - (dstWidth / 2.F),
            .y = t.y - (dstHeight / 2.F),
            .w = dstWidth,
            .h = dstHeight,
        };

        SDL_FPoint rotCenter = {center.x * dstRect.w, center.y * dstRect.h};

        SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaModFloat(texture, alpha);

        SDL_RenderTextureRotated(
            renderer.renderer(),
            texture,
            &srcRect,
            &dstRect,
            transform.rotation(),
            &rotCenter,
            SDL_FLIP_NONE
        );
    }
};

class ShapeRender : public RenderItem
{
public:
    std::unique_ptr<Shape> shape;
    Color color;

    void render(const Matrix3x3& transform, Renderer& renderer) const override
    {
        renderer.setColor(color);

        auto mScale = transform.scale();
        switch (shape->type) {
        case Shape::CIRCLE: {
            auto circle = static_cast<const Circle&>(*shape);

            renderer.drawEllipse(
                circle.pos + transform.translation(),
                circle.rad * mScale.x,
                circle.rad * mScale.y
            );
            break;
        }
        case Shape::RECT: {
            auto rect = static_cast<const Rect&>(*shape);

            renderer.drawRect(
                rect.center() + transform.translation(),
                rect.width() * mScale.x,
                rect.height() * mScale.y
            );
            break;
        }
        case Shape::POLYGON: {
            auto poly = static_cast<const Polygon&>(*shape);
            // TODO translate and scale
            renderer.drawPoly(poly.points);
            break;
        }
        default: {
            std::unreachable();
        }
        }
    }
};
