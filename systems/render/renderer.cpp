#include "renderer.h"
#include "systems/render/shader.h"

#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <SDL3/SDL_video.h>

// emscripten helpers
// to make the canvas properly cover the entire web page
// TODO: Make this under a fullscreen flag in Renderer
// Currently we are just always forcing it to be the size of the browser tab window

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

Renderer::Renderer() :
    m_windowTitle("Game"),
    m_windowWidth(800),
    m_windowHeight(400) { };

Renderer::Renderer(std::string title, int width, int height) :
    m_windowTitle(std::move(title)),
    m_windowWidth(width),
    m_windowHeight(height) { };

Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
    SDL_GL_DestroyContext(m_glContext);
}

SDL_AppResult Renderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    };

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(
        "Game",
        800,
        450,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == nullptr) {
        std::cerr << "SDL_GL_CreateContext error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (version == 0) {
        std::cerr << "gladLoadGL error: failed to load OpenGL\n";
        return SDL_APP_FAILURE;
    }
    std::cout << "Using OpenGL " << glGetString(GL_VERSION) << "\n";

#ifdef __EMSCRIPTEN__
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, m_window, 0, resizeCanvas);
    resizeCanvas(0, nullptr, m_window);
#endif

    SDL_GL_SetSwapInterval(-1);

    m_batcher.init();

    return SDL_APP_CONTINUE;
}

void Renderer::processSDLEvent(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED:
        m_windowWidth = event->window.data1;
        m_windowHeight = event->window.data2;

        glViewport(0, 0, m_windowWidth, m_windowHeight);
        m_batcher.transform = Matrix3x3(
            {0, 0},
            0,
            {1.F / (m_windowWidth / 2.F), -(1.F / (m_windowHeight / 2.F))}
        );
        break;
    }
}

SDL_Window* Renderer::window() const
{
    return m_window;
}

bool Renderer::setWindowTitle(const std::string& title)
{
    m_windowTitle = title;
    return SDL_SetWindowTitle(m_window, title.c_str());
}

std::string Renderer::windowTitle() const
{
    return m_windowTitle;
}

int Renderer::windowWidth() const
{
    return m_windowWidth;
}

bool Renderer::setWindowSize(int width, int height)
{
    return SDL_SetWindowSize(m_window, width, height);
}

int Renderer::windowHeight() const
{
    return m_windowHeight;
}

void Renderer::clear()
{
    glClearColor(
        m_clearColor.normalizedR(),
        m_clearColor.normalizedG(),
        m_clearColor.normalizedB(),
        m_clearColor.normalizedA()
    );
    glClear(GL_COLOR_BUFFER_BIT);

    m_batcher.beginBatch();
}

void Renderer::present()
{
    m_batcher.flushBatch();
    SDL_GL_SwapWindow(m_window);
}
