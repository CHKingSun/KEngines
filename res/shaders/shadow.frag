#version 430 core

struct Texture {
    sampler2D tex;
    int type;
};
const int MAX_TEXTURES_NUM = 9;

struct BasicLight {
    vec3 ambient;
    float intensity;
    bool enable;
};

struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    bool enable;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float kq, kl, kc;
    bool enable;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float kq, kl, kc;
    float inner_cutoff, outer_cutoff;
    bool enable;
};
const int MAX_LIGHTS_NUM = 6;

void dealBasicLight(const in BasicLight light, inout vec3 ambient) {
    ambient += light.ambient * light.intensity;
}

void dealDirectionLight(const in DirectionLight light, const in float u_shininess, const in vec3 N, const in vec3 E,
                       inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
    vec3 L = normalize(-light.direction);
    float cosT = max(dot(L, N), 0.f);
    float cosA = 0.f;
    if(cosT != 0.f) cosA = max(dot(N, normalize(L + E)), 0.f);

    ambient += light.ambient * light.intensity;
    diffuse += light.diffuse * light.intensity * cosT;
    specular += light.specular * light.intensity * pow(cosA, u_shininess);
}

void dealPointLight(const in PointLight light, const in float u_shininess, const in vec3 N, const in vec3 E,
                    const in vec3 world_pos, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
    float dis = length(light.position - world_pos);
    float attenuation = 1.f / (light.kc + light.kl * dis + light.kq * dis * dis); 

    vec3 L = normalize(light.position - world_pos);
    float cosT = max(dot(L, N), 0.f);
    float cosA = 0.f;
    if(cosT != 0.f) cosA = max(dot(N, normalize(L + E)), 0.f);

    attenuation *= light.intensity;
    ambient += light.ambient * attenuation;
    diffuse += light.diffuse * attenuation * cosT;
    specular += light.specular * attenuation * pow(cosA, u_shininess);
}

void dealSpotLight(const in SpotLight light, const in float u_shininess, const in vec3 N, const in vec3 E,
                    const in vec3 world_pos, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
    float dis = length(light.position - world_pos);
    float attenuation = 1.f / (light.kc + light.kl * dis + light.kq * dis * dis); 

    vec3 L = normalize(light.position - world_pos);
    float intensity = smoothstep(light.outer_cutoff, light.inner_cutoff, dot(normalize(light.direction), -L));
    if(intensity == 0.f) return;
    float cosT = max(dot(L, N), 0.f);
    float cosA = 0.f;
    if(cosT != 0.f) cosA = max(dot(N, normalize(L + E)), 0.f);

    attenuation *= light.intensity * intensity;
    ambient += light.ambient * attenuation;
    diffuse += light.diffuse * attenuation * cosT;
    specular += light.specular * attenuation * pow(cosA, u_shininess);
}

vec4 dealShadowCoord(const in vec4 lgiht_world_space) {
    return (lgiht_world_space + 1.f) / 2.f;
}

in V_OUT {
    vec2 v_tex_coord;
    vec3 v_N;
    vec3 v_E;
    vec3 v_world_pos;
    vec4 v_light_world_pos;
} fs_in;

out vec4 frag_color;

uniform BasicLight u_bLights[MAX_LIGHTS_NUM];
uniform DirectionLight u_dLights[MAX_LIGHTS_NUM];
uniform PointLight u_pLights[MAX_LIGHTS_NUM];
uniform SpotLight u_sLights[MAX_LIGHTS_NUM];

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;
uniform float u_opacity;

uniform float u_depth;

uniform Texture u_textures[MAX_TEXTURES_NUM];
uniform sampler2DShadow u_shadow_map;

void main() {
    vec4 m_ambient = vec4(0.f);
    vec4 m_diffuse = vec4(0.f);
    vec4 m_specular = vec4(0.f);

    bool flag[] = bool[3]( false, false, false );
    for(int i = 0; i < MAX_TEXTURES_NUM; ++i) {
        if(u_textures[i].type == 1) {
            flag[0] = true;
            m_ambient += texture(u_textures[i].tex, fs_in.v_tex_coord);
        } else if (u_textures[i].type == 2) {
            flag[1] = true;
            m_diffuse += texture(u_textures[i].tex, fs_in.v_tex_coord);
        } else if (u_textures[i].type == 3) {
            flag[2] = true;
            m_specular += texture(u_textures[i].tex, fs_in.v_tex_coord);
        }
    }

    if(flag[0]) m_ambient *= vec4(u_ambient, u_opacity);
    else m_ambient = vec4(u_ambient, u_opacity);
    if(flag[1]) m_diffuse *= vec4(u_diffuse, u_opacity);
    else m_diffuse = vec4(u_diffuse, u_opacity);
    if(flag[2]) m_specular *= vec4(u_specular, u_opacity);
    else m_specular = vec4(u_specular, u_opacity);

    vec3 l_ambient = vec3(0.f);
    vec3 l_diffuse = vec3(0.f);
    vec3 l_specular = vec3(0.f);

    for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
        if(u_bLights[i].enable) {
            dealBasicLight(u_bLights[i], l_ambient);
        }
        if(u_dLights[i].enable) {
            dealDirectionLight(u_dLights[i], u_shininess, fs_in.v_N, fs_in.v_E,
                               l_ambient, l_diffuse, l_specular);
        }
        if(u_pLights[i].enable) {
            dealPointLight(u_pLights[i], u_shininess, fs_in.v_E, fs_in.v_E, fs_in.v_world_pos,
                           l_ambient, l_diffuse, l_specular);
        }
        if(u_sLights[i].enable) {
            dealSpotLight(u_sLights[i], u_shininess, fs_in.v_E, fs_in.v_E, fs_in.v_world_pos,
                           l_ambient, l_diffuse, l_specular);
        }
    }

    vec4 shadow_coord = fs_in.v_light_world_pos;
    shadow_coord.z -= 0.0014f;
    float depth = textureProj(u_shadow_map, dealShadowCoord(shadow_coord));
    frag_color = m_ambient * vec4(l_ambient, 1.f)
               + m_diffuse *  vec4(l_diffuse * depth, 1.f)
               + m_specular * vec4(l_specular * depth, 1.f);
}