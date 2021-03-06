#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec3 normal;

uniform mat4 wvp;

out vec3 TexCoords;

void main()
{
	vec4 pos = wvp * vec4( position, 1.0 );
	gl_Position = pos.xyww;

	TexCoords = position;
}