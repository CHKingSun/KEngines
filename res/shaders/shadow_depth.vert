#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 3) in vec3 a_location;

uniform mat4 u_model_matrix;
uniform mat4 u_light_matrix;

void main() {
    gl_Position = u_light_matrix * u_model_matrix * vec4(a_position + a_location, 1.f);
}