#include "app/app.h"

SDL_AppResult GameApp::init(int /*argc*/, char** /*argv*/)
{
    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::update(float /*dt*/)
{
    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::SDLEvent(SDL_Event* /*event*/)
{
    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::processSDLIterate()
{
    uint64_t newNow = SDL_GetPerformanceCounter();
    float dt = ((newNow - m_lastNow) / (float)SDL_GetPerformanceFrequency());
    m_lastNow = newNow;

    renderer().clear();

    SDL_AppResult result = update(dt);

    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    inputManager().flush();
    renderer().present();

    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::processSDLEvent(SDL_Event* event)
{
    inputManager().processSDLEvent(event);
    renderer().processSDLEvent(event);

    SDL_AppResult result = SDLEvent(event);

    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
};
