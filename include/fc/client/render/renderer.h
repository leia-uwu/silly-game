/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/color.h"
#include "fc/client/render/batcher.h"
#include "fc/client/resources.h"

#include <glad/gl.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

/**
 * Class to help manage and SDL Window and OpenGL Renderer
 */
class Renderer
{
public:
    struct InitFlags
    {
        size_t width = 800;
        size_t height = 400;
        bool resizable = true;
        std::string windowTitle = "Game";
    };

    Renderer(const InitFlags& flags);
    Renderer(const Renderer&) = delete;

    ~Renderer();

    /**
     * Tries to initialize SDL, the window and OpenGL context
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

    [[nodiscard]] bool resizable() const;
    void setResizable(bool resizable);
    [[nodiscard]] bool focused() const;

    ResourceManager& resources()
    {
        return m_resources;
    }

    RenderBatcher& batcher()
    {
        return m_batcher;
    }

    Color clearColor()
    {
        return m_clearColor;
    }

    void setClearColor(const Color& color)
    {
        m_clearColor = color;
    }

    void clear();

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext;

    std::string m_windowTitle;

    int m_windowWidth;
    int m_windowHeight;

    bool m_resizable = true;
    bool m_focused = true;

    void resize();

    Color m_clearColor;

    ResourceManager m_resources;
    RenderBatcher m_batcher;
};
