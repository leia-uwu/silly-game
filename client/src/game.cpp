#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <chrono>
#include <cmath>

#include "systems/render/renderItem.h"
#include "systems/render/renderer.h"

#include "game.h"

Game::Game(Renderer& renderer)
    : m_renderer(renderer)
{

    // root.pos.x = 100;
    // root.pos.y = 100;
    m_renderer.loadTexture("circle", "../client/assets/circle.bmp");

    auto* parent = new RenderItem();
    parent->scale.set(5, 5);
    parent->pos.set(-500, -500);
    root.addChild(parent);
    for (int i = 0; i < 100; i++) {
        auto* circle = new SpriteItem();
        circle->setTexture("circle");
        // circle->rad = 100;
        circle->pos.set(100, 100);
        circle->tint = 0xff0000;
        circle->scale.set(0.9, 0.9);
        circle->rot = 0.6;
        parent->addChild(circle);
        parent = circle;
    }

    // parent = &root;
    // for (int i = 0; i < 100; i++) {
    //     auto* circle = new CircleItem();
    //     // circle->setTexture(texture);
    //     circle->rad = 50;
    //     circle->pos.set(100, 100);
    //     circle->color = 0x00ff00;
    //     circle->scale.set(0.9, 0.9);
    //     circle->rot = 0.6;
    //     parent->addChild(circle);
    //     parent = circle;
    // }

    player.body.setTexture("circle");
    player.handL.setTexture("circle");
    player.handR.setTexture("circle");

    root.addChild(&player.container);
}

SDL_AppResult Game::update()
{
    std::chrono::duration<double> newNow = std::chrono::system_clock::now().time_since_epoch();
    float dt = std::chrono::duration<double, std::ratio<1>>(newNow - m_lastNow).count();
    m_lastNow = newNow;

    SDL_SetRenderDrawColor(m_renderer.renderer(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.renderer());

    float speed = 240 * dt;
    if (m_inputManager.isKeyDown("A")) {
        player.container.pos.x -= speed;
    }
    if (m_inputManager.isKeyDown("D")) {
        player.container.pos.x += speed;
    }
    if (m_inputManager.isKeyDown("W")) {
        player.container.pos.y -= speed;
    } else if (m_inputManager.isKeyDown("S")) {
        player.container.pos.y += speed;
    }

    if (m_inputManager.getWheelDelta().y > 0) {
        root.scale *= 1.1;
    } else if (m_inputManager.getWheelDelta().y < 0) {
        root.scale *= 0.9;
        root.scale.x = std::fmax(root.scale.x, 0.001);
        root.scale.y = std::fmax(root.scale.y, 0.001);
    }

    Vec2 windowSize{(float)m_renderer.windowWidth(), (float)m_renderer.windowHeight()};
    Vec2 halfSize = windowSize / 2.F;

    auto sub = m_inputManager.getMousePos() - halfSize;
    player.container.rot = std::atan2(sub.y, sub.x);

    root.pos = -(player.container.pos * root.scale.x) + halfSize;
    root.renderChildren(root.getMatrix(), m_renderer);

    SDL_RenderPresent(m_renderer.renderer());
    m_inputManager.flush();
    return SDL_APP_CONTINUE;
};

SDL_AppResult Game::processEvent(SDL_Event* event)
{
    m_renderer.processSDLEvent(event);
    m_inputManager.processSDLEvent(event);

    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;
    }
    return SDL_APP_CONTINUE;
}
