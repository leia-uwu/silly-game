#pragma once

#include "systems/input/inputManager.h"
#include "systems/render/renderItem.h"
#include "systems/render/renderer.h"

#include <SDL3/SDL.h>
#include <chrono>

class Player
{
public:
    RenderItem container;
    SpriteItem body;
    SpriteItem handL;
    SpriteItem handR;

    Player()
    {
        body.tint = 0xff0000;
        handL.scale.set(0.35, 0.35);
        handL.pos.set(40, 40);
        handL.tint = 0x0000ff;
        handR.scale.set(0.35, 0.35);
        handR.pos.set(40, -40);
        handR.tint = 0x00ff00;

        container.addChild({&body, &handL, &handR});
    }
};

class Game
{

private:
    SDL_Window* m_window;
    SDL_Renderer* m_SDLRenderer;
    std::chrono::duration<double> m_lastNow = std::chrono::system_clock::now().time_since_epoch();

public:
    Game(SDL_Window* window, SDL_Renderer* renderer);

    Renderer m_renderer;

    InputManager m_inputManager;

    RenderItem root;

    Player player;

    SDL_AppResult update();

    SDL_AppResult processEvent(SDL_Event* event);

    void shutdown();
};
