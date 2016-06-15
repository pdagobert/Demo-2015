#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <GL/glew.h>
#include <functional>
#include "Universe.hpp"
#include "Shader.hpp"
#include "Geometry.hpp"

class Skybox
{
public:
	Skybox();

	void preRenderSkybox( int windowWidth, int windowHeight );

	void renderToCubemap( int windowWidth, int windowHeight, std::function< void( const glm::mat4&, const glm::mat4& ) > func );

	GLuint getCubemap() const;

private:
	std::vector< DistantStar > getSortedStars( int numStars );

private:
	GLuint m_cubemapFbo;
	GLuint m_fbo;

	Shader m_nebula;

	Mesh m_cube;
};
#endif