#include "app/app.h"
#include <SDL3/SDL_init.h>

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
    std::chrono::duration<double> newNow = std::chrono::system_clock::now().time_since_epoch();
    float dt = std::chrono::duration<double, std::ratio<1>>(newNow - m_lastNow).count();
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
