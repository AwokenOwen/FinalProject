#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;

in vec2 TexCoords;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{
    vec4 base = texture2D(textures[0], TexCoords);
    // Do stuff



    FragColor = base;
}