#include "render/resources.h"

#include <SDL3/SDL_filesystem.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

#ifndef __EMSCRIPTEN__

void ResourceManager::loadTexture(const char* id, const char* path)
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

#else

// for emscripten...
struct LoadCTX
{
    const char* path;
    GLuint texture;
};

void ResourceManager::loadTexture(const char* id, const char* path)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    Texture texture{textureId};

    auto* ctx = new LoadCTX{
        .path = path,
        .texture = textureId
    };

    std::cout << "Loading " << path << "\n";

    // fetch the texture asynchronously and update it after its done
    emscripten_async_wget_data(
        path,
        ctx,
        [](void* userData, void* buffer, int size) {
            auto* ctx = (LoadCTX*)userData;

            int width, height, nrChannels;
            // TODO: find a way to decode the image using js built in stuff instead of stb
            uint8_t* data = stbi_load_from_memory((uint8_t*)buffer, size, &width, &height, &nrChannels, 0);

            Texture texture{ctx->texture};

            texture.generate(width, height, data);

            std::cout << "Loaded " << ctx->path << "\n";

            delete ctx;
        },
        [](void* data) {
            auto* ctx = (LoadCTX*)data;

            std::cerr << "Error loading " << ctx->path << "\n";

            delete ctx;
        }
    );

    m_textures[id] = texture;
}

#endif

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
