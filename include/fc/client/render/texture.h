/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

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

    Texture();
    Texture(const Texture& other) = delete;
    Texture operator=(const Texture& other) = delete;

    void generate(GLuint width, GLuint height, uint8_t* data);

    void bind() const;

    ~Texture();
};
