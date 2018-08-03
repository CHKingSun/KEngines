#version 430 core

struct Texture {
    sampler2D tex;
    int type;
};
const int MAX_TEXTURE_NUM = 6;

in vec2 v_tex_coord;

out vec4 frag_color;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;
uniform float u_opacity;

uniform Texture u_textures[MAX_TEXTURE_NUM];

void main() {
    //Just a test shader.
    vec4 ambient = vec4(0.f);
    vec4 diffuse = vec4(0.f);
    vec4 specular = vec4(0.f);

    for(int i = 0; i < MAX_TEXTURE_NUM; ++i) {
        if(u_textures[i].type == 1) {
            ambient += texture(u_textures[i].tex, v_tex_coord);
        } else if (u_textures[i].type == 2) {
            diffuse += texture(u_textures[i].tex, v_tex_coord);
        } else if (u_textures[i].type == 3) {
            specular += texture(u_textures[i].tex, v_tex_coord);
        }
    }

    if(ambient == vec4(0.f)) ambient = vec4(0.8f, 0.8f, 0.8f, 1.f);
    if(diffuse == vec4(0.f)) diffuse = vec4(0.8f, 0.8f, 0.8f, 1.f);
    if(specular == vec4(0.f)) specular = vec4(0.8f, 0.8f, 0.8f, 1.f);

    frag_color = ambient * vec4(u_ambient, u_opacity) * 0.6f
               + diffuse * vec4(u_diffuse, u_opacity) * 0.2f
               + specular * vec4(u_specular, u_opacity) * pow(0.2f, u_shininess);

    // frag_color = diffuse * vec4(u_diffuse, u_opacity) * 0.8f
    //            + specular * vec4(u_specular, u_opacity) * pow(0.2f, u_shininess);
}