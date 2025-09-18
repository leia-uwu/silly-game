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
};
