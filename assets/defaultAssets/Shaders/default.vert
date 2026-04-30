#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

// change to number of textures needed for object
// Make sure it's changed in fragment as well
#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = vec3(transpose(inverse(model)) * vec4(aNormal, 1.0)); 
    vec4 pos = projection * view * model * vec4(aPos, 1.0);

    vec3 WOP = vec3(0.0);

    WorldPos += WOP;
    pos += vec4(WOP, 0.0);
    gl_Position = pos;
}