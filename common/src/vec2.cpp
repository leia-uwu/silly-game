#include "vec2.h"
#include <cmath>

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(float xPos)
{
    x = xPos;
    y = xPos;
}

Vec2::Vec2(float xPos, float yPos)
{
    x = xPos;
    y = yPos;
}

Vec2 Vec2::clone()
{
    return {x, y};
}

Vec2* Vec2::add(const float& value)
{
    x += value;
    y += value;

    return this;
}

Vec2* Vec2::add(const Vec2& a)
{
    x += a.x;
    y += a.y;

    return this;
}

Vec2* Vec2::sub(const float& value)
{
    x -= value;
    y -= value;

    return this;
}

Vec2* Vec2::sub(const Vec2& a)
{
    x -= a.x;
    y -= a.y;

    return this;
}

Vec2* Vec2::mul(const float& value)
{
    x *= value;
    y *= value;

    return this;
}

Vec2* Vec2::mul(const Vec2& a)
{
    x *= a.x;
    y *= a.y;

    return this;
}

Vec2* Vec2::div(const float& value)
{
    x /= value;
    y /= value;

    return this;
};

Vec2* Vec2::div(const Vec2& a)
{
    x /= a.x;
    y /= a.y;

    return this;
}

Vec2* Vec2::rotate(const float& rad)
{
    float cosr = cos(rad);
    float sinr = sin(rad);

    x = x * cosr - y * sinr;
    y = x * sinr + y * cosr;

    return this;
}

Vec2* Vec2::neg()
{
    return new Vec2(-x, -y);
}

Vec2* Vec2::normalize()
{
    float eps = 0.000001;
    float len = length();

    x = len > eps ? x / len : x;
    y = len > eps ? y / len : y;

    return this;
};

Vec2* Vec2::normalize_safe(const Vec2& v)
{
    float eps = 0.000001;
    float len = length();

    x = len > eps ? x / len : v.x;
    y = len > eps ? y / len : v.y;

    return this;
}

float Vec2::length_sqr()
{
    return x * x + y * y;
}

float Vec2::length()
{
    return sqrt(length_sqr());
}

float Vec2::distance_to(const Vec2& a)
{
    return clone().sub(a)->length();
}

float Vec2::dot(const Vec2& a)
{
    return x * a.x + y * a.y;
}

bool Vec2::equals(const Vec2& a, float epsilon)
{
    return fabs(x - a.x) <= epsilon && fabs(y - a.y) <= epsilon;
}

Vec2 Vec2::add(const Vec2& a, const Vec2& b)
{
    return {a.x + b.x, a.y + b.y};
}

Vec2 Vec2::add(const Vec2& a, const float& value)
{
    return {a.x + value, a.y + value};
}

Vec2 Vec2::sub(const Vec2& a, const Vec2& b)
{
    return {a.x - b.x, a.y - b.y};
}

Vec2 Vec2::sub(const Vec2& a, const float& value)
{
    return {a.x - value, a.y - value};
}

Vec2 Vec2::mul(const Vec2& a, const Vec2& b)
{
    return {a.x * b.x, a.y * b.y};
}

Vec2 Vec2::mul(const Vec2& a, const float& scale)
{
    return {a.x * scale, a.y * scale};
}

Vec2 Vec2::div(const Vec2& a, const Vec2& b)
{
    return {a.x / b.x, a.y / b.y};
}

Vec2 Vec2::div(const Vec2& a, const float& scale)
{
    return {a.x / scale, a.y / scale};
}
