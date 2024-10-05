
#include "../../Include/Helper.h"
#include "../../Include/Objects.h"
#include "../../Include/Drawing.h"
#include "../../Include/Physics/Rope.h"

void Rope(ball* a, ball* b, float maxlength)
{

	glm::vec2 posdif = b->position - a->position;
	if (sqrlength(posdif) > maxlength * maxlength)
	{
		float dist = length(posdif);
		if(dist<0.0001f)
		{
			b->position.y += 0.0002f;
			posdif = b->position - a->position;
			dist = length(posdif);
		}
		glm::vec2 norm = posdif/dist;

		glm::vec2 Difference = posdif - norm * maxlength;



		a->position += Difference * 0.5f;
		b->position += -Difference * 0.5f;

		float dot1 = DOT(a->velocity - b->velocity, norm);
		float dot2 = DOT(b->velocity - a->velocity, norm);

		if (dot1 < 0.0f)
		{
			a->velocity -= dot1 * norm * 0.5f;
			b->velocity -= dot2 * norm * 0.5f;
		}
	}
}
void MRope(ball* a, ball* b, float maxlength)
{

	glm::vec2 posdif = b->position - a->position;
	if (sqrlength(posdif) > maxlength * maxlength)
	{
		float dist = length(posdif);
		if(dist<0.0001f)
		{
			b->position.y += 0.0002f;
			posdif = b->position - a->position;
			dist = length(posdif);
		}
		glm::vec2 norm = posdif/dist;

		glm::vec2 Difference = posdif - norm * maxlength;


		float mdivisor = 1.0f / (a->mass + b->mass);

		float mmult1 = a->mass * mdivisor;
		float mmult2 = b->mass * mdivisor;

		a->position += Difference * mmult1;
		b->position += -Difference * mmult2;

		float dot1 = DOT(a->velocity - b->velocity, norm);
		float dot2 = DOT(b->velocity - a->velocity, norm);

		if (dot1 < 0.0f)
		{
			a->velocity -= dot1 * norm * mmult1;
			b->velocity -= dot2 * norm * mmult2;
		}

	}
}

void DecorativeRope::Init(ball* b1, ball* b2, float length )
{
	this->b1 = b1;
	this->b2 = b2;
	this->length = length;

	for (int i = 0; i < 5; i++)
		rope[i].position = b1->position + i * 0.2f * (b2->position - b1->position);
}
void DecorativeRope::Process(float dt)
{
	for (int s = 0; s < 2; s++)
	{
		rope[0].position = b1->position;
		rope[0].velocity = b1->velocity;
		rope[0].Force = b1->Force;
		for (int i = 1; i < 5; i++)
		{
			rope[i].Force = Force;
			rope[i].Process(delta * 0.5f);

			rope[4].position = b2->position;
			rope[4].velocity = b2->velocity;
			rope[4].Force = b2->Force;
			Rope(&rope[i], &rope[i - 1], length * 0.2f);

		}
	}
}
void DecorativeRope::Draw(int Z_Index)
{
	rope[0].position = b1->position;
	rope[4].position = b2->position;
	if (Texture == NULL)
	{
		/*for (int i = 0; i < 5; i++)
			DrawCircle(rope[i].position, width, color, true, BallNormalMapTexture, Z_Index - 1);*/
		for (int i = 1; i < 5; i++)
			DrawLine(rope[i].position, rope[i - 1].position, width, color, true, NormalMap, Z_Index);
	}
	else
		for (int i = 1; i < 5; i++)
			DrawTexturedLine(Texture, rope[i].position, rope[i - 1].position, width, color, NormalMap, Z_Index);
}