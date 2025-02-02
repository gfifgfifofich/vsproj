#include "../engine/Components/Redactor.h"
#include "../engine/Components/Include/sounds.h"
#include "smallball.h"

/*
Some physics interactions lead to nans
like spawning inside a cube
*/

const float collisionRoughness = 0.95f;

int NewBall()
{
	int id = -1;
	if (freeBallIDs.size() > 0)
	{
		id = freeBallIDs.front();
		freeBallIDs.pop_front();
	}
	else
		id = lastid + 1;

	if (id >= 100'000)
		return -1;

	//failsafe
	while (IsBall[id])
	{
		if (freeBallIDs.size() > 0)
		{
			freeBallIDs.pop_front();
			id = freeBallIDs.front();
		}
		else
			id++;

		if (id >= 100'000)
			return -1;
	}

	ballPosition[id] = { 0.0f,0.0f };
	ballVelocity[id] = { 0.0f,0.0f };
	ballForce[id] = { 0.0f,0.0f };
	ballSoundcd[id] = 0.0f;
	ballTemp[id] = 0.0f;
	ballVelocityBuff[id] = { 0.0f,0.0f };
	IsBall[id] = true;
	if (id > lastid)
		lastid = id;
	return id;
}

void DeleteBall(int id)
{
	IsBall[id] = false;
	freeBallIDs.push_front(id);
}

void Strut(int b1, int b2, float StrutLength)
{
	if (b1 < 0 || b1 >= 100000 || b2 < 0 || b2 >= 100000)
		return;
	glm::vec2 posdif = ballPosition[b2] - ballPosition[b1];

	float dist = length(posdif);
	if (dist < 0.0001f)
	{
		ballVelocity[b2].y += 0.0002f;
		posdif = ballPosition[b2] - ballPosition[b1];
		dist = length(posdif);
	}

	glm::vec2 norm = posdif / dist;

	glm::vec2 Difference = posdif - norm * StrutLength;



	ballPosition[b1] += Difference * 0.5f;
	ballPosition[b2] += -Difference * 0.5f;

	glm::vec2  velbuf = ballVelocity[b1];

	ballVelocity[b1] -= DOT(ballVelocity[b1] - ballVelocity[b2], norm) * norm * 0.5f;
	ballVelocity[b2] -= DOT(ballVelocity[b2] - velbuf, norm) * norm * 0.5f;
}
void SpringBetweenBalls(int b1, int b2, float springLength, float stiffnes, float absorbtion)
{

	if (b1 < 0 || b1 >= 100000 || b2 < 0 || b2 >= 100000)
		return;
	glm::vec2 posdif = ballPosition[b2] - ballPosition[b1];
	float dist = length(posdif);
	glm::vec2 Difference = posdif - (posdif / dist) * springLength;
	glm::vec2 Change1 = Difference * 0.5f * stiffnes;
	glm::vec2 Change2 = -Difference * 0.5f * stiffnes;
	if (dist < 0.0001f)
	{
		ballPosition[b2].y += 0.0002f;
		posdif = ballPosition[b2] - ballPosition[b1];
		dist = length(posdif);
	}
	glm::vec2 norm = posdif / dist;


	glm::vec2 vel1 = ballVelocity[b1] - ballVelocity[b2];
	glm::vec2 vel2 = ballVelocity[b2] - ballVelocity[b1];


	glm::vec2 axis1 = DOT(vel1, norm) * norm;
	glm::vec2 axis2 = DOT(vel2, norm) * norm;


	ballForce[b1] += Change1 - axis1 * absorbtion;
	ballForce[b2] += Change2 - axis2 * absorbtion;

}
void Rope(int b1, int b2, float maxlength)
{

	if (b1 < 0 || b1 >= 100000 || b2 < 0 || b2 >= 100000)
		return;
	glm::vec2 posdif = ballPosition[b2] - ballPosition[b1];
	if (sqrlength(posdif) > maxlength * maxlength)
	{
		float dist = length(posdif);
		if (dist < 0.0001f)
		{
			ballPosition[b2].y += 0.0002f;
			posdif = ballPosition[b2] - ballPosition[b1];
			dist = length(posdif);
		}
		glm::vec2 norm = posdif / dist;

		glm::vec2 Difference = posdif - norm * maxlength;



		ballPosition[b1] += Difference * 0.5f;
		ballPosition[b2] += -Difference * 0.5f;

		float dot1 = DOT(ballVelocity[b1] - ballVelocity[b2], norm);
		float dot2 = DOT(ballVelocity[b2] - ballVelocity[b1], norm);

		if (dot1 < 0.0f)
		{
			ballVelocity[b1] -= dot1 * norm * 0.5f;
			ballVelocity[b2] -= dot2 * norm * 0.5f;
		}
	}
}

bool BallToPointCollision(int b, glm::vec2 point)
{
	if (b < 0 || b >= 100000)
		return false;
	if (sqrlength(ballPosition[b] - point) < PARTSIZE * PARTSIZE && sqrlength(ballPosition[b] - point) != 0.0f)
	{
		glm::vec2 dif = glm::vec2(0.0f);

		dif = ballPosition[b] - point;


		float dist = length(dif);
		float distancedifference = PARTSIZE - dist;

		glm::vec2 n1 = -dif / dist;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);

		float energy1 = DOT(ballVelocity[b], n1);
		glm::vec2 velocity1 = DOT(ballVelocity[b], CollisionLine) * CollisionLine;

		velocity1 = velocity1 * collisionRoughness;

		velocity1 -= n1 * energy1 * collisionRoughness;

		ballVelocity[b] = velocity1;
		ballPosition[b] += (dif / dist) * distancedifference;
		return true;
	}
	return false;
}
bool BalltoStaticBallCollision(int b1, ball* b2)
{
	if (b1 < 0 || b1 >= 100000 || b2 ==nullptr)
		return false;
	glm::vec2 dif = b2->position - ballPosition[b1];
	if (dif.x * dif.x + dif.y * dif.y < (PARTSIZE + b2->r) * (PARTSIZE + b2->r))
	{
		float dist = length(dif);
		float distancedifference = (PARTSIZE + b2->r) - dist;
		if (dist < 0.0001f)
		{
			b2->position.y += 0.0002f;
			dif = b2->position - ballPosition[b1];
			dist = length(dif);
			distancedifference = (PARTSIZE + b2->r) - dist;
		}
		// all the normals
		glm::vec2 n2 = dif / dist;
		glm::vec2 n1 = -n2;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);



		//velocities
		// "untached" component of velocity
		glm::vec2 velocity1 = DOT(ballVelocity[b1], CollisionLine) * CollisionLine;

		float energy1 = DOT(ballVelocity[b1], n1);// basicly, a component of velocity, that is changed after collision


		velocity1 = velocity1 * collisionRoughness;
		velocity1 -= n1 * collisionRoughness * energy1;

		//positions
		ballPosition[b1] = ballPosition[b1] + n1 * (distancedifference);


		ballVelocity[b1] = velocity1;
		return true;
	}
	return false;
}
bool BallToStaticQuadCollision(int b, cube* c)
{

	if (b < 0 || b >= 100000 )
		return false;
	float roughness = collisionRoughness;
	glm::vec2 posdifference = c->position - ballPosition[b];

	if (posdifference.x > c->width)
		posdifference.x = c->width;
	if (posdifference.x <= -c->width)
		posdifference.x = -c->width;
	if (posdifference.y > c->height)
		posdifference.y = c->height;
	if (posdifference.y <= -c->height)
		posdifference.y = -c->height;



	if (c->position - posdifference != ballPosition[b])
	{
		return BallToPointCollision(b, c->position - posdifference);
	}
	else
	{
		bool plane1, plane2;// y = x, y=-x;
		glm::vec2 relpos = ballPosition[b] - c->position;
		relpos.x *= c->height / c->width;
		if (relpos.y < relpos.x)
			plane1 = false;// under
		else
			plane1 = true; // above
		if (relpos.y < -relpos.x)
			plane2 = false;// under
		else
			plane2 = true; // above


		if (plane1 && !plane2)
		{//left
			ballPosition[b].x = c->position.x - c->width -  PARTSIZE * 0.5f;
		}
		if (plane1 && plane2)
		{//up 
			ballPosition[b].y = c->position.y + c->height + PARTSIZE * 0.5f;
		}
		if (!plane1 && plane2)
		{//right 
			ballPosition[b].x = c->position.x + c->width +  PARTSIZE * 0.5f;
		}
		if (!plane1 && !plane2)
		{//down 
			ballPosition[b].y = c->position.y - c->height - PARTSIZE * 0.5f;
		}
		posdifference = c->position - ballPosition[b];

		if (posdifference.x > c->width)
			posdifference.x = c->width;
		else if (posdifference.x < -c->width)
			posdifference.x = -c->width;
		if (posdifference.y > c->height)
			posdifference.y = c->height;
		else if (posdifference.y < -c->height)
			posdifference.y = -c->height;

		return BallToPointCollision(b, c->position - posdifference);
	
	}

	return false;
}
bool BtBCollision(int b1, int b2)
{
	if (b1 < 0 || b1 >= 100000 || b2 < 0 || b2 >= 100000)
		return false;
	glm::vec2 dif = ballPosition[b2]- ballPosition[b1];
	if (dif.x * dif.x + dif.y * dif.y < (PARTSIZE+PARTSIZE) * (PARTSIZE + PARTSIZE))
	{
		float dist = length(dif);
		float distancedifference = (PARTSIZE + PARTSIZE) - dist;
		if (dist < 0.0001f)
		{
			ballPosition[b2].y += 0.0002f;
			dif = ballPosition[b2]- ballPosition[b1];
			dist = length(dif);
			distancedifference = (PARTSIZE + PARTSIZE) - dist;
		}
		// all the normals
		glm::vec2 n2 = dif / dist;
		glm::vec2 n1 = -n2;
		glm::vec2 CollisionLine = glm::vec2(-n1.y, n1.x);



		//velocities
		// "untached" component of velocity
		glm::vec2 velocity1 = DOT(ballVelocity[b1], CollisionLine) * CollisionLine;
		glm::vec2 velocity2 = DOT(ballVelocity[b2], -CollisionLine) * -CollisionLine;

		float energy1 = DOT(ballVelocity[b1], n1);// basicly, a component of velocity, that is changed after collision
		float energy2 = DOT(ballVelocity[b2], n2);

		
		velocity1 -= n1 * energy2;
		velocity2 -= n2 * energy1;

		//positions
		ballPosition[b1] = ballPosition[b1] + n1 * (distancedifference * 0.5f);
		ballPosition[b2] = ballPosition[b2] + n2 * (distancedifference * 0.5f);
		

		ballVelocity[b1]= velocity1;
		ballVelocity[b2]= velocity2;
		return true;
	}
	return false;
}