#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <chrono>
#include <cstdlib>

#include "systems/math/collision.h"
#include "systems/math/shape.h"
#include "systems/render/renderItem.h"

#include "game.h"

Game::Game(Renderer& renderer) :
    m_renderer(renderer)
{

    // TODO: move those to methods on Renderer
    // SDL_SetRenderLogicalPresentation(renderer.renderer(), GAME_WIDTH, GAME_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetWindowMinimumSize(renderer.window(), GAME_WIDTH, GAME_HEIGHT);

    m_renderer.resources().loadTexture("bird", "assets/bird.bmp");
    m_renderer.resources().loadTexture("pipe", "assets/pipe.bmp");

    m_renderer.setClearColor(0x141414);
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
        if (m_player.hitbox.getCollision(pipe.hitbox, &res)) {
            // m_player.hitbox.translate(res.normal * -res.depth);
            m_player.sprite.tint = 0xff0000;
        }
    }

    m_inputManager.flush();

    //
    // RENDER
    //

    m_renderer.clear();
    root.clear();

    root.pos.x = -m_renderer.windowWidth() / 2.F;
    root.pos.y = -m_renderer.windowHeight() / 2.F;

    m_renderer.batcher.beginBatch();
    for (auto& pipe : m_pipes) {
        pipe.render(root);
    }

    m_player.render(root);
    root.renderChildren(root.getMatrix(), m_renderer);

    m_renderer.batcher.flushBatch(m_renderer.resources().getTexture("bird"));
    m_renderer.present();

    return SDL_APP_CONTINUE;
};

SDL_AppResult Game::processEvent(SDL_Event* event)
{
    m_renderer.processSDLEvent(event);
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
