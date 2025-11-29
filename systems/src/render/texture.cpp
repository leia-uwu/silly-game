#include "render/texture.h"

Texture::Texture(GLuint id) :
    id(id),
    width(0),
    height(0),
    internalFormat(GL_RGBA),
    imageFormat(GL_RGBA),
    wrapS(GL_REPEAT),
    wrapT(GL_REPEAT),
    filterMin(GL_LINEAR),
    filterMax(GL_LINEAR)
{
}

void Texture::generate(uint32_t width, uint32_t height, unsigned char* data)
{
    this->width = width;
    this->height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

    glGenerateMipmap(GL_TEXTURE_2D);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}
