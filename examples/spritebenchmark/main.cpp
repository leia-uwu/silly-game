#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "app/app.h"
#include "render/renderItem.h"

#include <algorithm>
#include <cstdlib>

class BenchSprite : public SpriteItem
{
public:
    BenchSprite() : SpriteItem() { };
    Vec2F velocity;
};

class Benchmark : public GameApp
{
public:
    Benchmark() :
        GameApp({
            .width = 800,
            .height = 800,
            .resizable = false,
            .windowTitle = "OBS bitrate killer",
        })
    {
    }

    SDL_AppResult init(int /*argc*/, char** /*argv*/) override
    {
        renderer().resources().loadTexture("bird", "assets/bird.bmp");

        for (auto& sprite : m_sprites) {
            sprite.setTexture("bird");
            sprite.width = 64;
            sprite.height = 64;
            sprite.tint = rand();
            sprite.pos.x = renderer().windowWidth() / 2.F;
            sprite.pos.y = renderer().windowHeight() / 2.F;
            sprite.velocity.x = ((std::rand() / (float)RAND_MAX) * 1000) - 500;
            sprite.velocity.y = ((std::rand() / (float)RAND_MAX) * 1000) - 500;
            root.addChild(&sprite);
        }
        renderer().setClearColor(0);

        return SDL_APP_CONTINUE;
    };

    RenderItem root;
    SDL_AppResult update(float dt) override
    {
        root.pos.x = -(renderer().windowWidth() / 2.F);
        root.pos.y = -(renderer().windowHeight() / 2.F);
        for (auto& sprite : m_sprites) {
            sprite.pos += sprite.velocity * dt;

            int width = renderer().windowWidth();
            int height = renderer().windowHeight();

            if (sprite.pos.x < 0 || sprite.pos.x > width) {
                sprite.velocity.x = -sprite.velocity.x;
            }
            if (sprite.pos.y < 0 || sprite.pos.y > height) {
                sprite.velocity.y = -sprite.velocity.y;
            }

            sprite.pos.x = std::clamp(sprite.pos.x, 0.F, (float)width);
            sprite.pos.y = std::clamp(sprite.pos.y, 0.F, (float)width);
        }

        root.renderChildren(root.getMatrix(), renderer());
        return SDL_APP_CONTINUE;
    };

private:
    std::array<BenchSprite, 9999> m_sprites;
};

INIT_APP(Benchmark)
