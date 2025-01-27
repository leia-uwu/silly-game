#include "inputManager.h"

void InputManager::onKeyDown(const char* key)
{
    m_keys[key] = true;
}

void InputManager::onKeyUp(const char* key)
{
    m_keys[key] = false;
}

bool InputManager::isKeyDown(const char* key)
{
    return m_keys[key] ? true : false;
}

void InputManager::onMotionEvent(SDL_MouseMotionEvent event)
{
    m_mousePos.x = event.x;
    m_mousePos.y = event.y;
}

Vec2 InputManager::getMousePos()
{
    return m_mousePos;
}
