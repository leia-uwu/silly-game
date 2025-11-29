#include "render/resources.h"

#include <SDL3/SDL_filesystem.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

static GLuint getFormat(int channels)
{
    GLuint format = GL_RGB;

    switch (channels) {
    case 1:
        return GL_LUMINANCE;
    case 2:
        // no idea if this one is correct lol
        // idk if theres a better way to translate the stbi channels to opengl formats
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        std::cerr << "Unknown image format, channels: " << channels << "\n";
        return GL_RGBA;
    }
}

#ifndef __EMSCRIPTEN__

void ResourceManager::loadTexture(const char* id, const char* path)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    Texture texture{textureId};

    std::string filePath = SDL_GetBasePath();
    filePath += path;

    std::cout << "Loading " << filePath << "\n";

    int width;
    int height;
    int nrChannels;
    uint8_t* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    GLuint format = getFormat(nrChannels);

    texture.imageFormat = format;
    texture.generate(width, height, data);

    stbi_image_free(data);

    m_textures[id] = texture;
}

#else

// for emscripten...
struct LoadCTX
{
    const char* path;
    const char* id;
    std::unordered_map<std::string, Texture>& textures;
    GLuint texture;
};

void ResourceManager::loadTexture(const char* id, const char* path)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    Texture texture{textureId};

    auto* ctx = new LoadCTX{
        .path = path,
        .id = id,
        .textures = m_textures,
        .texture = textureId,
    };

    std::cout << "Loading " << path << "\n";

    // fetch the texture asynchronously and update it after its done
    emscripten_async_wget_data(
        path,
        ctx,
        [](void* userData, void* buffer, int size) {
            auto* ctx = (LoadCTX*)userData;

            int width;
            int height;
            int nrChannels;
            // TODO: find a way to decode the image using js built in stuff instead of stb
            uint8_t* data = stbi_load_from_memory((uint8_t*)buffer, size, &width, &height, &nrChannels, 0);

            Texture& texture = ctx->textures[ctx->id];

            GLuint format = getFormat(nrChannels);

            // for webgl both need to be the same...?
            // doing this for native breaks
            texture.imageFormat = texture.internalFormat = format;
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
