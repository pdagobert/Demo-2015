#ifndef __ELEMENTS_HPP__
#define __ELEMENTS_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>

struct DistantStar
{
	float radius;
	glm::vec3 position;
	glm::vec3 color;
};

struct StarGlow
{
	float radius;
	glm::vec3 position;
	glm::vec3 color;
};

struct UVPlanet
{
	float radius;
	glm::vec3 position;
	float rotationSpeed;
	float angle;

	GLuint tex;

	float atmoRadius;
	glm::vec3 atmoColor;

	bool hasRing;
	float innerRingRadius, outerRingRadius;
	GLuint ringTex;
};

struct Asteroid
{
	float radius;
	glm::vec3 position;
	float angle;

	GLuint tex;
};
#endif