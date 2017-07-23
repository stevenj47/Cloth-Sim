#include "Particle.h"

Particle::Particle()
{
	velocity = acceleration = momentum = force = glm::vec3(0.0f);
	mass = 1.5f;
	isFixed = false;
	floor_height = -4.95f;
	elasticity = 1.2f;
	friction = 0.4f;
}

Particle::~Particle()
{
}

void Particle::Update(float deltaTime, glm::vec3 adjustPosition)
{
	if (!isFixed) 
	{
		acceleration = (1.0f / mass) * force;

		velocity += acceleration * deltaTime;

		if (position.y < floor_height)
		{
			position.y = 2.0f * floor_height - position.y;
			velocity.y = -1.0f * elasticity * velocity.y;
			velocity.x = (1.0f - friction) * velocity.x;
			velocity.z = (1.0f - friction) * velocity.z;
		}

		position += velocity * deltaTime;

		force = glm::vec3(0.0f);
	}
	else
	{
		position = position + adjustPosition;
		force = glm::vec3(0.0f);
	}
}

void Particle::Draw()
{
}

void Particle::ApplyForce(glm::vec3 force_partial)
{
//	fprintf(stderr, "Applying Force: %f, %f, %f \n", force_partial.x, force_partial.y, force_partial.z);

	force += force_partial;

//	fprintf(stderr, "Resulting Force: %f, %f, %f \n", force.x, force.y, force.z);

}

