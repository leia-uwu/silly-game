#pragma once

#include <glad/gl.h>

#include <cstdint>

class Texture
{
public:
    GLuint id;

    uint32_t width;
    uint32_t height;

    uint32_t internalFormat;
    uint32_t imageFormat;

    uint32_t wrapS;
    uint32_t wrapT;
    uint32_t filterMin;
    uint32_t filterMax;

    Texture(uint32_t id);
    Texture() = default;

    void generate(uint32_t width, uint32_t height, uint8_t* data);

    void bind() const;
};
