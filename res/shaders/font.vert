#version 430 core

layout (location = 0) in vec4 a_position; // <vec2 pos, vec2 tex>

uniform ivec2 s_size;

out vec2 tex_coord;

void main() {
    float x = a_position.x * 2 / s_size.x - 1.f;
    float y = a_position.y * 2 / s_size.y - 1.f;
    gl_Position = vec4(x, y, -1.f, 1.f);

    tex_coord = a_position.zw;
}