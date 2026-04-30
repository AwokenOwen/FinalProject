#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
layout (location = 0) in vec2 aPos;

out vec2 TexCoords;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = (aPos.xy + 1.0) / 2.0;
}