#version 430 core

layout (location = 0) in vec4 a_position; // <vec2 pos, vec2 tex>

uniform ivec2 s_size;

out vec2 tex_coord;

void main() {
    float x = a_position.x / s_size.x - 0.5f;
    float y = a_position.y / s_size.y - 0.5f;
    gl_Position = vec4(x, y, 0.0, 1.0);

    tex_coord = a_position.zw;
}