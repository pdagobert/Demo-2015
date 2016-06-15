#version 330

uniform sampler2D tex;
uniform vec3 lightDir;

in vec2 TexCoords;
in vec3 Normal;

out vec4 Color;

void main()
{
	float diffuse = max( dot( normalize( Normal ), lightDir ), 0.0 );
	Color = vec4( texture( tex, TexCoords ).rgb * diffuse, 1.0 );
}