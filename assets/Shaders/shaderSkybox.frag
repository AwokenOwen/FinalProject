#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 zenithColor;
uniform vec3 horizonColor;
uniform vec3 camPos;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{
	FragColor = vec4(vec3(0, 0.024, 0.102), 1.0);
}