
#include "../../Include/Helper.h"
#include "../../Include/Objects/Bodies.h"
#include "../../Include/Drawing.h"

void KinematicBody::init(int type, glm::vec4 color )
	{
		this->type = type;
		if (type == CIRCLE)
		{
			body.b.Kinematic = true;
			position = &body.b.position;

			ballsptr.push_back(&body.b);
		}
		else if (type == QUAD)
		{
			position = &body.c.position;
			cubes.push_back(&body.c);
		}
	}
void KinematicBody::Draw()
	{
		if (type == CIRCLE)
			DrawCircle(body.b.position, body.b.r, Color);
		else if (type == QUAD)
			DrawCube(body.c.position, glm::vec2(body.c.width, body.c.height), 0.0f, Color);
	}
void KinematicBody::PreProcess(float dt)
	{
		Draw();
		body.b.rotationVelocity = 0.0f;
		body.b.rotation = 0.0f;
		body.b.rotationForce = 0.0f;
	}
void KinematicBody::Process(float dt)
{
		PreProcess(dt);
}




void RigidBody::init(glm::vec2 position, float radius, glm::vec4 color)
	{
		body.position = position;
		body.r = radius;
		body.Kinematic = false;
		ballsptr.push_back(&body);
	}
void RigidBody::Draw()
	{
		//DrawBall(body, Color, Color2,false,BallNormalMapTexture,0);
	}
void RigidBody::PreProcess(float dt)
{
		Draw();
		body.Process(dt);
}
void RigidBody::Process(float dt)
	{
		PreProcess(dt);
	}


