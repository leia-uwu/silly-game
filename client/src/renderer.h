#include "common/src/vec2.h"
#include <SDL3/SDL_render.h>

class Renderer
{
private:
    SDL_Renderer* m_renderer;

    Vec2 m_render_position = {0, 0};
    Vec2 m_position = {0, 0};
    float m_scale = 1;

    float m_width = 0;
    float m_height = 0;

    void update_transform();

public:
    Renderer(SDL_Renderer* renderer);

    void set_position(const Vec2& position);
    void set_size(const float width, const float height);
    float get_scale();
    void set_scale(const float scale);

    void draw_circle(const Vec2& position, const float radius);
    void draw_ellipse(const Vec2& position, const float radiusX, const float radiusY);

    void draw_rect(const Vec2& position, const float width, const float height);
};
