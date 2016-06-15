#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Skybox.hpp"
#include <vector>

class Application : public Window
{
public:
	Application();

private:
	void initSDLMixer();

	void getCameraSetup( float time, glm::vec3& cameraPos, glm::vec3& cameraTarget );

	void renderIntro( float time );

	void renderSkybox( GLuint cubemap, const glm::mat4& proj, const glm::mat4& view );

	void renderRocket( float time, const glm::mat4& proj, const glm::mat4& view );
	void renderLensFlares( const glm::vec3& starPos, const glm::mat4& proj, const glm::mat4& view );
	void renderAsteroid( const glm::mat4& proj, const glm::mat4& view );

public:
	virtual void update( float deltaTime );
	virtual void render();

	//virtual void keyPressed( int key );

private:
	Shader m_introShader, m_skyboxShader, m_rocketShader, m_flares, m_creditShader;

	Mesh m_quad, m_cube;

	glm::vec3 m_lightDir;

	Skybox m_skybox;

	float m_angle;
	bool m_moves;

	std::vector< UVPlanet > m_planets;

	Mesh m_rocket;
	GLuint m_rocketTex;

	glm::vec3 m_rocketPos;

	GLuint m_flareTex, m_asteroidTex;

	glm::vec3 m_asteroidPos;

	int m_volume;

	GLuint m_credits[ 2 ];

	float m_time;
};
#endif