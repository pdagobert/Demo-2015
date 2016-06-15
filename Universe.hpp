#ifndef __UNIVERSE_HPP__
#define __UNIVERSE_HPP__

#include "Elements.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class UniverseRenderer
{
public:
	UniverseRenderer();

	static UniverseRenderer& getInstance();

	void renderDistantStar( const DistantStar& s, const glm::mat4& proj, const glm::mat4& view );
	void renderStarGlow( const StarGlow& s, const glm::mat4& proj, const glm::mat4& view );
	void renderUVPlanet( const UVPlanet& p, const glm::mat4& proj, const glm::mat4& view, const glm::vec3& cameraPos );
	void renderAtmosphere( const UVPlanet& p, const glm::mat4& proj, const glm::mat4& view, const glm::vec3& cameraPos );
	void renderRing( const UVPlanet& p, const glm::mat4& proj, const glm::mat4& view );
	void renderAsteroid( const Asteroid& a, const glm::mat4& proj, const glm::mat4& view );

private:
	Shader m_distantStar, m_starGlow, m_uvPlanet, m_atmosphere, m_ring, m_asteroid;

	GLuint m_starGlowTex;
	Mesh m_quad, m_sphere, m_disk;
};
#endif