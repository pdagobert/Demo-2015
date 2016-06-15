#version 330 core

#include "Noise/noise4D.glsl"

uniform float time;

in vec3 NormalLocalSpace;

out vec4 Color;

float fbm( vec4 vec, int octaves, float frequency, float persistence )
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

float _fbm( vec4 vec, int octaves, float frequency, float persistence )
{
	float value = 0.0;
	float amplitude = 1.0;
	
	float maxAmplitude = 0.0;
	
	for( int i = 0; i < octaves; i++ )
	{
		float n = snoise( vec * frequency );
		value += 1.0 - abs( n ) * amplitude;
		frequency *= 2.0;
		
		maxAmplitude += amplitude;
		amplitude *= persistence;	
	}
	
	return value / maxAmplitude;
}

vec3 getColor( float f )
{
	vec3 c1 = vec3( 1.0, 1.0, 0.0 );
	vec3 c2 = vec3( 1.0, 0.2, 0.0 );
	vec3 c3 = vec3( 0.0 );
	
	if( f > 0.5 )
	{
		float c = ( f - 0.5 ) * 2.0;
		return mix( c2, c3, c );
	}
	else
	{
		float c = f * 2.0;
		return mix( c1, c2, c );
	}
}

void main()
{
	vec3 normal = normalize( NormalLocalSpace );
	
	float n = fbm( vec4( normal, time / 5.0 ), 4, 4.0, 0.7 ) * 2.0;
	float n2 = _fbm( vec4( normal, time / 5.0 ), 4, 1.0, 0.7 ) * 2.0;
	float n3 = fbm( vec4( normal, time / 5.0 ), 4, 8.0, 0.7 ) * 2.0;
	float c = ( n + n2 + n3 ) / 6;
	
	Color = vec4( vec3( getColor( c ) ), 1.0 );
}