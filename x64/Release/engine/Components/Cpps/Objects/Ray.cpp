#include "../../Include/Helper.h"
#include "../../Include/Objects/Ray.h"

void Ray::normalize()
	{
		direction = Normalize(direction);

		step = length / Steps;
	}
void Ray::Update()
	{

		step = length / Steps;
		ClosestPoint = position + length * direction;

	}
Ray::Ray(glm::vec2 position, glm::vec2 direction, float length, int steps)
	{
		this->position = position;
		this->direction = direction;

		this->length = length;
		this->Steps = Steps;

		normalize();
		Update();
	}
Ray::Ray()
	{
		normalize();
		Update();
	}
