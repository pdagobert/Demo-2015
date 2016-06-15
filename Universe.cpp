#include "Universe.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

UniverseRenderer::UniverseRenderer() :
	m_distantStar( "Shaders/Billboard.vs", "Shaders/DistantStar.fs" ),
	m_starGlow( "Shaders/Billboard.vs", "Shaders/StarGlow.fs" ),
	m_uvPlanet( "Shaders/Mesh.vs", "Shaders/UVPlanet.fs" ),
	m_atmosphere( "Shaders/Mesh.vs", "Shaders/Atmosphere.fs" ),
	m_ring( "Shaders/Mesh.vs", "Shaders/Ring.fs" ),
	m_asteroid( "Shaders/Asteroid.vs", "Shaders/Asteroid.fs" ),
	m_starGlowTex( load2DTexture( "Textures/star_glow.png" ) ),
	m_quad( createUnitQuad() ),
	m_sphere( createUVSphere( 1.0f, 100 ) ),
	m_disk( createDisk( 25.0f, 50.0f ) )
{

}

UniverseRenderer& UniverseRenderer::getInstance()
{
	static UniverseRenderer instance;
	return instance;
}

void UniverseRenderer::renderDistantStar( const DistantStar& s, const glm::mat4& proj, const glm::mat4& view )
{
	m_distantStar.bind();
	m_distantStar.setUniform( "wvp", proj * view );
	m_distantStar.setUniform( "world", glm::mat4() );
	m_distantStar.setUniform( "view", view );
	m_distantStar.setUniform( "center", s.position );
	m_distantStar.setUniform( "scale", s.radius * 3.5f );
	m_distantStar.setUniform( "radius", s.radius * 2.2f );
	m_distantStar.setUniform( "origin", s.position );
	m_distantStar.setUniform( "color", s.color );

	m_quad.render( GL_TRIANGLES );
}

void UniverseRenderer::renderStarGlow( const StarGlow& s, const glm::mat4& proj, const glm::mat4& view )
{
	m_starGlow.bind();
	m_starGlow.setUniform( "wvp", proj * view );
	m_starGlow.setUniform( "world", glm::mat4() );
	m_starGlow.setUniform( "view", view );
	m_starGlow.setUniform( "center", s.position );
	m_starGlow.setUniform( "scale", s.radius );
	m_starGlow.setUniform( "color", s.color );
	m_starGlow.setUniform( "tex", 0 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_starGlowTex );

	m_quad.render( GL_TRIANGLES );
}

void UniverseRenderer::renderUVPlanet( const UVPlanet& p, const glm::mat4& proj, const glm::mat4& view, const glm::vec3& cameraPos )
{
	glm::vec3 lightDir = -glm::normalize( p.position );

	glm::mat4 world;
	world = glm::translate( glm::mat4(), p.position );
	world = glm::rotate( world, p.angle, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	world = glm::scale( world, glm::vec3( p.radius ) );

	m_uvPlanet.bind();
	m_uvPlanet.setUniform( "wvp", proj * view * world );
	m_uvPlanet.setUniform( "world", world );
	m_uvPlanet.setUniform( "tex", 0 );
	m_uvPlanet.setUniform( "lightDir", lightDir );
	m_uvPlanet.setUniform( "cameraPos", cameraPos );
	m_uvPlanet.setUniform( "atmoColor", p.atmoColor );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, p.tex );

	m_sphere.render( GL_TRIANGLES );

	if( p.hasRing )
		renderRing( p, proj, view );

	renderAtmosphere( p, proj, view, cameraPos );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

void UniverseRenderer::renderAtmosphere( const UVPlanet& p, const glm::mat4& proj, const glm::mat4& view, const glm::vec3& cameraPos )
{
	if( p.atmoRadius == 0.0f )
		return;

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	glm::mat4 world;
	world = glm::translate( world, p.position );
	world = glm::scale( world, glm::vec3( p.radius + p.atmoRadius ) );

	m_atmosphere.bind();
	m_atmosphere.setUniform( "wvp", proj * view * world );
	m_atmosphere.setUniform( "world", world );
	m_atmosphere.setUniform( "planetWorldPos", p.position );
	m_atmosphere.setUniform( "cameraPos", cameraPos );
	m_atmosphere.setUniform( "planetRadius", p.radius );
	m_atmosphere.setUniform( "atmoRadius", p.atmoRadius );
	m_atmosphere.setUniform( "atmoColor", p.atmoColor );
	m_atmosphere.setUniform( "lightDir", -glm::normalize( p.position ) );

	m_sphere.render( GL_TRIANGLES );

	glDisable( GL_CULL_FACE );
}

void UniverseRenderer::renderRing( const UVPlanet& p, const glm::mat4& proj, const glm::mat4& view )
{
	glm::mat4 world;
	world = glm::translate( world, p.position );
	//world = glm::scale( world, glm::vec3( p.outerRingRadius, 1.0f, p.outerRingRadius ) );

	m_ring.bind();
	m_ring.setUniform( "wvp", proj * view * world );
	m_ring.setUniform( "world", world );
	m_ring.setUniform( "tex", 0 );
	m_ring.setUniform( "innerAngle", p.innerRingRadius );
	m_ring.setUniform( "outerAngle", p.outerRingRadius );
	m_ring.setUniform( "position", p.position );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_1D, p.ringTex );

	m_disk.render( GL_TRIANGLES );

	glBindTexture( GL_TEXTURE_1D, 0 );
}

void UniverseRenderer::renderAsteroid( const Asteroid& a, const glm::mat4& proj, const glm::mat4& view )
{
	glm::mat4 world;
	world = glm::translate( world, a.position );
	world = glm::rotate( world, a.angle, glm::normalize( glm::vec3( 0.2, 0.6, 0.4 ) ) );

	m_asteroid.bind();
	m_asteroid.setUniform( "wvp", proj * view * world );
	m_asteroid.setUniform( "world", world );
	m_asteroid.setUniform( "radius", a.radius );
	m_asteroid.setUniform( "tex", 0 );
	m_asteroid.setUniform( "lightDir", -glm::normalize( a.position ) );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, a.tex );

	m_sphere.render( GL_TRIANGLES );

	glBindTexture( GL_TEXTURE_2D, 0 );
}
