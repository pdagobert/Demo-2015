#version 330 core

uniform float time;

in vec2 TexCoords;
in vec3 Normal;

out vec4 Color;

vec2 rotZ( vec2 vec, float a )
{
	mat2 m = mat2( cos( a ), -sin( a ), sin( a ), cos( a ) );
	return m * vec;
}

vec3 getColorMask()
{
	vec3 red = vec3( 1.0, 0.0, 0.0 );
	vec3 green = vec3( 0.0, 1.0, 0.0 );
	vec3 blue = vec3( 0.0, 0.0, 1.0 );
	vec3 white = vec3( 1.0 );
	vec3 black = vec3( 0.0 );
	
	float t = mod( time, 15.0 );
	
	if( t < 3.0 )
	{
		float c = t / 3.0;
		return mix( black, red, c );
	}
	else if( t < 6.0 )
	{
		float c = ( t - 3.0 ) / 3.0;
		return mix( red, green, c );
	}
	else if( t < 9.0 )
	{
		float c = ( t - 6.0 ) / 3.0;
		return mix( green, blue, c );
	}
	else if( t < 12.0 )
	{
		float c = ( t - 9.0 ) / 3.0;
		return mix( blue, red, c );
	}
	else
	{
		if( time < 15.0 )
		{
			float c = ( t - 13.0 ) / 3.0;
			return mix( red, white, c );
		}
		else
		{
			float c = ( t - 12.0 ) / 1.5;
			return mix( red, white, c );
		}
	}
}

vec3 anim1()
{
	vec3 color = vec3( 0.0 );

	for( int i = 0; i < 5; i++ )
	{
		vec2 uv = TexCoords * 2.0 - 1.0;
		//uv += sin( time );
		uv = rotZ( uv, time + i * 0.4 * time );
		uv *= 1.0 + mod( time * i, 10.0 );
	
		float x = fract( uv.x );
		float y = fract( uv.y );
		
		float dist = distance( vec2( 0.5, 0.5 ), vec2( x, y ) ) / distance( vec2( 0.5 ), vec2( 0.0 ) );
		
		float c = 1.0 - dist;
		
		float alpha = 1.0 - mod( time * i, 10.0 ) / 10.0;
		color += getColorMask() * c * ( alpha / 2.0 );
	}

	return color;
}

vec3 anim2()
{
	vec3 color = vec3( 0.0 );

	for( int i = 0; i < 5; i++ )
	{
		vec2 uv = TexCoords * 2.0 - 1.0;
		uv *= 1.0 + mod( time * i, 5.0 );
		uv = rotZ( uv, time + i * 0.4 * time );
	
		float x = fract( uv.x );
		float y = fract( uv.y );
		
		float dist = distance( vec2( 0.5, 0.5 ), vec2( x, y ) ) / distance( vec2( 0.5 ), vec2( 0.0 ) );
		
		float c = 1.0 - cos( dist );
		
		float alpha = 1.0 - mod( time * i, 5.0 ) / 5.0;
		color += getColorMask() * c * alpha;
	}

	if( 15.0 + 12.0 < time )
	{
		float c = ( time - 27.0 ) / 1.5;
		color = mix( color, vec3( 1.0 ), c );
	}

	return color;
}

void main()
{		
	vec3 color = vec3( 0.0 );

	if( time < 15.0 )
		color = anim1();
	else
		color = anim2();

	Color = vec4( color, 1.0 );
}