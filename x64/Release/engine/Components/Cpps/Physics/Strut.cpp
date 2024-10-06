
#include "../../Include/Helper.h"
#include "../../Include/Objects.h"
#include "../../Include/Drawing.h"
#include "../../Include/Physics/Strut.h"
void Strut(ball* a, ball* b, float StrutLength, bool Draw)
{
	glm::vec2 posdif = b->position - a->position;
	
	float dist = length(posdif);
	if(dist<0.0001f)
	{
		b->position.y += 0.0002f;
		posdif = b->position - a->position;
		dist = length(posdif);
	}

	glm::vec2 norm = posdif/dist;

	glm::vec2 Difference = posdif - norm * StrutLength;



	a->position += Difference * 0.5f;
	b->position += -Difference * 0.5f;

	glm::vec2  velbuf = a->velocity;

	a->velocity -= DOT(a->velocity - b->velocity, norm) * norm * 0.5f;
	b->velocity -= DOT(b->velocity - velbuf, norm) * norm * 0.5f;

	if (Draw)
		DrawLine(a->position, b->position, (a->r + b->r) * 0.5f);
}
void MStrut(ball* a, ball* b, float StrutLength, bool Draw)
{
	glm::vec2 posdif = b->position - a->position;

	glm::vec2 norm = Normalize(posdif);

	glm::vec2 Difference = posdif - norm * StrutLength;

	float mdivisor = 1.0f / (a->mass + b->mass);

	float mmult1 = a->mass * mdivisor;
	float mmult2 = b->mass * mdivisor;

	a->position += Difference * mmult1;
	b->position += -Difference * mmult2;

	glm::vec2  velbuf = a->velocity;


	a->velocity -= DOT(a->velocity - b->velocity, norm) * norm * mmult1;
	b->velocity -= DOT(b->velocity - velbuf, norm) * norm * mmult2;

	if (Draw)
		DrawLine(a->position, b->position, (a->r + b->r) * 0.5f);
}

