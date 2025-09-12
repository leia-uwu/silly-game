#pragma once

#include "systems/math/vec2.h"
#include "systems/render/color.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_render.h>
#include <iostream>
#include <vector>

class Renderer
{
private:
    SDL_Renderer* m_renderer;

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

    SDL_Texture* loadTexture(const char* path)
    {
        std::string filePath = SDL_GetBasePath();
        filePath += "../";
        filePath += path;

        std::cout << "Loading " << filePath << "\n";

        SDL_Surface* surf = SDL_LoadBMP(filePath.c_str());
        if (surf == nullptr) {
            std::cerr << "Couldn't load bitmap: " << SDL_GetError() << "\n";
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
        if (texture == nullptr) {
            std::cerr << "Couldn't create texture: " << SDL_GetError() << "\n";
        }
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
        SDL_DestroySurface(surf);

        return texture;
    }
};
