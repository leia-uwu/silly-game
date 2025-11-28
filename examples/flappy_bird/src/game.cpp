#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

#include <cstdlib>

#include "math/collision.h"
#include "math/shape.h"
#include "render/renderItem.h"

#include "game.h"

SDL_AppResult Game::init(int /*argc*/, char** /*argv*/)
{
    SDL_SetWindowMinimumSize(renderer().window(), GAME_WIDTH, GAME_HEIGHT);
    renderer().setClearColor(0x141414);

    renderer().resources().loadTexture("bird", "assets/bird.bmp");
    renderer().resources().loadTexture("pipe", "assets/pipe.bmp");

    return SDL_APP_CONTINUE;
}

SDL_AppResult Game::update(float dt)
{
    if (renderer().focused()) {
        //
        // update loop, only run when focused
        //

        if (inputManager().isKeyDown("W") || inputManager().isMouseBtnDown(1)) {
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

            Collision::CollRes res;
            if (m_player.hitbox.getCollision(pipe.hitbox, &res)) {
                // m_player.hitbox.translate(res.normal * -res.depth);
                m_player.sprite.tint = 0xff0000;
            }
        }
    }
    //
    // RENDER
    //

    root.clear();

    root.pos.x = -renderer().windowWidth() / 2.F;
    root.pos.y = -renderer().windowHeight() / 2.F;

    for (auto& pipe : m_pipes) {
        pipe.render(root);
    }

    m_player.render(root);
    root.renderChildren(root.getMatrix(), renderer());

    return SDL_APP_CONTINUE;
};

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

Player::Player() :
    hitbox(Rect::fromDims(PLAYER_SIZE, PLAYER_SIZE, {PLAYER_SIZE, PLAYER_SIZE}).getPoints())
{
    sprite.width = PLAYER_SIZE;
    sprite.height = PLAYER_SIZE;
    sprite.setTexture("bird");
}

void Player::update(float dt)
{
    vel.y += GRAVITY * dt;

    Vec2 step = vel * dt;
    hitbox.translate(step);

    Collision::CollRes res;
    if (hitbox.getCollision(GAME_FLOOR, &res)) {
        hitbox.translate(res.normal * -res.depth);
        vel.y = 0;
    }
}

void Player::render(RenderItem& root)
{
    sprite.pos = hitbox.center();
    root.addChild(&sprite);
}

Pipe::Pipe(
    Vec2 pos,
    float width,
    float height
) :
    hitbox(Rect::fromDims(width, height, pos))
{
    sprite.setTexture("pipe");
    sprite.tint = 0x00ff00;
}

void Pipe::update(float dt)
{
    Vec2 step = vel * dt;
    hitbox.translate(step);
}

void Pipe::render(RenderItem& root)
{
    sprite.pos = hitbox.center();
    sprite.width = hitbox.width();
    sprite.height = hitbox.height();

    root.addChild(&sprite);
}
