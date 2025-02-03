#include "renderer.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <cmath>

Renderer::Renderer(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
}

void Renderer::update_transform()
{
    m_render_position = Vec2::sub(Vec2::mul(m_position, m_scale), {m_width / 2, m_height / 2});
}

void Renderer::set_position(const Vec2& position)
{
    m_position = position;
    update_transform();
}

float Renderer::get_scale() const
{
    return m_scale;
}

void Renderer::set_size(const float width, const float height)
{
    m_width = width;
    m_height = height;
    update_transform();
}

void Renderer::set_scale(const float scale)
{
    m_scale = scale;
    update_transform();
}

void Renderer::draw_circle(const Vec2& position, const float radius)
{
    draw_ellipse(position, radius, radius);
}

// https://stackoverflow.com/a/47766167
void Renderer::draw_ellipse(const Vec2& position, const float radiusX, const float radiusY)
{
    const float rx = radiusX * m_scale;
    const float ry = radiusY * m_scale;
    Vec2 finalPos = Vec2::sub(Vec2::mul(position, m_scale), m_render_position);
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
        x1 = lround((rx * cosf(theta)) + 0.5); // new point (+.5 is a quick rounding method)
        y1 = lround((ry * sinf(theta)) + 0.5); // new point (+.5 is a quick rounding method)

        // draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1)) // only draw if coordinate changed
        {
            SDL_RenderLine(m_renderer, finalPos.x + x, finalPos.y - y, finalPos.x + x1, finalPos.y - y1); // quadrant TR
            SDL_RenderLine(m_renderer, finalPos.x - x, finalPos.y - y, finalPos.x - x1, finalPos.y - y1); // quadrant TL
            SDL_RenderLine(m_renderer, finalPos.x - x, finalPos.y + y, finalPos.x - x1, finalPos.y + y1); // quadrant BL
            SDL_RenderLine(m_renderer, finalPos.x + x, finalPos.y + y, finalPos.x + x1, finalPos.y + y1); // quadrant BR
        }
        // save previous points
        x = x1; // save new previous point
        y = y1; // save new previous point
    }
    // arc did not finish because of rounding, so finish the arc
    if (x != 0) {
        x = 0;
        SDL_RenderLine(m_renderer, finalPos.x + x, finalPos.y - y, finalPos.x + x1, finalPos.y - y1); // quadrant TR
        SDL_RenderLine(m_renderer, finalPos.x - x, finalPos.y - y, finalPos.x - x1, finalPos.y - y1); // quadrant TL
        SDL_RenderLine(m_renderer, finalPos.x - x, finalPos.y + y, finalPos.x - x1, finalPos.y + y1); // quadrant BL
        SDL_RenderLine(m_renderer, finalPos.x + x, finalPos.y + y, finalPos.x + x1, finalPos.y + y1); // quadrant BR
    }
}

void Renderer::draw_rect(const Vec2& position, const float width, const float height)
{
    const float w = width * m_scale;
    const float h = height * m_scale;
    SDL_FRect rect = {
        (position.x * m_scale - m_render_position.x) - w / 2,
        (position.y * m_scale - m_render_position.y) - h / 2,
        w,
        h
    };
    SDL_RenderFillRect(m_renderer, &rect);
}
