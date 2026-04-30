#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// change to number of textures needed for object
// Make sure it's changed in vertex as well
#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

//Skybox
uniform samplerCube skybox;
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D BRDFMap;

// lights
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform float dirLightPow;

#define NUM_LIGHTS 20
uniform vec3 lightPositions[NUM_LIGHTS];
uniform vec3 lightColors[NUM_LIGHTS];
uniform float lightPowers[NUM_LIGHTS];

uniform vec3 camPos;

const float PI = 3.14159265359;

vec3 CreateMaterial(vec3 _albedo, vec3 _normal, float _metallic, float _roughness, float _ao, vec3 _emission);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 getNormalFromMap(sampler2D _normalMap);
vec3 CalcDirectionalLight(vec3 albedo, float metallic, float roughness, vec3 N, vec3 V, vec3 L, vec3 F0);
vec3 CalcOtherLight(vec3 albedo, float metallic, float roughness, vec3 N, vec3 V, vec3 L, vec3 F0, vec3 lightColor, float lightPower);

void main() {
    // Default Values can be removed
    vec3 albedo     = vec3(1.0);
    vec3 normal     = normalize(Normal);
    float metallic  = 0.0;
    float roughness = 1.0;
    float ao        = 1.0;
    vec3 emission = vec3(0.0);
    // Do calculations here to customize input values





    // Running Normal Lighting Calculations
    albedo = pow(albedo, vec3(2.2));
    vec3 color = CreateMaterial(albedo, normal, metallic, roughness, ao, emission);

    // Setting Color
    FragColor = vec4(color, 1.0);
    //FragColor = vec4(lightPositions[0], 1.0);
}

vec3 CreateMaterial(vec3 _albedo, vec3 _normal, float _metallic, float _roughness, float _ao, vec3 _emission){
    vec3 albedo     = _albedo;
    float metallic  = _metallic;
    float roughness = _roughness;
    float ao        = _ao;
    vec3 emission   = _emission;

    vec3 N = _normal;
    vec3 V = normalize(camPos - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    //Directional Light
    Lo += CalcDirectionalLight(albedo, metallic, roughness, N, V, dirLightDir, F0);

    //All other lights
    for(int i = 0; i < NUM_LIGHTS; ++i)
    {
        Lo += CalcOtherLight(albedo, metallic, roughness, N, V, lightPositions[i], F0, lightColors[i], lightPowers[i]);
    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = textureCube(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilteredMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF  = texture2D(BRDFMap, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = Lo + ambient + emission;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    return color;
}
float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 getNormalFromMap(sampler2D _normalMap){
    vec3 tangentNormal = texture2D(_normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
vec3 CalcDirectionalLight(vec3 albedo, float metallic, float roughness, vec3 N, vec3 V, vec3 L, vec3 F0){
    vec3 H = normalize(V + L);
    float attenuation = dirLightPow;
    vec3 radiance     = dirLightColor * attenuation;

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
    //return (numerator);
}
vec3 CalcOtherLight(vec3 albedo, float metallic, float roughness, vec3 N, vec3 V, vec3 lightPosition, vec3 F0, vec3 lightColor, float lightPower){
    // calculate per-light radiance
    vec3 L = normalize(lightPosition - WorldPos);
    vec3 H = normalize(V + L);
    float distance    = length(lightPosition - WorldPos);
    float attenuation = lightPower / (distance * distance);
    vec3 radiance     = lightColor * attenuation;

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}
