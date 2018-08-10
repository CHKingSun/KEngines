#version 430 core

struct Texture {
    sampler2D tex;
    int type;
};
const int MAX_TEXTURE_NUM = 6;

in vec2 v_tex_coord;
in vec3 v_N;
in vec3 v_E;

out vec4 frag_color;

vec3 light_ambient = vec3(0.8f, 0.8f, 0.8f);
vec3 light_diffuse = vec3(0.8f, 0.8f, 0.8f);
vec3 light_specular = vec3(0.8f, 0.8f, 0.8f);
vec3 light_direction = vec3(-3.f, 3.f, 1.f);

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

    bool flag[] = bool[3]( false, false, false );
    for(int i = 0; i < MAX_TEXTURE_NUM; ++i) {
        if(u_textures[i].type == 1) {
            flag[0] = true;
            ambient += texture(u_textures[i].tex, v_tex_coord);
        } else if (u_textures[i].type == 2) {
            flag[1] = true;
            diffuse += texture(u_textures[i].tex, v_tex_coord);
        } else if (u_textures[i].type == 3) {
            flag[2] = true;
            specular += texture(u_textures[i].tex, v_tex_coord);
        }
    }

    if(flag[0]) ambient *= vec4(u_ambient, u_opacity);
    else ambient = vec4(u_ambient, u_opacity);
    if(flag[1]) diffuse *= vec4(u_diffuse, u_opacity);
    else diffuse = vec4(u_diffuse, u_opacity);
    if(flag[2]) specular *= vec4(u_specular, u_opacity);
    else specular = vec4(u_specular, u_opacity);

    vec3 L = normalize(light_direction);
    float cosT = max(dot(L, v_N), 0.f);
    float cosA = 0.f;
    if(cosT != 0.f) cosA = max(dot(v_N, normalize(L + v_E)), 0.f);

    ambient *= vec4(light_ambient, 1.f);
    diffuse *= vec4(light_diffuse, 1.f) * cosT;
    specular *= vec4(light_specular, 1.f) * pow(cosA, u_shininess);

    frag_color = ambient + diffuse + specular;
}