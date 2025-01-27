#pragma once

#include <SDL3/SDL_events.h>

#include "../../common/src/vec2.h"
#include <map>

class InputManager
{
private:
    std::map<const char*, bool> m_keys;

    Vec2 m_mousePos;

public:
    void onKeyDown(const char* key);
    void onKeyUp(const char* key);
    bool isKeyDown(const char* key);

    void onMotionEvent(SDL_MouseMotionEvent event);

    Vec2 getMousePos();
};
