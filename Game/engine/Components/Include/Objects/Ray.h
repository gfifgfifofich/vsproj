#pragma once

class Ray
{
public:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 direction = glm::vec2(1.0f, 0.0f);

	glm::vec2 ClosestPoint = glm::vec2(0.0f);

	int Steps = 100;
	int iterations = 2;
	float length = 1000.0f;

	float step = 10;


	void normalize();
	void Update();
	Ray(glm::vec2 position, glm::vec2 direction, float length, int steps);
	Ray();

};
