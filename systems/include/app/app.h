#include "input/inputManager.h"
#include "render/renderer.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <chrono>
#include <memory>

class GameApp
{
public:
    GameApp(Renderer* renderer) :
        m_renderer(renderer)
    {
        assert(m_renderer.get() != nullptr);
    }
    virtual SDL_AppResult init(int argc, char** argv);

    /**
     * User provided update function
     * dt is the "delta time" / time in seconds since the last update.
     */
    virtual SDL_AppResult update(float dt);

    /**
     * User provided callback for SDL events
     */
    virtual SDL_AppResult SDLEvent(SDL_Event* event);

    virtual ~GameApp() = default;

    Renderer& renderer()
    {
        return *m_renderer;
    }

    InputManager& inputManager()
    {
        return m_inputManager;
    }

    SDL_AppResult processSDLIterate();

    SDL_AppResult processSDLEvent(SDL_Event* event);

protected:
    std::unique_ptr<Renderer> m_renderer;

    InputManager m_inputManager;

    std::chrono::duration<double> m_lastNow = std::chrono::system_clock::now().time_since_epoch();
};

// NOLINTBEGIN(bugprone-macro-parentheses)

/**
 * Initializes the application, APP must be a valid subclass of GameApp
 * This will:
 * - call app->initSystems then app->init, exiting if any of them fail
 * - Set up SDL event handlers (SDL_AppIterate, SDL_AppEvent and SDL_AppQuit)
 */
#define INIT_APP(APP)                                                 \
    SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) \
    {                                                                 \
        auto* renderer = new Renderer();                              \
                                                                      \
        SDL_AppResult result = renderer->init();                      \
        if (result != SDL_APP_CONTINUE) {                             \
            return result;                                            \
        }                                                             \
                                                                      \
        auto* app = new APP(renderer);                                \
        result = app->init(argc, argv);                               \
        if (result != SDL_APP_CONTINUE) {                             \
            return result;                                            \
        }                                                             \
                                                                      \
        *appstate = app;                                              \
        return SDL_APP_CONTINUE;                                      \
    };                                                                \
                                                                      \
    SDL_AppResult SDL_AppIterate(void* appstate)                      \
    {                                                                 \
        auto* app = static_cast<APP*>(appstate);                      \
        return app->processSDLIterate();                              \
    };                                                                \
                                                                      \
    SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)      \
    {                                                                 \
        auto* app = static_cast<APP*>(appstate);                      \
        return app->processSDLEvent(event);                           \
    };                                                                \
                                                                      \
    void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/)        \
    {                                                                 \
        auto* app = static_cast<APP*>(appstate);                      \
        delete app;                                                   \
    };

// NOLINTEND(bugprone-macro-parentheses)
