#version 430 core

in vec2 tex_coord;

uniform sampler2D text;
uniform vec3 text_color;

out vec4 frag_color;

void main() {    
    frag_color = vec4(text_color, texture(text, tex_coord).r);
}