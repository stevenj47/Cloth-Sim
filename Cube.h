#ifndef _CUBE_H_
#define _CUBE_H_

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
#include <glm/gtc/matrix_transform.hpp>

#include "Quarternion.h"

class Cube
{
public:
	Cube();
	~Cube();

	glm::mat4 toWorld;

	Quarternion * rotation;

	void draw(GLuint, glm::vec3);
	void update();
	void move(float);

	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};


const GLfloat vertices[8][3] = {

	{-0.5, -0.5,  0.5}, {0.5, -0.5,  0.5}, {0.5,  0.5,  0.5}, {-0.5,  0.5,  0.5},
	{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5,  0.5, -0.5}, {-0.5,  0.5, -0.5}
};


const GLuint indices[6][6] = {
	{0, 1, 2, 2, 3, 0},
	{1, 5, 6, 6, 2, 1},
	{7, 6, 5, 5, 4, 7},
	{4, 0, 3, 3, 7, 4},
	{4, 5, 1, 1, 0, 4},
	{3, 2, 6, 6, 7, 3}
};

#endif

