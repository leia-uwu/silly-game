#pragma once

#include <cmath>
#include <format>
#include <string>

#define VEC2_EPSILON 0.001F

class Vec2
{
public:
    float x;
    float y;

    Vec2();
    Vec2(float xPos);
    Vec2(float xPos, float yPos);

    [[nodiscard]] Vec2 clone() const;

    void set(float xPos, float yPos);

    [[nodiscard]] bool isValid() const;

    Vec2& add(float value);
    Vec2& add(const Vec2& a);
    Vec2& sub(float value);
    Vec2& sub(const Vec2& a);
    Vec2& scale(float scale);
    Vec2& scale(const Vec2& a);
    Vec2& div(float scale);
    Vec2& div(const Vec2& a);
    Vec2& rotate(float rad);
    Vec2& invert();
    Vec2& perp();
    Vec2& normalize();
    Vec2& normalize(float length);
    Vec2& normalizeSafe(const Vec2& v = {1.0, 0.0});

    [[nodiscard]] float lengthSqr() const;
    [[nodiscard]] float length() const;
    [[nodiscard]] float distanceTo(const Vec2& a) const;
    [[nodiscard]] float dot(const Vec2& a) const;

    [[nodiscard]] bool equals(const Vec2& a) const;
    [[nodiscard]] bool equals(const Vec2& a, float epsilon) const;
    bool operator==(const Vec2& a) const;
    bool operator!=(const Vec2& a) const;

    float operator[](int index) const;
    float& operator[](int index);
    Vec2 operator-() const;
    float operator*(const Vec2& a) const;
    Vec2 operator*(float a) const;
    Vec2 operator/(float a) const;
    Vec2 operator+(const Vec2& a) const;
    Vec2 operator-(const Vec2& a) const;
    Vec2& operator+=(const Vec2& a);
    Vec2& operator-=(const Vec2& a);
    Vec2& operator/=(const Vec2& a);
    Vec2& operator/=(float a);
    Vec2& operator*=(float a);

    [[nodiscard]] std::string toString() const;

    static Vec2 add(const Vec2& a, const Vec2& b);
    static Vec2 add(const Vec2& a, float value);
    static Vec2 sub(const Vec2& a, const Vec2& b);
    static Vec2 sub(const Vec2& a, float value);
    static Vec2 mul(const Vec2& a, const Vec2& b);
    static Vec2 mul(const Vec2& a, float scale);
    static Vec2 div(const Vec2& a, const Vec2& b);
    static Vec2 div(const Vec2& a, float scale);

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);
};

inline Vec2::Vec2()
    : x(0)
    , y(0)
{
}

inline Vec2::Vec2(const float xPos)
    : x(xPos)
    , y(xPos)
{
}

inline Vec2::Vec2(const float xPos, const float yPos)
    : x(xPos)
    , y(yPos)
{
}

inline void Vec2::set(const float xPos, const float yPos)
{
    x = xPos;
    y = yPos;
}

inline Vec2 Vec2::clone() const
{
    return {x, y};
}

inline bool Vec2::isValid() const
{
    return std::isfinite(x) && std::isfinite(y);
}

inline Vec2& Vec2::add(const float value)
{
    x += value;
    y += value;

    return *this;
}

inline Vec2& Vec2::add(const Vec2& a)
{
    x += a.x;
    y += a.y;

    return *this;
}

inline Vec2& Vec2::sub(const float value)
{
    x -= value;
    y -= value;

    return *this;
}

inline Vec2& Vec2::sub(const Vec2& a)
{
    x -= a.x;
    y -= a.y;

    return *this;
}

inline Vec2& Vec2::scale(const float scale)
{
    x *= scale;
    y *= scale;

    return *this;
}

inline Vec2& Vec2::scale(const Vec2& a)
{
    x *= a.x;
    y *= a.y;

    return *this;
}

inline Vec2& Vec2::div(const float scale)
{
    x /= scale;
    y /= scale;

    return *this;
};

inline Vec2& Vec2::div(const Vec2& a)
{
    x /= a.x;
    y /= a.y;

    return *this;
}

inline Vec2& Vec2::rotate(const float rad)
{
    const float cosr = std::cos(rad);
    const float sinr = std::sin(rad);

    x = x * cosr - y * sinr;
    y = x * sinr + y * cosr;

    return *this;
}

