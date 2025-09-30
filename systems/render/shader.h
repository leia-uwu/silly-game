#pragma once

#include "systems/math/matrix.h"
#include "systems/math/vec2.h"

#include <glad/gl.h>

class Shader
{
public:
    GLuint id;

    Shader& use();

    void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource);

    void setFloat(const char* name, float value) const;
    void setInt(const char* name, GLint value) const;
    void setVec2(const char* name, const Vec2& vec) const;
    void setVec2(const char* name, float x, float y) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setVec4(const char* name, float x, float y, float z, float w) const;
    void setMatrix3(const char* name, const Matrix3x3& matrix) const;
};
