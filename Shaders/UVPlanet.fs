#version 330 core

uniform sampler2D tex;
uniform vec3 lightDir;
uniform vec3 cameraPos;
uniform vec3 atmoColor;

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;

out vec4 Color;

void main()
{
	vec3 texColor = texture( tex, TexCoords ).rgb;

	float diffuseFactor = max( dot( normalize( Normal ), lightDir ), 0.0 );
	
	vec3 fragToCam = normalize( cameraPos - Position );
	
	float atmosphereFactor = max( dot( fragToCam, normalize( Normal ) ), 0.0 );
	
	vec3 c = mix( atmoColor, texColor, atmosphereFactor );
	
	Color = vec4( c * diffuseFactor, 1.0 );
}
