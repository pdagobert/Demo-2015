#include "Skybox.hpp"
#include "Utility.hpp"
#include "Geometry.hpp"
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

Skybox::Skybox() :
	m_cubemapFbo( createCubemap( 1024 ) ),
	m_fbo( createFramebuffer( m_cubemapFbo, 1024, 1024 ) ),
	m_nebula( "Shaders/Skybox.vs", "Shaders/Nebula.fs" ),
	m_cube( createUnitCube() )
{

}

std::vector< DistantStar > Skybox::getSortedStars( int numStars )
{
	std::vector< DistantStar > stars( numStars );

	int numColors = 4;
	glm::vec3 colors[] = { glm::vec3( 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) };

	for( DistantStar& s : stars )
	{
		s.radius = 0.2 + Random() * 0.3f;

		//if( Random() > 0.98 )
		//	s.radius *= 1.0f + Random() * 4.0f;

		float dist = 180.0f + 30 * Random();
		s.position = glm::normalize( glm::vec3( Random() * 2.0 - 1.0, Random() * 2.0 - 1.0, Random() * 2.0 - 1.0 ) ) * dist;

		int index = rand() % numColors;

		s.color = colors[ index ];
	}

	std::sort( stars.begin(), stars.end(),
		[]( const DistantStar& a, const DistantStar& b )
		{
			return glm::length( a.position ) > glm::length( b.position );
		} );

	return stars;
}

void Skybox::preRenderSkybox( int windowWidth, int windowHeight )
{
	std::vector< DistantStar > stars = getSortedStars( 1000 );

	renderToCubemap( windowWidth, windowHeight,
		[ this, &stars ]( const glm::mat4& proj, const glm::mat4& view )
	{
		m_nebula.bind();
		m_nebula.setUniform( "wvp", proj * glm::mat4( glm::mat3( view ) ) );

		m_cube.render( GL_TRIANGLES );

		for( DistantStar& s : stars )
			UniverseRenderer::getInstance().renderDistantStar( s, proj, view );
	} );
}

void Skybox::renderToCubemap( int windowWidth, int windowHeight, std::function< void( const glm::mat4&, const glm::mat4& ) > func )
{
	glViewport( 0, 0, 1024, 1024 );
	glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );

	glm::mat4 proj = glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 1000.0f );
	
	glm::vec3 cameraDirs[] = 
	{
		glm::vec3( 1.0f, 0.0f, 0.0f ),
		glm::vec3( -1.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f )
	};

	glm::vec3 cameraUps[] =
	{
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f )
	};

	for( int i = 0; i < 6; i++ )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_cubemapFbo, 0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::mat4 view = glm::lookAt( glm::vec3( 0.0 ), cameraDirs[ i ], cameraUps[ i ] );

		func( proj, view );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, windowWidth, windowHeight );
}

GLuint Skybox::getCubemap() const
{
	return m_cubemapFbo;
}
