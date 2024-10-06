#pragma once

struct point
{
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	glm::vec2 Force = glm::vec2(0.0f);
	float mass = 1.0f;

	void Process(float dt);
	
};
//A simpler wersion of ball(no velocity, color ect.)
struct circle
{
	float r = 10.0f;
	glm::vec2 Position = glm::vec2(0.0f);
	glm::vec2 Velocity = glm::vec2(0.0f);
};


class ball
{
public:
	float r = 10;

	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	glm::vec2 Force = glm::vec2(0.0f);

	float mass = 1.0f;
	float roughness = 1.0f;
	float bounciness = 1.0f;

	float rotation = 0.0f;
	float rotationVelocity = 0.0f;
	float rotationForce = 0.0f;

	int id = -1;

	bool FixedRotation=false;
	bool del = false;
	bool Kinematic = false;
	bool colided = false;

	glm::vec2 GroundVector = glm::vec2(0.0f, 0.0f);// stores last collision, helpfull for springs


	void Process(float dt);
};
static std::vector <ball*> ballsptr;

