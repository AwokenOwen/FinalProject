#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 color;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    gl_FragColor = vec4(color, 1.0) * sampled;
}  