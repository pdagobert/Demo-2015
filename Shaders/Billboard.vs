#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;
layout( location = 2 ) in vec3 normal;

uniform vec3 center;
uniform float scale;

uniform mat4 wvp;
uniform mat4 world;
uniform mat4 view;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
	vec3 cameraRight = vec3( view[ 0 ][ 0 ], view[ 1 ][ 0 ], view[ 2 ][ 0 ] );
	vec3 cameraUp = vec3( view[ 0 ][ 1 ], view[ 1 ][ 1 ], view[ 2 ][ 1 ] );
	
	vec3 pos = center + cameraRight * position.x * scale + cameraUp * position.y * scale;
	
	gl_Position = wvp * vec4( pos, 1.0 );
	
	Position = vec3( world * vec4( pos, 1.0 ) );

	TexCoords = texCoords;
	Normal = mat3( transpose( inverse( world ) ) ) * normal;
}
