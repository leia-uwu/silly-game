#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include "systems/math/collision.h"
#include "systems/math/shape.h"
#include "systems/render/renderItem.h"

#include "game.h"

Game::Game(SDL_Window* window, SDL_Renderer* renderer)
    : m_window(window)
    , m_SDLRenderer(renderer)
    , m_renderer(renderer)
{

    SDL_SetRenderLogicalPresentation(renderer, GAME_WIDTH, GAME_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetWindowMinimumSize(window, GAME_WIDTH, GAME_HEIGHT);

    m_renderer.loadTexture("bird", "../examples/flappy-bird/assets/bird.bmp");
    m_renderer.loadTexture("pipe", "../examples/flappy-bird/assets/pipe.bmp");
}

SDL_AppResult Game::update()
{
    //
    // UPDATE
    //
    std::chrono::duration<double> newNow = std::chrono::system_clock::now().time_since_epoch();
    float dt = std::chrono::duration<double, std::ratio<1>>(newNow - m_lastNow).count();
    m_lastNow = newNow;

    if (m_inputManager.isKeyDown("W") || m_inputManager.isMouseBtnDown(0)) {
        m_player.vel.y = -500;
    }

    m_player.update(dt);

    m_lastPipeDistance += PIPE_SPEED * dt;

    if (m_lastPipeDistance > PIPE_DISTANCE) {
        m_lastPipeDistance = 0;

        addPipe();
    }

    m_player.sprite.tint = 0xaa55ff;

    for (auto& pipe : m_pipes) {
        pipe.update(dt);
        // std::cout << pipe.hitbox.center() << "\n";

        Collision::CollRes res;
        if (COLLISION_FNS.check(m_player.hitbox, pipe.hitbox, &res)) {
            // m_player.hitbox.translate(res.normal * -res.depth);
            m_player.sprite.tint = 0xff0000;
        }
    }

    m_inputManager.flush();

    //
    // RENDER
    //

    SDL_SetRenderDrawColor(m_SDLRenderer, 20, 20, 20, 255);
    SDL_RenderClear(m_SDLRenderer);

    root.clear();
    for (auto& pipe : m_pipes) {
        pipe.render(root);
    }
    m_player.render(root);
    root.renderChildren(root.getMatrix(), m_renderer);

    SDL_RenderPresent(m_SDLRenderer);
    return SDL_APP_CONTINUE;
};

SDL_AppResult Game::processEvent(SDL_Event* event)
{
    m_inputManager.processSDLEvent(event);

    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void Game::addPipe()
{

    const int pad = PIPE_GAP / 2;
    const int gapYMax = GAME_HEIGHT - PIPE_GAP - pad;

    const float randomT = std::rand() / (float)RAND_MAX;

    const float gapYStart = (randomT * (gapYMax - pad)) + pad;
    const float gapYEnd = gapYStart + PIPE_GAP;

    auto findPipe = [this](Vec2 pos, float width, float height) {
        for (Pipe& pipe : m_pipes) {
            if (pipe.hitbox.max.x < 0) {

                Rect r = Rect::fromDims(width, height, pos);
                pipe.hitbox.min = r.min;
                pipe.hitbox.max = r.max;

                return;
            }
        }

        m_pipes.emplace_back(pos, width, height);
    };

    findPipe(
        {GAME_WIDTH + (PIPE_WIDTH / 2), gapYStart / 2},
        PIPE_WIDTH,
        gapYStart
    );

    findPipe(
        {GAME_WIDTH + (PIPE_WIDTH / 2), gapYEnd + ((GAME_HEIGHT - gapYEnd) / 2)},
        PIPE_WIDTH,
        GAME_HEIGHT - gapYEnd
    );
}

void Game::shutdown()
{
    SDL_DestroyRenderer(m_SDLRenderer);
    SDL_DestroyWindow(m_window);
}
