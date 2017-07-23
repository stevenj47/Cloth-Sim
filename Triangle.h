#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

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
#include <cmath>


#include "Particle.h"

class Triangle
{
public:
	Triangle(Particle * v1, Particle * v2, Particle * v3);
	~Triangle();

	Particle * p1;
	Particle * p2;
	Particle * p3;

	float density;
	float drag;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> indices;

	GLuint VBO, VAO, EBO, NBO;
	GLuint uProjection, uModelview;

	void ComputeForce();
	void Draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 pers);
	void Update(glm::vec3 adjustWind);
};

#endif