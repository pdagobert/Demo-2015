#version 330 core

uniform float planetRadius;
uniform float atmoRadius;
uniform vec3 atmoColor;

uniform vec3 lightDir;

uniform vec3 planetWorldPos;
uniform vec3 cameraPos;


in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;
in vec3 LocalSpaceNormal;

out vec4 Color;

void main()
{
	vec3 planetToCam = planetWorldPos - cameraPos;
	vec3 ray = normalize( Position - cameraPos );
	
	float p = dot( planetToCam, ray );
	ray *= p;

	float d = 1.0 - ( distance( planetToCam, ray ) - planetRadius ) / atmoRadius;
	
	float diffuseFactor = max( dot( normalize( Normal ), lightDir ), 0.0 );
	
	Color = vec4( atmoColor, d * diffuseFactor );
}


/*
ray = norm( a )
ray *= dot

*/