#version 330 core

uniform sampler1D tex;
uniform float innerAngle;
uniform float outerAngle;
uniform vec3 position;

in vec3 Position;
in vec3 Normal;
in vec3 LocalSpaceNormal;

out vec4 Color;

void main()
{
	vec3 normal = normalize( Normal );
	
	if( !gl_FrontFacing )
		normal = -normal;
	
	float dist = distance( Position, position );

	float alpha = 1.0;
	
	float smoothSize = ( outerAngle - innerAngle ) / 5.0;

	if( dist < innerAngle + smoothSize )
	{
		float cap = innerAngle + smoothSize;
		alpha = ( dist - innerAngle ) / ( cap - innerAngle );
	}
	else if( dist > outerAngle - smoothSize )
	{
		float cap = outerAngle - smoothSize;

		alpha = ( outerAngle - dist ) / ( outerAngle - cap );
	}

	vec3 lightPos = vec3( 0.0, 0.0, 1.0 );
	
	float diffuseFactor = max( dot( lightPos, normal ), 0.0 );
	
	float texCoord = ( dist - innerAngle ) / ( outerAngle - innerAngle );
	
	vec3 final = texture( tex, texCoord ).rgb;
	Color = vec4( final, alpha );
}