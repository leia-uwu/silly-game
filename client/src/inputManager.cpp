#include "inputManager.h"

void InputManager::on_key_down(const std::string& key)
{
    m_keys.insert_or_assign(key, true);
}

void InputManager::on_key_up(const std::string& key)
{
    m_keys.insert_or_assign(key, false);
}

bool InputManager::is_key_down(const std::string& key)
{
    return m_keys[key];
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
