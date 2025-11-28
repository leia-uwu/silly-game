#version 300 es

precision highp float;

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_textureCord;
layout (location = 2) in uint a_color;

out vec4 v_color;
out vec2 v_textureCord;

uniform mat3 u_transform;

void main()
{
    float r = float((a_color >> 24) & 255u) / 255.0;
    float g = float((a_color >> 16) & 255u) / 255.0;
    float b = float((a_color >> 8 ) & 255u) / 255.0;
    float a = float((a_color      ) & 255u) / 255.0;

    v_color = vec4(r, g, b, a);

    v_textureCord = a_textureCord;
    gl_Position = mat4(u_transform) * vec4(a_pos.x, a_pos.y, 0.0, 1.0);
}
