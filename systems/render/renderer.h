#pragma once

#include "systems/math/vec2.h"
#include "systems/render/color.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include <vector>

class Renderer
{
private:
    SDL_Renderer* m_renderer;

    std::unordered_map<std::string, SDL_Texture*> m_textures;

public:
    float m_width = 0;
    float m_height = 0;
    Renderer(SDL_Renderer* renderer);

    [[nodiscard]] SDL_Renderer* renderer() const
    {
        return m_renderer;
    }

    void setColor(const Color& color)
    {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    };

    void drawCircle(const Vec2& position, float radius);
    void drawEllipse(const Vec2& position, float radiusX, float radiusY);

    void drawRect(const Vec2& position, float width, float height);
    void drawPoly(const std::vector<Vec2>& points);

    void loadTexture(const std::string& id, const std::string& path);

    SDL_Texture* getTexture(const std::string& id) const
    {
        if (!m_textures.contains(id)) {
            return nullptr;
        }
        return m_textures.at(id);
    }
};
