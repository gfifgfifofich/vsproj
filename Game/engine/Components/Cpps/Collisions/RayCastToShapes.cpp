
#include "../../Include/Helper.h"

#include "../../Include/Collisions/RayCastToShapes.h"

bool CastRayToQuad(Ray* r, cube c)
{
	glm::vec2 CollisionPosition = r->position;

	bool pass = true;

	glm::vec2 normal = glm::vec2(-r->direction.y, r->direction.x);

	bool aa = DOT(c.position - glm::vec2(c.width, c.height) - r->position, normal) <= 0.0f;
	bool bb = DOT(c.position - glm::vec2(c.width, -c.height) - r->position, normal) <= 0.0f;
	bool cc = DOT(c.position - glm::vec2(-c.width, c.height) - r->position, normal) <= 0.0f;
	bool dd = DOT(c.position - glm::vec2(-c.width, -c.height) - r->position, normal) <= 0.0f;

	if ((aa && bb && cc && dd) || (!aa && !bb && !cc && !dd))
	{
		pass = false;
		CollisionPosition = CollisionPosition + r->direction * r->length;
	}
	/*if ((DOT(c.position - glm::vec2(c.width, c.height) - r->position, normal) <= 0.0f))
	{
		pass = false;
		CollisionPosition = r->position;
	}*/
	if (PointToQuadCollisionCheck(c, r->position))
	{
		pass = false;
		CollisionPosition = r->position;
	}
	if (pass)
	{
		float step = r->step;
		for (int it = 0; it < r->iterations; it++)
		{

			int i = 0;
			bool stop = false;
			while (i < r->Steps && !stop)
			{
				i++;

				glm::vec2 pnt = CollisionPosition + r->direction * step * (float)i;

				stop = PointToQuadCollisionCheck(c, pnt);
			}
			if (!stop)
			{
				CollisionPosition = CollisionPosition + r->direction * step * (float)(i);
				if (sqrlength(r->position - CollisionPosition) < sqrlength(r->position - r->ClosestPoint))
				{
					r->ClosestPoint = CollisionPosition;
					return true;
				}
			return false;
			}
			else
			{
				CollisionPosition = CollisionPosition + r->direction * step * (float)(i - 1.0f);
				step /= r->Steps;

			}
		}
	}
	if (sqrlength(r->position - CollisionPosition) < sqrlength(r->position - r->ClosestPoint))
		{
			r->ClosestPoint = CollisionPosition;
			return true;
		}
	return false;
}
bool CastRayToBall(Ray* r, ball b)
{
	glm::vec2 CollisionPosition = r->position;

	glm::vec2 normal = glm::vec2(-r->direction.y, r->direction.x);

	bool pass = true;

	if ((DOT(b.position - normal * b.r - r->position, normal) <= 0.0f ==
		DOT(b.position + normal * b.r - r->position, normal) <= 0.0f))
	{
		pass = false;
		CollisionPosition = CollisionPosition + r->direction * r->length;
	}
	if (sqrlength(b.position - r->position) > (b.r + r->length) * (b.r + r->length))
	{
		pass = false;
		CollisionPosition = CollisionPosition + r->direction * r->length;
	}
	if ((DOT(b.position - b.r * r->direction - r->position, r->direction) <= 0.0f))
	{
		pass = false;
		CollisionPosition = CollisionPosition + r->direction * r->length;
	}
	if (BalltoPointCollisionCheck(b, r->position))
	{
		pass = false;
		r->ClosestPoint = r->position;
		return true;
	}
	if (pass)
	{
		float step = r->step;
		for (int it = 0; it < r->iterations; it++)
		{

			int i = 0;
			bool stop = false;
			while (i < r->Steps && !stop)
			{
				i++;

				glm::vec2 pnt = CollisionPosition + r->direction * step * (float)i;

				stop = BalltoPointCollisionCheck(b, pnt);
			}
			if (!stop)
			{
				CollisionPosition = CollisionPosition + r->direction * step * (float)(i);
				if (sqrlength(r->position - CollisionPosition) < sqrlength(r->position - r->ClosestPoint))
				{
					r->ClosestPoint = CollisionPosition;
					return true;
				}
				return false;
			}
			else
			{
				CollisionPosition = CollisionPosition + r->direction * step * (float)(i - 1.0f);
				step /= r->Steps;

			}
		}
	}

	if (sqrlength(r->position - CollisionPosition) < sqrlength(r->position - r->ClosestPoint))
		{
			r->ClosestPoint = CollisionPosition;
			return true;
		}
	return false;
}
bool CastRayToTriangle(Ray* r, Triangle t)
{
	glm::vec2 CollisionPosition = r->position;

	float step = r->step;

	for (int it = 0; it < r->iterations; it++)
	{

		int i = 0;
		bool stop = false;
		while (i < r->Steps && !stop)
		{
			i++;

			glm::vec2 pnt = CollisionPosition + r->direction * step * (float)i;

			stop = PointToTriangleCollisionCheck(pnt, t);
		}
		if (!stop)
		{
			CollisionPosition = CollisionPosition + r->direction * step * (float)(i);
			if (sqrlength(r->position - CollisionPosition) < sqrlength(r->position - r->ClosestPoint))
			{
				r->ClosestPoint = CollisionPosition;
				return true;
			}
			return false;
		}
		else
		{
			CollisionPosition = CollisionPosition + r->direction * step * (float)(i - 1.0f);
			step /= r->Steps;

		}
	}
	if (sqrlength(r->position - CollisionPosition) < sqrlength(r->position - r->ClosestPoint))
		{
			r->ClosestPoint = CollisionPosition;
			return true;
		}
	return false;
}