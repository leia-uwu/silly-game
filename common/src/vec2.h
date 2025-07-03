#pragma once

#include <string>

#define VEC2_EPSILON 0.001F

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
    Vec2() noexcept;
    Vec2(float xPos) noexcept;
    Vec2(float xPos, float yPos) noexcept;

    [[nodiscard]] Vec2 clone() const noexcept;

    void set(float xPos, float yPos);

    [[nodiscard]] bool is_valid() const;

    Vec2& add(float value);
    Vec2& add(const Vec2& a);
    Vec2& sub(float value);
    Vec2& sub(const Vec2& a);
    Vec2& mul(float scale);
    Vec2& mul(const Vec2& a);
    Vec2& div(float scale);
    Vec2& div(const Vec2& a);
    Vec2& rotate(float rad);
    Vec2& invert();
    Vec2& perp();
    Vec2& normalize();
    Vec2& normalize_safe(const Vec2& v = {1.0, 0.0});

    [[nodiscard]] float length_sqr() const;
    [[nodiscard]] float length() const;
    [[nodiscard]] float distance_to(const Vec2& a) const;
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

    std::string to_string();

    static Vec2 add(const Vec2& a, const Vec2& b);
    static Vec2 add(const Vec2& a, float value);
    static Vec2 sub(const Vec2& a, const Vec2& b);
    static Vec2 sub(const Vec2& a, float value);
    static Vec2 mul(const Vec2& a, const Vec2& b);
    static Vec2 mul(const Vec2& a, float scale);
    static Vec2 div(const Vec2& a, const Vec2& b);
    static Vec2 div(const Vec2& a, float scale);
};
