#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;
layout( location = 2 ) in vec3 normal;

uniform mat4 wvp;
uniform mat4 world;
uniform mat4 view;

uniform vec3 center;
uniform float scale;
uniform float flareOffset;
uniform float textureType;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;
flat out float alpha;

void main()
{
	vec4 c = wvp * vec4( center, 1.0 );
	c /= c.w;
	
	vec2 screenSpacePos = c.xy;
	vec2 dir = vec2( 0.0 ) - screenSpacePos;

	c.xy += position.xy * scale * ( 0.1 + ( 1.0 - textureType ) * 0.1 ) + dir * pow( flareOffset, 2.0 ) * 0.5;//scale * position.xy * dir * pow( flareOffset, 2.0 ) * 0.5;
	
	alpha = max( 0.0, 1.0 - length( dir ) / 1.0 ) * 0.2;
	
	c.z = 0.0;
	
	gl_Position = c;
	
	float tex = texCoords.x;
	if( textureType == 0.0 )
		tex *= 0.5;
	else
		tex = min( tex + 0.5, 1.0 );
	
	TexCoords = vec2( tex, texCoords.y );
	
	/*vec3 cameraRight = vec3( view[ 0 ][ 0 ], view[ 1 ][ 0 ], view[ 2 ][ 0 ] );
	vec3 cameraUp = vec3( view[ 0 ][ 1 ], view[ 1 ][ 1 ], view[ 2 ][ 1 ] );
	
	//vec3 pos = center + cameraRight * position.x + cameraUp * position.y;
	
	//gl_Position = wvp * vec4( pos, 1.0 );
	//gl_Position /= gl_Position.w;
	
	// lens flares strength
	vec2 dir = vec2( 0.0 ) - getScreenSpaceCenter();
	
	gl_Position.xy *= 1.0;//* dir * flareOffset * pow( flareOffset, 2.0 ) * 0.5;
	
	Position = vec3( world * vec4( pos, 1.0 ) );

	
	Normal = mat3( transpose( inverse( world ) ) ) * normal;*/
}
