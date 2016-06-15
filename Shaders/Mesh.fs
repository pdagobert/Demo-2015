#version 330 core

uniform sampler2D tex;
uniform vec3 lightDir;

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;

out vec4 Color;

void main()
{
	vec3 texColor = texture( tex, TexCoords ).rgb;

	float diffuseFactor = max( dot( normalize( Normal ), lightDir ), 0.0 );

	vec3 c = texColor * diffuseFactor;
	Color = vec4( c, 1.0 );
}
