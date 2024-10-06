#include "../../Include/Helper.h"
#include "../../Include/Objects/VerletObject.h"
void VerletObject::UpdatePos()
{
	velocity = (cpos - ppos);

	ppos = cpos;


	cpos = cpos + velocity + (acceleration * deltatime);


	acceleration = glm::vec2(0.0f);
}