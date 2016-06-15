#version 330 core

uniform sampler2D tex;
uniform vec3 color;
uniform vec3 cameraDir;

uniform vec3 origin;
uniform float scale;

in vec3 Position;
in vec2 TexCoords;

out vec4 Color;

#include "Noise/noise4D.glsl"

vec3 getColor( float dist )
{
	return mix( vec3( 1.0 ), color, dist * 5 );
}

void main()
{
	float dist = length( Position - origin ) / scale;
	
	vec3 glowColor = texture( tex, TexCoords ).rgb;
	Color = vec4( glowColor * getColor( dist ), glowColor.r );
}