/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/renderer.h"

#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <SDL3/SDL_video.h>

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
    auto* renderer = static_cast<Renderer*>(userData);
    if (!renderer->resizable())
        return 0;

    int width = getWindowWidth();
    int height = getWindowHeight();

    if (!SDL_SetWindowSize(renderer->window(), width, height)) {
        std::cout << "SDL_SetWindowSize error: " << SDL_GetError() << "\n";
    }
    return 0;
};
#endif

Renderer::Renderer(const InitFlags& flags) :
    m_windowWidth(flags.width),
    m_windowHeight(flags.height),
    m_resizable(flags.resizable),
    m_windowTitle(flags.windowTitle) { };

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
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_WindowFlags flags = SDL_WINDOW_OPENGL;

    if (m_resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    m_window = SDL_CreateWindow(
        m_windowTitle.c_str(),
        m_windowWidth,
        m_windowHeight,
        flags
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
    resizeCanvas(0, nullptr, this);
#endif

    if (!SDL_GL_SetSwapInterval(-1)) {
        std::cerr << "Adaptive vsync not supported!\n";
        SDL_GL_SetSwapInterval(1);
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_batcher.init();

    resize();

    return SDL_APP_CONTINUE;
}

void Renderer::processSDLEvent(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED:
        m_windowWidth = event->window.data1;
        m_windowHeight = event->window.data2;

        resize();
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        m_focused = false;
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        m_focused = true;
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

int Renderer::windowHeight() const
{
    return m_windowHeight;
}

bool Renderer::setWindowSize(int width, int height)
{
    return SDL_SetWindowSize(m_window, width, height);
}

[[nodiscard]] bool Renderer::resizable() const
{
    return m_resizable;
}

void Renderer::setResizable(bool resizable)
{
    m_resizable = true;
    SDL_SetWindowResizable(m_window, resizable);
}

[[nodiscard]] bool Renderer::focused() const
{
    return m_focused;
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

void Renderer::resize()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    m_batcher.transform = Matrix3x3(
        {0, 0},
        0,
        {1.F / (m_windowWidth / 2.F), (1.F / (m_windowHeight / 2.F))}
    );
};
