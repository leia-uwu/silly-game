#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <chrono>

#include "game.h"

Game::Game(SDL_Window* window, SDL_Renderer* renderer)
    : m_window(window)
    , m_SDLRenderer(renderer)
    , m_renderer(renderer)
{
    int w;
    int h;
    SDL_GetWindowSize(m_window, &w, &h);
    m_renderer.set_size(w, h);
}

SDL_AppResult Game::update()
{
    std::chrono::duration<double> newNow = std::chrono::system_clock::now().time_since_epoch();
    double dt = std::chrono::duration<double, std::ratio<1>>(newNow - m_lastNow).count();
    m_lastNow = newNow;

    SDL_SetRenderDrawColor(m_SDLRenderer, 90, 137, 57, 255);
    SDL_RenderClear(m_SDLRenderer);


    static Vec2 player = {0, 0};

    static Vec2 obstacle = {40, 50};


    if (m_inputManager.is_key_down("W")) {
        player.y -= 20 * dt;
    }
    if (m_inputManager.is_key_down("S")) {
        player.y += 20 * dt;
    }
    if (m_inputManager.is_key_down("D")) {
        player.x += 20 * dt;
    }
    if (m_inputManager.is_key_down("A")) {
        player.x -= 20 * dt;
    }

    if (m_inputManager.is_key_down("O")) {
        m_renderer.set_scale(m_renderer.get_scale() + 1 * dt);
    }
    if (m_inputManager.is_key_down("I")) {
        m_renderer.set_scale(m_renderer.get_scale() - 1 * dt);
    }

    m_renderer.set_position(player);

    SDL_SetRenderDrawColor(m_SDLRenderer, 255, 170, 127, 255);
    m_renderer.draw_circle(player, 80);
    m_renderer.draw_rect(obstacle, 20, 20);

    SDL_RenderPresent(m_SDLRenderer);
    return SDL_APP_CONTINUE;
};

SDL_AppResult Game::process_event(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_RESIZED:
        int w;
        int h;
        SDL_GetWindowSize(m_window, &w, &h);
        m_renderer.set_size(w, h);
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
    SDL_DestroyRenderer(m_SDLRenderer);
    SDL_DestroyWindow(m_window);
}
