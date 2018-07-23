#version 430 core

layout(location = 0) in vec4 a_position;

uniform float u_opacity;

out vec2 v_tex_coord;
out float v_opacity;

void main() {
    if(gl_VertexID < 6) {
        gl_Position = vec4(a_position.xy, 0.5f, 1.f);
        v_opacity = 1.f;
    } else {
        gl_Position = vec4(a_position.xy, 0.f, 1.f);
        v_opacity = u_opacity;
    }

    v_tex_coord = a_position.zw;
}