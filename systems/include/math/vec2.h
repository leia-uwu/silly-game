#pragma once

#include <cmath>
#include <format>
#include <ostream>
#include <string>

#define VEC2_EPSILON 0.001F

class Vec2
{
public:
    float x;
    float y;

    Vec2() : x(0), y(0)
    {
    }

    Vec2(float xAndY) : x(xAndY), y(xAndY)
    {
    }

    Vec2(float xPos, float yPos) : x(xPos), y(yPos)
    {
    }

    [[nodiscard]] Vec2 clone() const
    {
        return {x, y};
    }

    void set(float xPos, float yPos)
    {
        x = xPos;
        y = yPos;
    }

    [[nodiscard]] bool isValid() const
    {
        return std::isfinite(x) && std::isfinite(y);
    };

    Vec2& add(float value)
    {
        x += value;
        y += value;

        return *this;
    }

    Vec2& add(const Vec2& a)
    {
        x += a.x;
        y += a.y;

        return *this;
    }

    Vec2& sub(float value)
    {
        x -= value;
        y -= value;

        return *this;
    }

    Vec2& sub(const Vec2& a)
    {
        x -= a.x;
        y -= a.y;

        return *this;
    }

    Vec2& scale(float scale)
    {
        x *= scale;
        y *= scale;

        return *this;
    }

    Vec2& scale(const Vec2& a)
    {
        x *= a.x;
        y *= a.y;

        return *this;
    }

    Vec2& div(float scale)
    {
        x /= scale;
        y /= scale;

        return *this;
    }

    Vec2& div(const Vec2& a)
    {
        x /= a.x;
        y /= a.y;

        return *this;
    }

    Vec2& rotate(float rad)
    {
        const float cosr = std::cos(rad);
        const float sinr = std::sin(rad);

        x = x * cosr - y * sinr;
        y = x * sinr + y * cosr;

        return *this;
    }

    Vec2& invert()
    {
        x = -x;
        y = -y;
        return *this;
    }

    Vec2& perp()
    {
        float oldX = x;
        x = -y;
        y = oldX;
        return *this;
    }

    Vec2& normalize()
    {
        const float len = length();

        if (len > VEC2_EPSILON) {
            x /= len;
            y /= len;
        }

        return *this;
    }

    Vec2& normalize(float length)
    {
        if (length > VEC2_EPSILON) {
            x /= length;
            y /= length;
        }

        return *this;
    }

    Vec2& normalizeSafe(const Vec2& v = {1.0, 0.0})
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

    [[nodiscard]] float lengthSqr() const
    {
        return x * x + y * y;
    }

    [[nodiscard]] float length() const
    {
        return std::sqrt(lengthSqr());
    }

    [[nodiscard]] float distanceTo(const Vec2& a) const
    {
        return Vec2::sub(*this, a).length();
    }

    [[nodiscard]] float dot(const Vec2& a) const
    {
        return x * a.x + y * a.y;
    }

    [[nodiscard]] bool equals(const Vec2& a) const
    {
        return x == a.x && y == a.y;
    }

    [[nodiscard]] bool equals(const Vec2& a, float epsilon) const
    {
        return std::fabs(x - a.x) <= epsilon && std::fabs(y - a.y) <= epsilon;
    }

    bool operator==(const Vec2& a) const
    {
        return equals(a);
    }

    bool operator!=(const Vec2& a) const
    {
        return !equals(a);
    }

    [[nodiscard]] float operator[](int index) const
    {
        return (&x)[index];
    }

    float& operator[](int index)
    {
        return (&x)[index];
    }

    [[nodiscard]] Vec2 operator-() const
    {
        return {-x, -y};
    }

    float operator*(const Vec2& a) const
    {
        return x * a.x + y * a.y;
    }

    [[nodiscard]] Vec2 operator*(float a) const
    {
        return {x * a, y * a};
    }

    [[nodiscard]] Vec2 operator/(float a) const
    {
        return {x / a, y / a};
    }

    [[nodiscard]] Vec2 operator+(const Vec2& a) const
    {
        return {x + a.x, y + a.y};
    }

    [[nodiscard]] Vec2 operator-(const Vec2& a) const
    {
        return {x - a.x, y - a.y};
    }

    Vec2& operator+=(const Vec2& a)
    {
        x += a.x;
        y += a.y;

        return *this;
    }

    Vec2& operator-=(const Vec2& a)
    {
        x -= a.x;
        y -= a.y;

        return *this;
    }

    Vec2& operator/=(const Vec2& a)
    {
        x /= a.x;
        y /= a.y;

        return *this;
    }

    Vec2& operator/=(float a)
    {
        x /= a;
        y /= a;

        return *this;
    }

    Vec2& operator*=(float a)
    {
        x *= a;
        y *= a;

        return *this;
    }

    [[nodiscard]] std::string toString() const
    {
        return std::format("X: {0:.4f}, Y: {1:.4f}", x, y);
    };

    [[nodiscard]] static Vec2 add(const Vec2& a, const Vec2& b)
    {
        return {a.x + b.x, a.y + b.y};
    }

    [[nodiscard]] static Vec2 add(const Vec2& a, float value)
    {
        return {a.x + value, a.y + value};
    }

    [[nodiscard]] static Vec2 sub(const Vec2& a, const Vec2& b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    [[nodiscard]] static Vec2 sub(const Vec2& a, float value)
    {
        return {a.x - value, a.y - value};
    }

    [[nodiscard]] static Vec2 mul(const Vec2& a, const Vec2& b)
    {
        return {a.x * b.x, a.y * b.y};
    }

    [[nodiscard]] static Vec2 mul(const Vec2& a, float scale)
    {
        return {a.x * scale, a.y * scale};
    }

    [[nodiscard]] static Vec2 div(const Vec2& a, const Vec2& b)
    {
        return {a.x / b.x, a.y / b.y};
    }

    [[nodiscard]] static Vec2 div(const Vec2& a, float scale)
    {
        return {a.x / scale, a.y / scale};
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec)
    {
        os << vec.toString();
        return os;
    };
};
