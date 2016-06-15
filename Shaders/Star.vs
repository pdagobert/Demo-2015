#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;
layout( location = 2 ) in vec3 normal;

uniform mat4 wvp;
uniform mat4 world;

out vec3 NormalLocalSpace;

void main()
{
	gl_Position = wvp * vec4( position, 1.0 );
	
	NormalLocalSpace = normal;
}