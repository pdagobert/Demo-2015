#version 330 core

#include "Noise/noise3D.glsl"

uniform float time;

uniform float radius;
uniform vec3 origin;
uniform vec3 color;

in vec3 Position;
in vec2 TexCoords;

out vec4 Color;

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

float coronaPerturb()
{
	vec3 vec = vec3( TexCoords, time / 150.0 );
	float n = fbm( vec, 8, 40.0, 0.65 );
	return n;
}

void main()
{
	float dist = distance( Position, origin ) / radius;
	
	float f = 1.0 - dist * dist;
	f = clamp( f, 0.0, 1.0 );
	
	float f2 = 1.0 - dist;
	f2 = clamp( f2, 0.0, 1.0 );
	
	vec3 c2 = vec3( 1.0 );

	/*float t = 0.001;

	float dx = Position.x - origin.x;
	float dy = Position.y - origin.y;
	
	float glow = t / abs( dx ) * t / abs( dy );
	glow *= glow / 100.0;*/
	
	Color = vec4( mix( color, c2, f ), f2 ) + vec4( color, 1.0 ) * f2;
}