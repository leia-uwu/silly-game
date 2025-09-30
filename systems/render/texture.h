#pragma once

#include <glad/gl.h>

#include <cstdint>

class Texture
{
public:
    GLuint id;

    GLuint width;
    GLuint height;

    GLuint internalFormat;
    GLuint imageFormat;

    GLuint wrapS;
    GLuint wrapT;
    GLuint filterMin;
    GLuint filterMax;

    Texture(GLuint id);
    Texture() = default;

    void generate(GLuint width, GLuint height, uint8_t* data);

    void bind() const;
};
