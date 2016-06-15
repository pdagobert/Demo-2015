#version 330 core

in vec3 TexCoords;

out vec4 Color;

#include "Noise/noise3D.glsl"

float fbm2( vec3 vec, int octaves, float frequency, float persistence )
{
	float value = 0.0;
	float amplitude = 1.0;
	
	float maxAmplitude = 0.0;
	
	for( int i = 0; i < octaves; i++ )
	{
		float n = ( 1.0 - abs( snoise( vec * frequency ) ) ) * 2.0 - 1.0;
		value += n * amplitude;
		frequency *= 2.0;
		
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}
	
	return value / maxAmplitude;
}

vec4 nebulaColor( float f )
{
	vec3 vec = normalize( TexCoords );
	vec3 c = vec3( 0.0 );

	vec3 white = vec3( 2.0 );
	vec3 purple = vec3( 1.0, 0.0, 1.0 );
	vec3 yellow = vec3( 1.0, 1.0, 0.0 );
	vec3 green = vec3( 0.0, 1.0, 0.0 );
	vec3 pink = vec3( 1.0, 0.0, 0.5 );
	vec3 blue = vec3( 0.0, 0.8, 1.0 );
	vec3 black = vec3( 0.0 );
	vec3 orange = vec3( 1.0, 0.5, 0.0 );
	
	float f2 = clamp( f, 0.0, 0.8 );

	if( f2 > 0.4 )
		c = mix( blue, white, ( f2 - 0.4 ) / 0.4 );
	else
		c = mix( black, blue, ( f2 / 0.4 ) );
	
	return vec4( c * f, f );
}

vec4 nebula()
{
	vec3 nebulaCenter = vec3( 1.0, 0.0, 0.0 );
	float n = fbm2( normalize( TexCoords ), 8, 5.0, 0.65 );
	float n2 = fbm2( normalize( TexCoords ), 2, 1.0, 0.65 );

	float maxAngle = cos( 0.2 );

	float angle = ( dot( nebulaCenter, normalize( TexCoords ) ) - maxAngle ) / ( 1.0 - maxAngle );
	float factor = clamp( angle , 0.0, 1.0 );

	float color = n * n2;
	
	return nebulaColor( color ) * 1.0;
}

void main()
{
	vec4 color = nebula();
	Color = vec4( color.rgb, 1.0 );
}