#pragma once

#include "app/app.h"
#include "math/shape.h"
#include "math/vec2.h"
#include "render/renderItem.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

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

    Player();

    void update(float dt);

    void render(RenderItem& root);
};

class Pipe
{
public:
    SpriteItem sprite;

    Vec2 vel{-PIPE_SPEED, 0};

    Rect hitbox;

    Pipe(
        Vec2 pos,
        float width,
        float height
    );

    void update(float dt);

    void render(RenderItem& root);
};

class Game : public GameApp
{
public:
    Game(Renderer* renderer) :
        GameApp(renderer)
    {
    }

    SDL_AppResult init(int argc, char** argv) override;

    RenderItem root;
    SDL_AppResult update(float dt) override;

private:
    Player m_player;
    std::vector<Pipe> m_pipes;
    float m_lastPipeDistance = 0;

    void addPipe();
};
