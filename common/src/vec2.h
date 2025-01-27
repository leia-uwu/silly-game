#pragma once

#include <SDL3/SDL_rect.h>

class Vec2 : public SDL_FPoint
{
public:
    Vec2();
    Vec2(float xPos);
    Vec2(float xPos, float yPos);

    Vec2 clone();

    Vec2* add(const float& value);
    Vec2* add(const Vec2& a);
    Vec2* sub(const float& value);
    Vec2* sub(const Vec2& a);
    Vec2* mul(const float& scale);
    Vec2* mul(const Vec2& a);
    Vec2* div(const float& scale);
    Vec2* div(const Vec2& b);
    Vec2* rotate(const float& rad);
    Vec2* neg();
    Vec2* normalize();
    Vec2* normalize_safe(const Vec2& v = {1.0, 0.0});

    float length_sqr();
    float length();
    float distance_to(const Vec2& a);
    float dot(const Vec2& a);

    bool equals(const Vec2& a, float epsilon = 0.0001);

    static Vec2 add(const Vec2& a, const Vec2& b);
    static Vec2 add(const Vec2& a, const float& value);
    static Vec2 sub(const Vec2& a, const Vec2& b);
    static Vec2 sub(const Vec2& a, const float& value);
    static Vec2 mul(const Vec2& a, const Vec2& b);
    static Vec2 mul(const Vec2& a, const float& scale);
    static Vec2 div(const Vec2& a, const Vec2& b);
    static Vec2 div(const Vec2& a, const float& scale);
};
