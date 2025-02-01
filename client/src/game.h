#pragma once

#include "inputManager.h"
#include "renderer.h"
#include <SDL3/SDL.h>
#include <chrono>

class Game
{

private:
    SDL_Window* m_window;
    SDL_Renderer* m_SDLRenderer;
    std::chrono::duration<double> m_lastNow = std::chrono::system_clock::now().time_since_epoch();

public:
    Game(SDL_Window* window, SDL_Renderer* renderer);

    Renderer m_renderer;

    InputManager m_inputManager;

    SDL_AppResult update();

    SDL_AppResult process_event(SDL_Event* event);

    void shutdown();
};
