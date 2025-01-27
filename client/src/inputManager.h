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
    void on_key_down(const char* key);
    void on_key_up(const char* key);
    bool is_key_down(const char* key);

    void on_motion_event(const SDL_MouseMotionEvent& event);

    Vec2 get_mouse_pos();
};
