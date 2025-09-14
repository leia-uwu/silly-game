#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <SDL3/SDL.h>
#include <iostream>

#include "game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_JS(int, getWindowWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getWindowHeight, (), {
    return window.innerHeight;
});

static bool resizeCanvas(int eventType, const EmscriptenUiEvent* event, void* userData)
{
    auto window = static_cast<SDL_Window*>(userData);

    int width = getWindowWidth();
    int height = getWindowHeight();

    if (!SDL_SetWindowSize(window, width, height)) {
        std::cout << "SDL_SetWindowSize error: " << SDL_GetError() << "\n";
    }
    return 0;
};
#endif

SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*argv*/)
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "SDL_InitSubSystem error: " << SDL_GetError() << "\n";
    }

    SDL_Window* window = SDL_CreateWindow(
        "Game",
        800,
        450,
        SDL_WINDOW_RESIZABLE
    );

    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 0, resizeCanvas);
    resizeCanvas(0, nullptr, window);
#endif

    SDL_SetRenderVSync(renderer, 1);

    *appstate = new Game(window, renderer);

    return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto* game = static_cast<Game*>(appstate);
    return game->update();
};

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    auto* game = static_cast<Game*>(appstate);
    return game->processEvent(event);
};

void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/)
{
    auto* game = static_cast<Game*>(appstate);
    game->shutdown();
};
