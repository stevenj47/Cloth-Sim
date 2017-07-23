#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle * v1, Particle * v2)
{
	p1 = v1;
	p2 = v2;

	springConstant = 300.0f;
	dampingFactor = 18.0f;
	restLength = glm::length(p1->position - p2->position);
}

SpringDamper::~SpringDamper()
{
}

void SpringDamper::ComputeForce()
{
	glm::vec3 distance_vec = p2->position - p1->position;
	float length = glm::length(distance_vec);

	distance_vec = glm::normalize(distance_vec);

	glm::vec3 v1 = distance_vec * p1->velocity;
	glm::vec3 v2 = distance_vec * p2->velocity;

	glm::vec3 force_sd = -1.0f * springConstant * (restLength - length) - dampingFactor * (v1 - v2);

	glm::vec3 f1 = force_sd * distance_vec;
	glm::vec3 f2 = -1.0f * f1;

	p1->ApplyForce(f1);
	p2->ApplyForce(f2);
}
