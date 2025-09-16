#include "renderer.h"
#include "systems/math/gmath.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <cmath>

#include <iostream>

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

Renderer::Renderer()
    : m_windowTitle("Game")
    , m_windowWidth(800)
    , m_windowHeight(400) { };

Renderer::Renderer(std::string title, int width, int height)
    : m_windowTitle(std::move(title))
    , m_windowWidth(width)
    , m_windowHeight(height) { };

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

SDL_AppResult Renderer::init()
{
    m_window = SDL_CreateWindow(
        "Game",
        800,
        450,
        SDL_WINDOW_RESIZABLE
    );

    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (m_renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, m_window, 0, resizeCanvas);
    resizeCanvas(0, nullptr, m_window);
#endif

    SDL_SetRenderVSync(m_renderer, 1);

    return SDL_APP_CONTINUE;
}

void Renderer::processSDLEvent(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED:
        m_windowWidth = event->window.data1;
        m_windowHeight = event->window.data2;
        break;
    }
}

SDL_Renderer* Renderer::renderer() const
{
    return m_renderer;
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

bool Renderer::present()
{
    return SDL_RenderPresent(m_renderer);
}

void Renderer::setColor(const Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::clear(const Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.b, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

void Renderer::drawCircle(const Vec2& position, const float radius)
{
    drawEllipse(position, radius, radius);
}

// https://stackoverflow.com/a/47766167
void Renderer::drawEllipse(const Vec2& position, const float radiusX, const float radiusY)
{
    const float rx = radiusX;
    const float ry = radiusY;
    // drew  28 lines with   4x4  circle with precision of 150 0ms
    // drew 132 lines with  25x14 circle with precision of 150 0ms
    // drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 270; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0; // angle that will be increased each loop

    // starting point
    int x = (float)rx * std::cos(theta); // start point
    int y = (float)ry * std::sin(theta); // start point
    int x1 = x;
    int y1 = y;

    // repeat until theta >= 90;
    float step = GMath::PI_2 / (float)prec; // amount to add to theta each time (degrees)
    for (theta = step; theta <= GMath::PI_2; theta += step) // step through only a 90 arc (1 quadrant)
    {
        // get new point location
        x1 = std::round((rx * std::cos(theta)) + 0.5); // new point (+.5 is a quick rounding method)
        y1 = std::round((ry * std::sin(theta)) + 0.5); // new point (+.5 is a quick rounding method)

        // draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1)) // only draw if coordinate changed
        {
            SDL_RenderLine(m_renderer, position.x + x, position.y - y, position.x + x1, position.y - y1); // quadrant TR
            SDL_RenderLine(m_renderer, position.x - x, position.y - y, position.x - x1, position.y - y1); // quadrant TL
            SDL_RenderLine(m_renderer, position.x - x, position.y + y, position.x - x1, position.y + y1); // quadrant BL
            SDL_RenderLine(m_renderer, position.x + x, position.y + y, position.x + x1, position.y + y1); // quadrant BR
        }
        // save previous points
        x = x1; // save new previous point
        y = y1; // save new previous point
    }
    // arc did not finish because of rounding, so finish the arc
    if (x != 0) {
        x = 0;
        SDL_RenderLine(m_renderer, position.x + x, position.y - y, position.x + x1, position.y - y1); // quadrant TR
        SDL_RenderLine(m_renderer, position.x - x, position.y - y, position.x - x1, position.y - y1); // quadrant TL
        SDL_RenderLine(m_renderer, position.x - x, position.y + y, position.x - x1, position.y + y1); // quadrant BL
        SDL_RenderLine(m_renderer, position.x + x, position.y + y, position.x + x1, position.y + y1); // quadrant BR
    }
}

void Renderer::drawRect(const Vec2& position, const float width, const float height)
{
    const float w = width;
    const float h = height;
    SDL_FRect rect = {
        position.x - w / 2,
        position.y - h / 2,
        w,
        h
    };
    SDL_RenderRect(m_renderer, &rect);
}

void Renderer::drawPoly(const std::vector<Vec2>& points)
{
    size_t size = points.size();
    for (size_t i = 0; i < size; i++) {
        Vec2 a = points[i];
        Vec2 b = points[(i + 1) % size];

        SDL_RenderLine(m_renderer, a.x, a.y, b.x, b.y);
    }
}

void Renderer::loadTexture(const std::string& id, const std::string& path)
{
    std::string filePath = SDL_GetBasePath();
    filePath += path;

    std::cout << "Loading " << filePath << "\n";

    SDL_Surface* surf = SDL_LoadBMP(filePath.c_str());
    if (surf == nullptr) {
        std::cerr << "Couldn't load bitmap: " << SDL_GetError() << "\n";
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    if (texture == nullptr) {
        std::cerr << "Couldn't create texture: " << SDL_GetError() << "\n";
    }
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
    SDL_DestroySurface(surf);

    m_textures[id] = texture;
}

SDL_Texture* Renderer::getTexture(const std::string& id) const
{
    if (!m_textures.contains(id)) {
        return nullptr;
    }
    return m_textures.at(id);
}
