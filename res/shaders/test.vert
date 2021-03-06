#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;

//row_major?
uniform mat4 u_model_matrix;
uniform mat3 u_normal_matrix;

uniform mat4 u_proj_matrix;

out vec3 v_color;

void main() {
    gl_Position = u_proj_matrix * (u_model_matrix * vec4(a_position, 1.f));

    v_color = (normalize(u_normal_matrix * a_normal) + vec3(1.f)) * 0.5f; 
    // v_color = abs(normalize(u_normal_matrix * a_normal));
}