#version 330 core
#extension GL_NV_shadow_samplers_cube : enable

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

uniform vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
