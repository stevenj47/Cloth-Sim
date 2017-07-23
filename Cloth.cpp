#include "Cloth.h"

Cloth::Cloth(float width, float height, float rows, float columns)
{
	cloth_width = width;
	cloth_height = height;
	num_rows = rows;
	num_columns = columns;

	createParticles();
	createDampers();
	createTriangles();

	for (int i = 0; i < num_columns; i++)
	{
		particles[i]->isFixed = true;
	//	particles[num_columns - 1 - i]->isFixed = true;
	}

	for (int i = 0; i < particles.size(); i++)
	{
		vertices.push_back(particles[i]->position.x);
		vertices.push_back(particles[i]->position.y);
		vertices.push_back(particles[i]->position.z);

		normals.push_back(particles[i]->normal.x);
		normals.push_back(particles[i]->normal.y);
		normals.push_back(particles[i]->normal.z);
	}


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLuint), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

Cloth::~Cloth()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &EBO);
}

void Cloth::createParticles()
{
	particles.resize(num_rows * num_columns);

	for (int x = 0; x < num_columns; x++)
	{
		for (int y = 0; y < num_rows; y++)
		{
			Particle * p = new Particle();
			p->position = glm::vec3(cloth_width * (float)(x / num_columns) - 10.0f, -cloth_height * (float)(y / num_rows) + 15.0f, 0.0f);
			particles[y * num_columns + x] = p;
		}
	}

}

void Cloth::createDampers()
{
	for (int x = 0; x < num_columns; x++)
	{
		for (int y = 0; y < num_rows; y++)
		{
			SpringDamper * damp;
			int position = y * num_columns + x;

			if (x < num_columns - 1)
			{
				damp = new SpringDamper(particles[position], particles[position + 1]);
				dampers.push_back(damp);
			}

			if (y < num_rows - 1)
			{
				damp = new SpringDamper(particles[position], particles[position + num_columns]);
				dampers.push_back(damp);
			}

			if (x < num_columns - 1 && y < num_columns - 1)
			{
				damp = new SpringDamper(particles[position], particles[position + num_columns + 1]);
				dampers.push_back(damp);
				damp = new SpringDamper(particles[position + 1], particles[position + num_columns]);
				dampers.push_back(damp);
			}

			/*
			if (x < num_columns - 2)
			{
				damp = new SpringDamper(particles[position], particles[position + 2]);
				dampers.push_back(damp);
			}

			if (y < num_rows - 2)
			{
				damp = new SpringDamper(particles[position], particles[position + 2 * num_columns]);
				dampers.push_back(damp);
			}

			if (x < num_columns - 2 && y < num_columns - 2)
			{
				damp = new SpringDamper(particles[position], particles[position + 2 * num_columns + 2]);
				dampers.push_back(damp);
				damp = new SpringDamper(particles[position + 2], particles[position + 2 * num_columns]);
				dampers.push_back(damp);
			} */
		}
	}
}

void Cloth::createTriangles()
{
	for (int x = 0; x < num_columns - 1; x++)
	{
		for (int y = 0; y < num_rows - 1; y++)
		{
			Triangle * triang;
			int position = y * num_columns + x;
			
			triang = new Triangle(particles[position + 1], particles[position], particles[position + num_columns]);
			triangles.push_back(triang);

			indices.push_back(position + 1);
			indices.push_back(position);
			indices.push_back(position + num_columns);

			triang = new Triangle(particles[position + 1 + num_columns], particles[position + 1], particles[position + num_columns]);
			triangles.push_back(triang);

			indices.push_back(position + 1 + num_columns);
			indices.push_back(position + 1);
			indices.push_back(position + num_columns);
		}
	}
}

void Cloth::AddFloor()
{
	int indice = vertices.size();

	vertices.push_back(100.0f);
	vertices.push_back(-5.0f);
	vertices.push_back(-100.0f);

	vertices.push_back(-100.0f);
	vertices.push_back(-5.0f);
	vertices.push_back(-100.0f);

	vertices.push_back(-100.0f);
	vertices.push_back(-5.0f);
	vertices.push_back(100.0f);

	vertices.push_back(100.0f);
	vertices.push_back(-5.0f);
	vertices.push_back(100.0f);

	for (int i = 0; i < 4; i++)
	{
		normals.push_back(0.0f);
		normals.push_back(1.0f);
		normals.push_back(0.0f);
	}

	indices.push_back(indice - 1);
	indices.push_back(indice);
	indices.push_back(indice + 1);

	indices.push_back(indice + 3);
	indices.push_back(indice);
	indices.push_back(indice + 2);

}

void Cloth::Draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 pers, glm::vec3 cam_pos)
{
	glm::mat4 modelview = view;
	glm::mat4 perspective = pers;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &pers[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &view[0][0]);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW); 

	GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
	GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");

	// Pearl
	glUniform3f(matAmbientLoc, 0.25f, 0.20725f, 0.20725f);
	glUniform3f(matDiffuseLoc, 1.0f, 0.829f, 0.829f);
	glUniform3f(matSpecularLoc, 0.296648f, 0.296648f, 0.296648f);
	glUniform1f(matShineLoc, 0.088f * 128.0f);

	glUniform1f(glGetUniformLocation(shaderProgram, "toggle"), 1.0f);

	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &cam_pos[0]);

	// glPointSize(3.0f);
	// glDrawArrays(GL_POINTS, 0, vertices.size());

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Cloth::Update(glm::vec3 adjustPosition, glm::vec3 adjustWind)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->normal = glm::vec3(0.0f);
		glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f) * particles[i]->mass;
		particles[i]->ApplyForce(gravity);
	}

	for (int i = 0; i < dampers.size(); i++)
	{
		dampers[i]->ComputeForce();
	}

	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i]->Update(adjustWind);
	}

	vertices.clear();
	normals.clear();

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Update(0.008f, adjustPosition);

		vertices.push_back(particles[i]->position.x);
		vertices.push_back(particles[i]->position.y);
		vertices.push_back(particles[i]->position.z);

		normals.push_back(particles[i]->normal.x);
		normals.push_back(particles[i]->normal.y);
		normals.push_back(particles[i]->normal.z);
	}
}
