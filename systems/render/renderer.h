#pragma once

#include "systems/math/vec2.h"
#include "systems/render/color.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <unordered_map>
#include <vector>

/**
 * Class to help manage and SDL Window and SDL Renderer
 */
class Renderer
{
private:
    SDL_Renderer* m_renderer = nullptr;
    SDL_Window* m_window = nullptr;

    std::unordered_map<std::string, SDL_Texture*> m_textures;

    std::string m_windowTitle;

    int m_windowWidth;
    int m_windowHeight;

public:
    Renderer();
    Renderer(std::string title, int width, int height);

    ~Renderer();

    /**
     * Tries to create the SDL Window and SDL Renderer
     * If it fails you should exit the program
     */
    SDL_AppResult init();

    void processSDLEvent(SDL_Event* event);

    [[nodiscard]] SDL_Renderer* renderer() const;

    [[nodiscard]] SDL_Window* window() const;

    bool setWindowTitle(const std::string& title);

    [[nodiscard]] std::string windowTitle() const;

    [[nodiscard]] int windowWidth() const;

    bool setWindowSize(int width, int height);

    [[nodiscard]] int windowHeight() const;

    bool present();

    //
    // Rendering functions
    // Temporary stuff using SDL 2D Rendering api until we get a custom OpenGL render
    //

    void setColor(const Color& color);
    void clear(const Color& color);
    void drawCircle(const Vec2& position, float radius);
    void drawEllipse(const Vec2& position, float radiusX, float radiusY);
    void drawRect(const Vec2& position, float width, float height);
    void drawPoly(const std::vector<Vec2>& points);

    //
    // Texture functions
    // Will probably move to a dedicated ResourceManager class or smth like that in the future
    // That will also handle other types of assets like sounds
    //
    void loadTexture(const std::string& id, const std::string& path);
    [[nodiscard]] SDL_Texture* getTexture(const std::string& id) const;
};
