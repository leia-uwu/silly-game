#pragma once

#include <SDL3/SDL_events.h>

#include "math/vec2.h"

#include <cstdint>
#include <string>
#include <unordered_map>

class InputManager
{
private:
    std::unordered_map<std::string, bool> m_keys;
    std::unordered_map<uint8_t, bool> m_mouseBtnsDown;

    Vec2 m_mousePos;

    Vec2 m_wheelDelta{0, 0};

public:
    [[nodiscard]] bool isKeyDown(const std::string& key);
    [[nodiscard]] bool isMouseBtnDown(uint8_t btn);
    [[nodiscard]] Vec2 getMousePos() const;
    [[nodiscard]] Vec2 getWheelDelta() const;

    void processSDLEvent(SDL_Event* event);

    void flush();
};
