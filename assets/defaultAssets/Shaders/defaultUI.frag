#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;

in vec2 TexCoords;

#define NUM_TEXTURES 12
uniform sampler2D textures[NUM_TEXTURES];

void main()
{    
	//FragColor = vec4(1.0);
	FragColor = texture2D(textures[0], normalize(TexCoords));
}