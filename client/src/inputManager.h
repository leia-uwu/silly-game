#pragma once

#include <SDL3/SDL_events.h>

#include "common/src/vec2.h"
#include <map>
#include <string>

class InputManager
{
private:
    std::map<std::string, bool> m_keys;

    Vec2 m_mousePos;

public:
    void on_key_down(const std::string& key);
    void on_key_up(const std::string& key);
    bool is_key_down(const std::string& key);

    void on_motion_event(const SDL_MouseMotionEvent& event);

    Vec2 get_mouse_pos();
};
