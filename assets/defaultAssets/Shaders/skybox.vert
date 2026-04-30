#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  