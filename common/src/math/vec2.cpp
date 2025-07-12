#include "vec2.h"
#include <cmath>
#include <format>

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(const float xPos)
{
    x = xPos;
    y = xPos;
}

Vec2::Vec2(const float xPos, const float yPos)
{
    x = xPos;
    y = yPos;
}

void Vec2::set(const float xPos, const float yPos)
{
    x = xPos;
    y = yPos;
}

Vec2 Vec2::clone() const
{
    return {x, y};
}

bool Vec2::is_valid() const
{
    return std::isfinite(x) && std::isfinite(y);
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

Vec2& Vec2::scale(const float scale)
{
    x *= scale;
    y *= scale;

    return *this;
}

Vec2& Vec2::scale(const Vec2& a)
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

Vec2& Vec2::invert()
{
    x = -x;
    y = -y;
    return *this;
}

Vec2& Vec2::perp()
{
    float oldX = x;
    x = -y;
    y = oldX;
    return *this;
}

Vec2& Vec2::normalize()
{
    const float len = length();

    if (len > VEC2_EPSILON) {
        x /= len;
        y /= len;
    }

    return *this;
};

Vec2& Vec2::normalize(const float length)
{
    if (length > VEC2_EPSILON) {
        x /= length;
        y /= length;
    }

    return *this;
};

Vec2& Vec2::normalize_safe(const Vec2& v)
{
    const float len = length();

    if (len > VEC2_EPSILON) {
        x /= len;
        y /= len;
    } else {
        x = v.x;
        y = v.y;
    }

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
    return Vec2::sub(*this, a).length();
}

float Vec2::dot(const Vec2& a) const
{
    return x * a.x + y * a.y;
}

bool Vec2::equals(const Vec2& a, float epsilon) const
{
    return std::fabs(x - a.x) <= epsilon && std::fabs(y - a.y) <= epsilon;
}

bool Vec2::equals(const Vec2& a) const
{
    return x == a.x && y == a.y;
}

bool Vec2::operator==(const Vec2& a) const
{
    return equals(a);
}

bool Vec2::operator!=(const Vec2& a) const
{
    return !equals(a);
}

float Vec2::operator[](int index) const
{
    return (&x)[index];
}

float& Vec2::operator[](int index)
{
    return (&x)[index];
}

Vec2 Vec2::operator-() const
{
    return {-x, -y};
}

Vec2 Vec2::operator-(const Vec2& a) const
{
    return {x - a.x, y - a.y};
}

float Vec2::operator*(const Vec2& a) const
{
    return x * a.x + y * a.y;
}

Vec2 Vec2::operator*(const float a) const
{
    return {x * a, y * a};
}

Vec2 Vec2::operator/(const float a) const
{
    return {x / a, y / a};
}

Vec2 operator*(const float a, const Vec2 b)
{
    return {b.x * a, b.y * a};
}

Vec2 Vec2::operator+(const Vec2& a) const
{
    return {x + a.x, y + a.y};
}

Vec2& Vec2::operator+=(const Vec2& a)
{
    x += a.x;
    y += a.y;

    return *this;
}

Vec2& Vec2::operator/=(const Vec2& a)
{
    x /= a.x;
    y /= a.y;

    return *this;
}

Vec2& Vec2::operator/=(const float a)
{
    x /= a;
    y /= a;

    return *this;
}

Vec2& Vec2::operator-=(const Vec2& a)
{
    x -= a.x;
    y -= a.y;

    return *this;
}

Vec2& Vec2::operator*=(const float a)
{
    x *= a;
    y *= a;

    return *this;
}

std::string Vec2::to_string() const
{
    return std::format("X: {0:.4f}, Y: {1:.4f}", x, y);
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

std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
    os << vec.to_string();
    return os;
}
