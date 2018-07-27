#version 430 core

in vec3 v_color;

out vec4 frag_color;

void main() {
    if(v_color == vec3(0.f)) frag_color = vec4(0.8f, 0.8f, 0.8f, 1.f);
	else frag_color = vec4(v_color, 1.f);
}