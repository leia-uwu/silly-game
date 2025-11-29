#pragma once

#include <cmath>
#include <cstdint>
#include <format>
#include <ostream>
#include <string>

#define VEC2_EPSILON 0.001F

template<typename VecT>
class Vec2
{
public:
    VecT x;
    VecT y;

    Vec2() : x(0), y(0)
    {
    }

    Vec2(VecT xAndY) : x(xAndY), y(xAndY)
    {
    }

    Vec2(VecT xPos, VecT yPos) : x(xPos), y(yPos)
    {
    }

    [[nodiscard]] Vec2 clone() const
    {
        return {x, y};
    }

    void set(VecT xPos, VecT yPos)
    {
        x = xPos;
        y = yPos;
    }

    [[nodiscard]] bool isValid() const
    {
        return std::isfinite(x) && std::isfinite(y);
    };

    Vec2& add(VecT value)
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

    Vec2& sub(VecT value)
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

    Vec2& scale(VecT scale)
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

    Vec2& div(VecT scale)
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

    Vec2& rotate(VecT rad)
    {
        const VecT cosr = std::cos(rad);
        const VecT sinr = std::sin(rad);

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
        VecT oldX = x;
        x = -y;
        y = oldX;
        return *this;
    }

    Vec2& normalize()
    {
        const VecT len = length();

        if (len > VEC2_EPSILON) {
            x /= len;
            y /= len;
        }

        return *this;
    }

    Vec2& normalize(VecT length)
    {
        if (length > VEC2_EPSILON) {
            x /= length;
            y /= length;
        }

        return *this;
    }

    Vec2& normalizeSafe(const Vec2& v = {1.0, 0.0})
    {
        const VecT len = length();

        if (len > VEC2_EPSILON) {
            x /= len;
            y /= len;
        } else {
            x = v.x;
            y = v.y;
        }

        return *this;
    }

    [[nodiscard]] VecT lengthSqr() const
    {
        return x * x + y * y;
    }

    [[nodiscard]] VecT length() const
    {
        return std::sqrt(lengthSqr());
    }

    [[nodiscard]] VecT distanceTo(const Vec2& a) const
    {
        return Vec2::sub(*this, a).length();
    }

    [[nodiscard]] VecT dot(const Vec2& a) const
    {
        return x * a.x + y * a.y;
    }

    [[nodiscard]] bool equals(const Vec2& a) const
    {
        return x == a.x && y == a.y;
    }

    [[nodiscard]] bool equals(const Vec2& a, VecT epsilon) const
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

    [[nodiscard]] VecT operator[](int index) const
    {
        return (&x)[index];
    }

    VecT& operator[](int index)
    {
        return (&x)[index];
    }

    [[nodiscard]] Vec2 operator-() const
    {
        return {-x, -y};
    }

    VecT operator*(const Vec2& a) const
    {
        return x * a.x + y * a.y;
    }

    [[nodiscard]] Vec2 operator*(VecT a) const
    {
        return {x * a, y * a};
    }

    [[nodiscard]] Vec2 operator/(VecT a) const
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

    Vec2& operator/=(VecT a)
    {
        x /= a;
        y /= a;

        return *this;
    }

    Vec2& operator*=(VecT a)
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

    [[nodiscard]] static Vec2 add(const Vec2& a, VecT value)
    {
        return {a.x + value, a.y + value};
    }

    [[nodiscard]] static Vec2 sub(const Vec2& a, const Vec2& b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    [[nodiscard]] static Vec2 sub(const Vec2& a, VecT value)
    {
        return {a.x - value, a.y - value};
    }

    [[nodiscard]] static Vec2 mul(const Vec2& a, const Vec2& b)
    {
        return {a.x * b.x, a.y * b.y};
    }

    [[nodiscard]] static Vec2 mul(const Vec2& a, VecT scale)
    {
        return {a.x * scale, a.y * scale};
    }

    [[nodiscard]] static Vec2 div(const Vec2& a, const Vec2& b)
    {
        return {a.x / b.x, a.y / b.y};
    }

    [[nodiscard]] static Vec2 div(const Vec2& a, VecT scale)
    {
        return {a.x / scale, a.y / scale};
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec)
    {
        os << vec.toString();
        return os;
    };
};

using Vec2F = Vec2<float>;
using Vec2U = Vec2<uint32_t>;
