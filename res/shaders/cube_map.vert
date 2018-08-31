#version 430 core

layout(location = 0) in vec3 a_position;

out vec3 v_tex_coord;

uniform mat3 u_rotation;
uniform mat4 u_projection;

void main() {
    v_tex_coord = u_rotation * a_position;

    gl_Position = (u_projection * vec4(a_position, 1.f)).xyzz;
}