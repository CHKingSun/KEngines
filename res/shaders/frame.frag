#version 430 core

in vec2 v_tex_coord;
in float v_opacity;

uniform sampler2D u_texture;

out vec4 frag_color;

void main() {
    frag_color = texture(u_texture, v_tex_coord) * vec4(1.f, 1.f, 1.f, v_opacity);
}


