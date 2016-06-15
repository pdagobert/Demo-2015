#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;
layout( location = 2 ) in vec3 normal;

uniform mat4 wvp;
uniform mat4 world;

uniform float radius;

out vec2 TexCoords;
out vec3 Normal;

#include "Noise/noise3D.glsl"

float fbm( vec3 vec, int octaves, float frequency, float persistence )
{
	float value = 0.0;
	float amplitude = 1.0;
	
	float maxAmplitude = 0.0;
	
	for( int i = 0; i < octaves; i++ )
	{
		value += snoise( vec * frequency ) * amplitude;
		frequency *= 2.0;
		
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}
	
	return value / maxAmplitude;
}

void main()
{
	vec3 pos = normal * ( radius + fbm( normal, 8, 0.4, 0.65 ) * 0.15 );
	gl_Position = wvp * vec4( pos, 1.0 );
	
	TexCoords = texCoords;
	Normal = mat3( transpose( inverse( world ) ) ) * normalize( pos );
}
