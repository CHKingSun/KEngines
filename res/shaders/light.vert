#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_location;

//row_major?
uniform mat4 u_model_matrix;
uniform mat3 u_normal_matrix;

uniform mat4 u_proj_matrix;
uniform vec3 u_view_pos;

out V_OUT {
    vec2 v_tex_coord;
    vec3 v_N;
    vec3 v_E;
    vec3 v_world_pos;
} fs_out;

void main() {
    vec4 world_pos = u_model_matrix * vec4(a_position + a_location, 1.f);
    gl_Position = u_proj_matrix * world_pos;

    fs_out.v_tex_coord = a_tex_coord;
    fs_out.v_N = normalize(u_normal_matrix * a_normal);
    fs_out.v_E = normalize(u_view_pos - world_pos.xyz);
    fs_out.v_world_pos = world_pos.xyz;
}