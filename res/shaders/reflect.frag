#version 430 core

in V_OUT {
    vec3 v_N;
    vec3 v_E;
} fs_in;

uniform samplerCube u_cube_tex;

out vec4 frag_color;

void main() {
    vec3 dir = reflect(fs_in.v_E, fs_in.v_N);
    // vec3 dir = refract(fs_in.v_E, fs_in.v_N, 1.52f);

    frag_color = texture(u_cube_tex, dir);
}