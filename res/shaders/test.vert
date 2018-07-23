#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;

uniform vec3 u_position;
uniform vec3 u_scale;
uniform mat3 u_rotation;

uniform mat4 u_proj;

out vec3 v_color;

void main() {
    gl_Position = u_proj * vec4(u_position + u_rotation * (u_scale * a_position), 1.f);

    v_color = u_rotation * (a_normal / u_scale);
}