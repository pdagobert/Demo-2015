#version 330 core

uniform sampler2D tex;

in vec2 TexCoords;
flat in float alpha;

out vec4 Color;

void main()
{
	vec3 c = texture( tex, TexCoords ).rgb;
	
	Color = vec4( c * alpha, 1.0 );
}