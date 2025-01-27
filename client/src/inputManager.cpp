#include "inputManager.h"

void InputManager::on_key_down(const char* key)
{
    m_keys[key] = true;
}

void InputManager::on_key_up(const char* key)
{
    m_keys[key] = false;
}

bool InputManager::is_key_down(const char* key)
{
    return m_keys[key] ? true : false;
}

void InputManager::on_motion_event(const SDL_MouseMotionEvent& event)
{
    m_mousePos.x = event.x;
    m_mousePos.y = event.y;
}

Vec2 InputManager::get_mouse_pos()
{
    return m_mousePos;
}
