#include "renderer.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <cmath>

Renderer::Renderer(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
}

void Renderer::drawCircle(const Vec2& position, const float radius)
{
    drawEllipse(position, radius, radius);
}

// https://stackoverflow.com/a/47766167
void Renderer::drawEllipse(const Vec2& position, const float radiusX, const float radiusY)
{
    const float rx = radiusX;
    const float ry = radiusY;
    // drew  28 lines with   4x4  circle with precision of 150 0ms
    // drew 132 lines with  25x14 circle with precision of 150 0ms
    // drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 270; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0; // angle that will be increased each loop

    // starting point
    int x = (float)rx * std::cos(theta); // start point
    int y = (float)ry * std::sin(theta); // start point
    int x1 = x;
    int y1 = y;

    // repeat until theta >= 90;
    float step = M_PI_2 / (float)prec; // amount to add to theta each time (degrees)
    for (theta = step; theta <= M_PI_2; theta += step) // step through only a 90 arc (1 quadrant)
    {
        // get new point location
        x1 = std::round((rx * std::cos(theta)) + 0.5); // new point (+.5 is a quick rounding method)
        y1 = std::round((ry * std::sin(theta)) + 0.5); // new point (+.5 is a quick rounding method)

        // draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1)) // only draw if coordinate changed
        {
            SDL_RenderLine(m_renderer, position.x + x, position.y - y, position.x + x1, position.y - y1); // quadrant TR
            SDL_RenderLine(m_renderer, position.x - x, position.y - y, position.x - x1, position.y - y1); // quadrant TL
            SDL_RenderLine(m_renderer, position.x - x, position.y + y, position.x - x1, position.y + y1); // quadrant BL
            SDL_RenderLine(m_renderer, position.x + x, position.y + y, position.x + x1, position.y + y1); // quadrant BR
        }
        // save previous points
        x = x1; // save new previous point
        y = y1; // save new previous point
    }
    // arc did not finish because of rounding, so finish the arc
    if (x != 0) {
        x = 0;
        SDL_RenderLine(m_renderer, position.x + x, position.y - y, position.x + x1, position.y - y1); // quadrant TR
        SDL_RenderLine(m_renderer, position.x - x, position.y - y, position.x - x1, position.y - y1); // quadrant TL
        SDL_RenderLine(m_renderer, position.x - x, position.y + y, position.x - x1, position.y + y1); // quadrant BL
        SDL_RenderLine(m_renderer, position.x + x, position.y + y, position.x + x1, position.y + y1); // quadrant BR
    }
}

void Renderer::drawRect(const Vec2& position, const float width, const float height)
{
    const float w = width;
    const float h = height;
    SDL_FRect rect = {
        position.x - w / 2,
        position.y - h / 2,
        w,
        h
    };
    SDL_RenderRect(m_renderer, &rect);
}

void Renderer::drawPoly(const std::vector<Vec2>& points)
{
    size_t size = points.size();
    for (size_t i = 0; i < size; i++) {
        Vec2 a = points[i];
        Vec2 b = points[(i + 1) % size];

        SDL_RenderLine(m_renderer, a.x, a.y, b.x, b.y);
    }
}
