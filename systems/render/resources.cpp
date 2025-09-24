#include "resources.h"

#include <SDL3/SDL_filesystem.h>

#include <glad/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

void ResourceManager::loadTexture(const std::string& id, const std::string& path)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    Texture texture{textureId};

    std::string filePath = SDL_GetBasePath();
    filePath += path;

    std::cout << "Loading " << filePath << "\n";

    int width, height, nrChannels;

    uint8_t* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    texture.generate(width, height, data);

    stbi_image_free(data);

    m_textures[id] = texture;
}

Texture ResourceManager::getTexture(const std::string& id)
{
    assert(m_textures.contains(id));

    return m_textures[id];
}

void ResourceManager::loadShader(
    const std::string& id,
    const char* vertex,
    const char* fragment,
    const char* geometry
)
{
    Shader shader;
    shader.compile(vertex, fragment, geometry);

    m_shaders[id] = shader;
}

Shader ResourceManager::getShader(const std::string& id)
{
    assert(m_shaders.contains(id));

    return m_shaders[id];
};
