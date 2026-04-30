#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
layout (location = 0) in vec3 aPos;

out vec3 localPos;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = aPos;
    gl_Position =  projection * view * vec4(localPos, 1.0);
}