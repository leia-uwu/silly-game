#include "vec2.h"
#include <cmath>

Vec2::Vec2() noexcept
{
    x = 0;
    y = 0;
}

Vec2::Vec2(const float xPos) noexcept
{
    x = xPos;
    y = xPos;
}

Vec2::Vec2(const float xPos, const float yPos) noexcept
{
    x = xPos;
    y = yPos;
}

Vec2 Vec2::clone() const noexcept
{
    return {x, y};
}

Vec2& Vec2::add(const float value)
{
    x += value;
    y += value;

    return *this;
}

Vec2& Vec2::add(const Vec2& a)
{
    x += a.x;
    y += a.y;

    return *this;
}

Vec2& Vec2::sub(const float value)
{
    x -= value;
    y -= value;

    return *this;
}

Vec2& Vec2::sub(const Vec2& a)
{
    x -= a.x;
    y -= a.y;

    return *this;
}

Vec2& Vec2::mul(const float scale)
{
    x *= scale;
    y *= scale;

    return *this;
}

Vec2& Vec2::mul(const Vec2& a)
{
    x *= a.x;
    y *= a.y;

    return *this;
}

Vec2& Vec2::div(const float scale)
{
    x /= scale;
    y /= scale;

    return *this;
};

Vec2& Vec2::div(const Vec2& a)
{
    x /= a.x;
    y /= a.y;

    return *this;
}

Vec2& Vec2::rotate(const float rad)
{
    const float cosr = std::cos(rad);
    const float sinr = std::sin(rad);

    x = x * cosr - y * sinr;
    y = x * sinr + y * cosr;

    return *this;
}

Vec2& Vec2::neg()
{
    x = -x;
    y = -y;
    return *this;
}

Vec2& Vec2::normalize()
{
    static constexpr float eps = 0.000001;
    const float len = length();

    x = len > eps ? x / len : x;
    y = len > eps ? y / len : y;

    return *this;
};

Vec2& Vec2::normalize_safe(const Vec2& v)
{
    static constexpr float eps = 0.000001;
    const float len = length();

    x = len > eps ? x / len : v.x;
    y = len > eps ? y / len : v.y;

    return *this;
}

float Vec2::length_sqr() const
{
    return x * x + y * y;
}

float Vec2::length() const
{
    return std::sqrt(length_sqr());
}

float Vec2::distance_to(const Vec2& a) const
{
    return clone().sub(a).length();
}

float Vec2::dot(const Vec2& a) const
{
    return x * a.x + y * a.y;
}

bool Vec2::equals(const Vec2& a, float epsilon) const
{
    return std::fabs(x - a.x) <= epsilon && std::fabs(y - a.y) <= epsilon;
}

Vec2 Vec2::add(const Vec2& a, const Vec2& b)
{
    return {a.x + b.x, a.y + b.y};
}

Vec2 Vec2::add(const Vec2& a, const float value)
{
    return {a.x + value, a.y + value};
}

Vec2 Vec2::sub(const Vec2& a, const Vec2& b)
{
    return {a.x - b.x, a.y - b.y};
}

Vec2 Vec2::sub(const Vec2& a, const float value)
{
    return {a.x - value, a.y - value};
}

Vec2 Vec2::mul(const Vec2& a, const Vec2& b)
{
    return {a.x * b.x, a.y * b.y};
}

Vec2 Vec2::mul(const Vec2& a, const float scale)
{
    return {a.x * scale, a.y * scale};
}

Vec2 Vec2::div(const Vec2& a, const Vec2& b)
{
    return {a.x / b.x, a.y / b.y};
}

Vec2 Vec2::div(const Vec2& a, const float scale)
{
    return {a.x / scale, a.y / scale};
}