inline Vec2& Vec2::invert()
{
    x = -x;
    y = -y;
    return *this;
}

inline Vec2& Vec2::perp()
{
    float oldX = x;
    x = -y;
    y = oldX;
    return *this;
}

inline Vec2& Vec2::normalize()
{
    const float len = length();

    if (len > VEC2_EPSILON) {
        x /= len;
        y /= len;
    }

    return *this;
};

inline Vec2& Vec2::normalize(const float length)
{
    if (length > VEC2_EPSILON) {
        x /= length;
        y /= length;
    }

    return *this;
};

inline Vec2& Vec2::normalizeSafe(const Vec2& v)
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

inline float Vec2::lengthSqr() const
{
    return x * x + y * y;
}

inline float Vec2::length() const
{
    return std::sqrt(lengthSqr());
}

inline float Vec2::distanceTo(const Vec2& a) const
{
    return Vec2::sub(*this, a).length();
}

inline float Vec2::dot(const Vec2& a) const
{
    return x * a.x + y * a.y;
}

inline bool Vec2::equals(const Vec2& a, float epsilon) const
{
    return std::fabs(x - a.x) <= epsilon && std::fabs(y - a.y) <= epsilon;
}

inline bool Vec2::equals(const Vec2& a) const
{
    return x == a.x && y == a.y;
}

inline bool Vec2::operator==(const Vec2& a) const
{
    return equals(a);
}

inline bool Vec2::operator!=(const Vec2& a) const
{
    return !equals(a);
}

inline float Vec2::operator[](int index) const
{
    return (&x)[index];
}

inline float& Vec2::operator[](int index)
{
    return (&x)[index];
}

inline Vec2 Vec2::operator-() const
{
    return {-x, -y};
}

inline Vec2 Vec2::operator-(const Vec2& a) const
{
    return {x - a.x, y - a.y};
}

inline float Vec2::operator*(const Vec2& a) const
{
    return x * a.x + y * a.y;
}

inline Vec2 Vec2::operator*(const float a) const
{
    return {x * a, y * a};
}

inline Vec2 Vec2::operator/(const float a) const
{
    return {x / a, y / a};
}

inline Vec2 operator*(const float a, const Vec2 b)
{
    return {b.x * a, b.y * a};
}

inline Vec2 Vec2::operator+(const Vec2& a) const
{
    return {x + a.x, y + a.y};
}

inline Vec2& Vec2::operator+=(const Vec2& a)
{
    x += a.x;
    y += a.y;

    return *this;
}

inline Vec2& Vec2::operator/=(const Vec2& a)
{
    x /= a.x;
    y /= a.y;

    return *this;
}

inline Vec2& Vec2::operator/=(const float a)
{
    x /= a;
    y /= a;

    return *this;
}

inline Vec2& Vec2::operator-=(const Vec2& a)
{
    x -= a.x;
    y -= a.y;

    return *this;
}

inline Vec2& Vec2::operator*=(const float a)
{
    x *= a;
    y *= a;

    return *this;
}

inline std::string Vec2::toString() const
{
    return std::format("X: {0:.4f}, Y: {1:.4f}", x, y);
}

inline Vec2 Vec2::add(const Vec2& a, const Vec2& b)
{
    return {a.x + b.x, a.y + b.y};
}

inline Vec2 Vec2::add(const Vec2& a, const float value)
{
    return {a.x + value, a.y + value};
}

inline Vec2 Vec2::sub(const Vec2& a, const Vec2& b)
{
    return {a.x - b.x, a.y - b.y};
}

inline Vec2 Vec2::sub(const Vec2& a, const float value)
{
    return {a.x - value, a.y - value};
}

inline Vec2 Vec2::mul(const Vec2& a, const Vec2& b)
{
    return {a.x * b.x, a.y * b.y};
}

inline Vec2 Vec2::mul(const Vec2& a, const float scale)
{
    return {a.x * scale, a.y * scale};
}

inline Vec2 Vec2::div(const Vec2& a, const Vec2& b)
{
    return {a.x / b.x, a.y / b.y};
}

inline Vec2 Vec2::div(const Vec2& a, const float scale)
{
    return {a.x / scale, a.y / scale};
}

inline std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
    os << vec.toString();
    return os;
}
