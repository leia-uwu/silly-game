#pragma once

#include "systems/input/inputManager.h"
#include "systems/math/shape.h"
#include "systems/math/vec2.h"
#include "systems/render/renderItem.h"
#include "systems/render/renderer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <chrono>

static const float GAME_WIDTH = 1024;
static const float GAME_HEIGHT = 512;
static const float PLAYER_SIZE = 64;

static const float GRAVITY = 1000;
static const float JUMP_SPEED = 450;
static const float PIPE_SPEED = 300;
static const float PIPE_WIDTH = 100;
static const float PIPE_GAP = 150;
static const float PIPE_DISTANCE = 400;

static const Rect GAME_FLOOR = Rect::fromDims(GAME_WIDTH, 20, {GAME_WIDTH / 2, GAME_HEIGHT + 10});

class Player
{
public:
    SpriteItem sprite;

    Vec2 vel;

    Polygon hitbox;

    Player() :
        hitbox(Rect::fromDims(PLAYER_SIZE, PLAYER_SIZE, {PLAYER_SIZE, PLAYER_SIZE}).getPoints())
    {
        sprite.width = PLAYER_SIZE;
        sprite.height = PLAYER_SIZE;
        sprite.setTexture("bird");
    }

    void update(float dt)
    {
        vel.y += GRAVITY * dt;

        Vec2 step = vel * dt;
        hitbox.translate(step);

        Collision::CollRes res;
        if (COLLISION_FNS.check(hitbox, GAME_FLOOR, &res)) {
            hitbox.translate(res.normal * -res.depth);
            vel.y = 0;
        }
    }

    void render(RenderItem& root)
    {
        sprite.pos = hitbox.center();
        root.addChild(&sprite);
    }
};

class Pipe;

class Game
{

private:
    Renderer& m_renderer;
    InputManager m_inputManager;

    std::chrono::duration<double> m_lastNow = std::chrono::system_clock::now().time_since_epoch();

    Player m_player;
    std::vector<Pipe> m_pipes;

    float m_lastPipeDistance = 0;

    void addPipe();

public:
    Game(Renderer& renderer);

    RenderItem root;

    [[nodiscard]] Renderer& renderer()
    {
        return m_renderer;
    };

    SDL_AppResult update();

    SDL_AppResult processEvent(SDL_Event* event);
};

class Pipe
{
public:
    SpriteItem sprite;

    Vec2 vel{-PIPE_SPEED, 0};

    Rect hitbox;

    // RectItem debugHitbox;

    Pipe(
        Vec2 pos,
        float width,
        float height
    ) :
        hitbox(Rect::fromDims(width, height, pos))
    {
        sprite.setTexture("pipe");
        sprite.tint = 0x00ff00;
    }

    void update(float dt)
    {
        Vec2 step = vel * dt;
        hitbox.translate(step);
    }

    void render(RenderItem& root)
    {
        sprite.pos = hitbox.center();
        sprite.width = hitbox.width();
        sprite.height = hitbox.height();

        // debugHitbox.pos = pos;
        // debugHitbox.width = hitbox.width();
        // debugHitbox.height = hitbox.height();

        root.addChild(&sprite);
        // root.addChild(&debugHitbox);
    }
};
