#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "systems/render/renderer.h"

#include "game.h"

struct App
{
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Game> game;
};

SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*argv*/)
{
    auto renderer = std::make_unique<Renderer>("Temp Test Game", 900, 450);

    const SDL_AppResult result = renderer->init();
    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    auto game = std::make_unique<Game>(*renderer.get());

    *appstate = new App{
        .renderer = std::move(renderer),
        .game = std::move(game),
    };

    return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto* app = static_cast<App*>(appstate);
    return app->game->update();
};

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    auto* app = static_cast<App*>(appstate);
    return app->game->processEvent(event);
};

void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/)
{
    auto* app = static_cast<App*>(appstate);
    delete app;
};
