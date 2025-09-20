#pragma once

#include <cstdint>

class Color
{
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;

    Color() :
        r(255),
        g(255),
        b(255)
    {
    }

    Color(uint32_t hex) :
        r((hex >> 16) & 0xFF),
        g((hex >> 8) & 0xFF),
        b((hex) & 0xFF)
    {
    }

    [[nodiscard]] uint32_t RGBHex() const
    {
        return r | (g << 8) | (b << 16);
    }

    [[nodiscard]] uint32_t RGBAHex() const
    {
        return r | (g << 8) | (b << 16) | (a << 24);
    }

    //
    // for values between 0 and 1
    //

    [[nodiscard]] float normalizedR() const
    {
        return r / 255.F;
    }

    [[nodiscard]] float normalizedG() const
    {
        return g / 255.F;
    }

    [[nodiscard]] float normalizedB() const
    {
        return b / 255.F;
    }

    [[nodiscard]] float normalizedA() const
    {
        return a / 255.F;
    }
};
