#ifndef _CLOTH_H_
#define _CLOTH_H_

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

#include "SpringDamper.h"
#include "Triangle.h"

class Cloth
{
public:
	Cloth(float width, float height, float rows, float columns);
	~Cloth();

	std::vector<Particle* > particles;
	std::vector<SpringDamper* > dampers;
	std::vector<Triangle* > triangles;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> indices;

	GLuint VBO, VAO, EBO, NBO;
	GLuint uProjection, uModelview;

	float cloth_width;
	float cloth_height;

	float num_rows;
	float num_columns;

	void createParticles();
	void createDampers();
	void createTriangles();
	void AddFloor();
	void Draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 pers, glm::vec3 cam_pos);
	void Update(glm::vec3 adjustPosition, glm::vec3 adjustWind);

};

#endif