#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{
	FragColor = textureCube(skybox, normalize(TexCoords));
}