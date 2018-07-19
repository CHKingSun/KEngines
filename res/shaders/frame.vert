#version 430 core

layout(location = 0) in vec4 a_position;

out vec2 v_tex_coord;

void main() {
    gl_Position = vec4(a_position.xy, 0.f, 1.f);

    v_tex_coord = a_position.zw;
}