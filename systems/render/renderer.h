#pragma once

#include "batcher.h"
#include "color.h"
#include "resources.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

/**
 * Class to help manage and SDL Window and OpenGL Renderer
 */
class Renderer
{
private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext;

    std::string m_windowTitle;

    int m_windowWidth;
    int m_windowHeight;

    Color m_clearColor;

    ResourceManager m_resources;


public:
    Renderer();
    Renderer(std::string title, int width, int height);
    Renderer(const Renderer&) = delete;

    ~Renderer();

    SpriteBatcher batcher;

    /**
     * Tries to create the SDL Window and SDL Renderer
     * If it fails you should exit the program
     */
    SDL_AppResult init();

    void processSDLEvent(SDL_Event* event);

    [[nodiscard]] SDL_Window* window() const;

    bool setWindowTitle(const std::string& title);

    [[nodiscard]] std::string windowTitle() const;

    [[nodiscard]] int windowWidth() const;
    [[nodiscard]] int windowHeight() const;

    bool setWindowSize(int width, int height);

    ResourceManager& resources() {
        return m_resources;
    }

    Color clearColor()
    {
        return m_clearColor;
    }
    void setClearColor(const Color& color)
    {
        m_clearColor = color;
    }

    void clear() const;
    void present() const;
};
