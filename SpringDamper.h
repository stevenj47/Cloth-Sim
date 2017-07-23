#ifndef _SPRING_H_
#define _SPRING_H_

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

#include "Particle.h"

class SpringDamper
{
public:
	SpringDamper(Particle * v1, Particle * v2);
	~SpringDamper();

	float springConstant;
	float dampingFactor;
	float restLength;

	Particle * p1;
	Particle * p2;

	void ComputeForce();
};

#endif
