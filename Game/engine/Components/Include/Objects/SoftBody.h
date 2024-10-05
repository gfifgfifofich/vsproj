#pragma once
#include "Ball.h"
class SoftBody
{
public:
	std::vector <ball> balls;

	glm::vec2 Force;

	float springLength = 1.0f;
	float absorption = 0.2f;
	float stiffness = 10000.0f;
	float friction = 0.0f;

	bool deformation = false;
	float deformationRadius = 35.0f;
	float deformationCoefficient = 0.3f;

	std::vector <glm::ivec3> springs;

	

	void Process(float dt)
	{
		for (int i = 0; i < springs.size(); i++)
		{
			//SpringBetweenBalls(&balls[springs[i].x], &balls[springs[i].y], springs[i].z* springLength, stiffness,absorption);

			glm::vec2 posdif = balls[springs[i].y].position - balls[springs[i].x].position;
			float posdifl = length(posdif);
			glm::vec2 norm = posdif / posdifl;
			glm::vec2 Difference = posdif - norm * (float)springs[i].z * springLength;


			glm::vec2 Change1 = Difference * 0.5f * stiffness * dt * 60.0f;
			glm::vec2 Change2 = -Difference * 0.5f * stiffness * dt * 60.0f;


			balls[springs[i].x].Force += Change1;
			balls[springs[i].y].Force += Change2;


			glm::vec2 vel1 = balls[springs[i].x].velocity - balls[springs[i].y].velocity;
			glm::vec2 vel2 = balls[springs[i].y].velocity - balls[springs[i].x].velocity;


			glm::vec2 axis1 = DOT(vel1, norm) * norm;
			glm::vec2 axis2 = DOT(vel2, norm) * norm;

			balls[springs[i].x].velocity -= axis1 * absorption * dt * 60.0f;
			balls[springs[i].y].velocity -= axis2 * absorption * dt * 60.0f;

			if (deformation)
			{
				float l = sqrlength(Difference);
				if (l > deformationRadius * deformationRadius)
				{
					if (posdifl < (float)springs[i].z * springLength)
						springs[i].z -= sqrt(l) * deformationCoefficient;
					else
						springs[i].z += sqrt(l) * deformationCoefficient;
				}
			}




		}


		for (int i = 0; i < balls.size(); i++)
		{
			balls[i].GroundVector = glm::vec2(0.0f);
			balls[i].velocity -= balls[i].velocity * friction * dt;
			balls[i].Force += Force;
			balls[i].Process(dt);
			balls[i].Force = glm::vec2(0.0f);
			balls[i].FixedRotation = true;
			balls[i].roughness = 0.0f;
		}
		Force = glm::vec2(0.0f);

	}

};

