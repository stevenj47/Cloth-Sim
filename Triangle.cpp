#include "Triangle.h"

Triangle::Triangle(Particle * v1, Particle * v2, Particle * v3)
{
	p1 = v1;
	p2 = v2;
	p3 = v3;

	density = 3.225f;
	drag = 0.25f;

	glm::vec3 n1 = p2->position - p1->position;
	glm::vec3 n2 = p3->position - p1->position;
	glm::vec3 norm = glm::normalize(glm::cross(n1, n2));

	p1->normal += norm;
	p2->normal += norm;
	p3->normal += norm;


}

Triangle::~Triangle()
{

}

void Triangle::ComputeForce()
{
}

void Triangle::Draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 pers)
{

}

void Triangle::Update(glm::vec3 adjustWind)
{
	glm::vec3 n1 = p2->position - p1->position;
	glm::vec3 n2 = p3->position - p1->position;
	glm::vec3 norm = glm::normalize(glm::cross(n1, n2));

	p1->normal += norm;
	p2->normal += norm;
	p3->normal += norm;

	glm::vec3 comb_velocity = (1.0f / 3.0f) * (p1->velocity + p2->velocity + p3->velocity) - adjustWind;
	glm::vec3 area = 0.5f * glm::cross(n1, n2) * glm::normalize(comb_velocity) * norm;
	float vel_length = glm::length(comb_velocity);
	glm::vec3 aero_force = (1.0f/3.0f) * -0.5f * density * pow(vel_length, 2.0f) * drag * area * norm;

	p1->ApplyForce(aero_force);
	p2->ApplyForce(aero_force);
	p3->ApplyForce(aero_force);

}
