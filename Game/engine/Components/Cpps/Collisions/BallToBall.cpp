#include "../../Include/Helper.h"

#include "../../Include/Collisions/BallToBall.h"



void BallToPointCollision(ball* b, glm::vec2 point, float roughness)
{
	bool rotation = b->FixedRotation;
	if (rotation && sqrlength(b->position - point) < b->r * b->r && sqrlength(b->position - point) != 0.0f)
	{
		b->colided = true;
		glm::vec2 dif = glm::vec2(0.0f);

		dif = b->position - point;


		float dist = length(dif);
		float distancedifference = b->r - dist;

		glm::vec2 n1 = -dif / dist;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);

		float energy1 = DOT(b->velocity, n1);
		glm::vec2 velocity1 = DOT(b->velocity, CollisionLine) * CollisionLine;

		velocity1 = velocity1 * roughness;

		velocity1 -= n1 * energy1 * b->bounciness;

		b->GroundVector = n1;
		b->velocity = velocity1;
		b->position += (dif / dist) * distancedifference;

	}
	else if (sqrlength(b->position - point) < b->r * b->r && sqrlength(b->position - point) != 0.0f)
	{
		b->colided = true;

		glm::vec2 dif = b->position - point;


		float dist = length(dif);
		float distancedifference = b->r - dist;

		glm::vec2 n1 = -dif / dist;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);

		float energy = DOT(b->velocity, n1);//for regular elastic collision
		glm::vec2 velocity = DOT(b->velocity, CollisionLine) * CollisionLine;//for regular elastic collision
		velocity -= n1 * energy * b->bounciness;//regular elastic collision



		glm::vec2 Pointvelocity = DOT(-(b->rotationVelocity * 2 * pi * b->r * CollisionLine + velocity), CollisionLine) * CollisionLine;

		float A_sine_wave_sinusoidal_wave_or_just_sinusoid = Pointvelocity.x * n1.y - Pointvelocity.y * n1.x;

		b->rotationVelocity += -A_sine_wave_sinusoidal_wave_or_just_sinusoid / (2 * pi * b->r) * 0.5f * (1.0f - roughness);
		b->GroundVector = n1;

		b->velocity = velocity + Pointvelocity * 0.5f * (1.0f - roughness);
		b->position += -n1 * distancedifference;

	}
}
void BtBCollision(ball* b1, ball* b2, float roughness)
{



	glm::vec2 dif = b2->position - b1->position;
	if (dif.x * dif.x + dif.y * dif.y < (b1->r + b2->r) * (b1->r + b2->r))
	{
		float dist = length(dif);
		float distancedifference = (b1->r + b2->r) - dist;
		if(dist<0.0001f)
		{
			b2->position.y += 0.0002f;
			dif = b2->position - b1->position;
			dist = length(dif);
			distancedifference = (b1->r + b2->r) - dist;
		}
		// all the normals
		glm::vec2 n2 = dif / dist;
		glm::vec2 n1 = -n2;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);



		//velocities
		// "untached" component of velocity
		glm::vec2 velocity1 = DOT(b1->velocity, CollisionLine) * CollisionLine;
		glm::vec2 velocity2 = DOT(b2->velocity, -CollisionLine) * -CollisionLine;

		float energy1 = DOT(b1->velocity, n1);// basicly, a component of velocity, that is changed after collision
		float energy2 = DOT(b2->velocity, n2);

		if (b1->mass != b2->mass)
		{
			float mass1 = b1->mass;
			float mass2 = b2->mass;

			float mdivisor = 1.0f / (b1->mass + b2->mass);

			velocity1 -= n2 * (((mass1 - mass2) * mdivisor) * energy1) + n1 * (((2 * mass2) * mdivisor) * energy2);

			velocity2 -= n1 * (((mass2 - mass1) * mdivisor) * energy2) + n2 * (((2 * mass1) * mdivisor) * energy1);

			//positions
			if (mass1 < mass2)
			{
				float ratio = mass1 * mdivisor;
				ratio *= ratio;
				b1->position = b1->position + n1 * (distancedifference * (1.0f - ratio));
				b2->position = b2->position + n2 * (distancedifference * ratio);
			}
			else
			{
				float ratio = mass2 * mdivisor;
				ratio *= ratio;
				b1->position = b1->position + n1 * (distancedifference * ratio);
				b2->position = b2->position + n2 * (distancedifference * (1.0f - ratio));
			}
		}
		else
		{
			velocity1 -= n1 * energy2 * roughness;
			velocity2 -= n2 * energy1;

			//positions
			b1->position = b1->position + n1 * (distancedifference * 0.5f);
			b2->position = b2->position + n2 * (distancedifference * 0.5f);
		}

		if (!b1->Kinematic) b1->velocity = velocity1;

		if (!b2->Kinematic) b2->velocity = velocity2;


	}

}

void BalltoStaticBallCollision(ball* b1, ball* b2, float roughness)
{



	glm::vec2 dif = b2->position - b1->position;
	if (dif.x * dif.x + dif.y * dif.y < (b1->r + b2->r) * (b1->r + b2->r))
	{
		float dist = length(dif);
		float distancedifference = (b1->r + b2->r) - dist;
		if(dist<0.0001f)
		{
			b2->position.y += 0.0002f;
			dif = b2->position - b1->position;
			dist = length(dif);
			distancedifference = (b1->r + b2->r) - dist;
		}
		// all the normals
		glm::vec2 n2 = dif / dist;
		glm::vec2 n1 = -n2;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);



		//velocities
		// "untached" component of velocity
		glm::vec2 velocity1 = DOT(b1->velocity, CollisionLine) * CollisionLine;

		float energy1 = DOT(b1->velocity, n1);// basicly, a component of velocity, that is changed after collision

	
		velocity1 -= n1 * roughness * energy1;

		//positions
		b1->position = b1->position + n1 * (distancedifference);
		

		b1->velocity = velocity1;



	}

}

//Check if b1 collides with b2
bool BtBCollisionCheck(ball b1, ball b2)
{

	glm::vec2 dif = b1.position - b2.position;
	if (sqrlength(dif) < (b1.r + b2.r) * (b1.r + b2.r))
		return true;
	else
		return false;
}
bool BalltoPointCollisionCheck(ball b, glm::vec2 p)
{

	glm::vec2 dif = b.position - p;
	if (sqrlength(dif) < b.r * b.r)
		return true;
	else
		return false;
}