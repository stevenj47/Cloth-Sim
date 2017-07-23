#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <utility>
#include <cstdlib>

class Particle
{
public:
	Particle();
	~Particle();

	void Update(float deltaTime, glm::vec3 adjustPosition);
	void Draw();
	void ApplyForce(glm::vec3 force_partial);

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 momentum;
	glm::vec3 force;

	float mass;
	float floor_height;
	float elasticity;
	float friction;

	bool isFixed;
};

#endif
