#include "../../Include/Helper.h"
#include "../../Include/Objects/Ball.h"
void point::Process(float dt)
{
	position += velocity * dt;
	velocity += Force / mass * dt;
}
void ball::Process(float dt)
{
	position += velocity * dt;
	velocity += Force / mass * dt;
	rotation += rotationVelocity * dt;
	rotationVelocity += rotationForce / mass * dt;
}