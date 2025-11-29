#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

#include "render/shader.h"
#include "render/texture.h"

class ResourceManager
{
public:
    void loadTexture(const char* id, const char* path);

    [[nodiscard]] Texture getTexture(const std::string& id);

private:
    std::unordered_map<std::string, Texture> m_textures;
};
