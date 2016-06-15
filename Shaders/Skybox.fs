#version 330 core

uniform samplerCube skybox;
in vec3 TexCoords;

out vec4 Color;

void main()
{
	vec4 color = texture( skybox, TexCoords );
	Color = vec4( color.rgb, 1.0 );
}