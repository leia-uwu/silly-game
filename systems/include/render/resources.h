#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

#include "render/shader.h"
#include "render/texture.h"

class ResourceManager
{
public:
    void loadTexture(const std::string& id, const std::string& path);
    [[nodiscard]] Texture getTexture(const std::string& id);

    void loadShader(
        const std::string& id,
        const char* vertex,
        const char* fragment,
        const char* geometry
    );

    [[nodiscard]] Shader getShader(const std::string& id);

private:
    std::unordered_map<std::string, Texture> m_textures;
    std::unordered_map<std::string, Shader> m_shaders;
};
