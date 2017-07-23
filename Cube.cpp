#include "Cube.h"
#include "Window.h"

Cube::Cube()
{
	toWorld = glm::mat4(1.0f);

	rotation = new Quarternion(0.0f, 0.0f, 0.0f, 1.0f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		3 * sizeof(GLfloat), 
		(GLvoid*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

Cube::~Cube()
{

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Cube::draw(GLuint shaderProgram, glm::vec3 cam_pos)
{ 
	glm::mat4 modelview = Window::V * toWorld * rotation->ToMat();

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	
	GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
	GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");

	// Chrome
	glUniform3f(matAmbientLoc, 0.25f, 0.25f, 0.25f);
	glUniform3f(matDiffuseLoc, 0.4f, 0.4f, 0.4f);
	glUniform3f(matSpecularLoc, 0.774597f, 0.774597f, 0.774597f);
	glUniform1f(matShineLoc, 0.6f * 128.0f);

	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &cam_pos[0]);
	
	glUniform1f(glGetUniformLocation(shaderProgram, "toggle"), 0.0f);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Cube::update()
{
}

void Cube::move(float dist)
{
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(dist, 0.0f, 0.0f)) * toWorld;
}

