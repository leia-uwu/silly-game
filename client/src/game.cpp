#include <SDL3/SDL.h>
#include <chrono>

#include "game.h"

Game::Game(SDL_Window* window, SDL_Renderer* renderer)
    : m_window(window)
    , m_renderer(renderer)
{
}

int roundUpToMultipleOfEight(const int v)
{
    return (v + (8 - 1)) & -8;
}

void DrawCircle(SDL_Renderer* renderer, const Vec2& center, const float radius)
{
    const int arrSize = roundUpToMultipleOfEight(radius * 8 * M_SQRT1_2);
    Vec2 points[arrSize];
    int drawCount = 0;

    const float diameter = (radius * 2);

    float x = (radius - 1);
    float y = 0;
    float tx = 1;
    float ty = 1;
    float error = (tx - diameter);

    while (x >= y) {
        // Each of the following renders an octant of the circle
        points[drawCount + 0] = {center.x + x, center.y - y};
        points[drawCount + 1] = {center.x + x, center.y + y};
        points[drawCount + 2] = {center.x - x, center.y - y};
        points[drawCount + 3] = {center.x - x, center.y + y};
        points[drawCount + 4] = {center.x + y, center.y - x};
        points[drawCount + 5] = {center.x + y, center.y + x};
        points[drawCount + 7] = {center.x - y, center.y + x};
        points[drawCount + 6] = {center.x - y, center.y - x};

        drawCount += 8;

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    SDL_RenderPoints(renderer, points, drawCount);
}

SDL_AppResult Game::update()
{
    std::chrono::duration<double> newNow = std::chrono::system_clock::now().time_since_epoch();
    double dt = std::chrono::duration<double, std::ratio<1>>(newNow - m_lastNow).count();
    m_lastNow = newNow;

    SDL_SetRenderDrawColor(m_renderer, 90, 137, 57, 255);
    SDL_RenderClear(m_renderer);

    SDL_SetRenderDrawColor(m_renderer, 255, 170, 127, 255);
    DrawCircle(m_renderer, m_inputManager.get_mouse_pos(), 40);

    SDL_RenderPresent(m_renderer);

    return SDL_APP_CONTINUE;
};

SDL_AppResult Game::process_event(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        m_inputManager.on_key_down(SDL_GetKeyName(event->key.key));
        break;
    case SDL_EVENT_KEY_UP:
        m_inputManager.on_key_up(SDL_GetKeyName(event->key.key));
        break;
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_FINGER_MOTION:
        m_inputManager.on_motion_event(event->motion);
        break;
    }

    return SDL_APP_CONTINUE;
}

void Game::shutdown()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}
