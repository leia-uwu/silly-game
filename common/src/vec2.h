#pragma once

// server doesn't need to link to SDL
// so only make Vec2 extend SDL_FPoint if SDL is defined
#ifdef SDL
#include <SDL3/SDL_rect.h>
class Vec2 : public SDL_FPoint
{
public:
#else
class Vec2
{
public:
    float x;
    float y;
#endif
    Vec2();
    Vec2(float xPos);
    Vec2(float xPos, float yPos);

    [[nodiscard]] Vec2 clone() const;

    Vec2& add(float value);
    Vec2& add(const Vec2& a);
    Vec2& sub(float value);
    Vec2& sub(const Vec2& a);
    Vec2& mul(float scale);
    Vec2& mul(const Vec2& a);
    Vec2& div(float scale);
    Vec2& div(const Vec2& a);
    Vec2& rotate(float rad);
    Vec2& neg();
    Vec2& normalize();
    Vec2& normalize_safe(const Vec2& v = {1.0, 0.0});

    [[nodiscard]] float length_sqr() const;
    [[nodiscard]] float length() const;
    [[nodiscard]] float distance_to(const Vec2& a) const;
    [[nodiscard]] float dot(const Vec2& a) const;

    [[nodiscard]] bool equals(const Vec2& a, float epsilon = 0.0001) const;

    static Vec2 add(const Vec2& a, const Vec2& b);
    static Vec2 add(const Vec2& a, float value);
    static Vec2 sub(const Vec2& a, const Vec2& b);
    static Vec2 sub(const Vec2& a, float value);
    static Vec2 mul(const Vec2& a, const Vec2& b);
    static Vec2 mul(const Vec2& a, float scale);
    static Vec2 div(const Vec2& a, const Vec2& b);
    static Vec2 div(const Vec2& a, float scale);
};
