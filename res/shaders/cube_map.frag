#version 430 core

in vec3 v_tex_coord;

out vec4 frag_color;

uniform samplerCube u_cube_tex;

void main() {
    frag_color = texture(u_cube_tex, v_tex_coord);
}