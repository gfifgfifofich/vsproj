#include "../../Include/Helper.h"

#include "../../Include/Collisions/BallToPolygon.h"



void BallToTriangleCollision(ball* b, Triangle p)
{

	float roughness = b->roughness > p.roughness ? p.roughness : b->roughness;

	for (int i = 0; i < p.SubTriangles.size(); i++)
	{
		glm::vec2 normal1 = p.SubTriangles[i].l1.normal * (b->r + p.SubTriangles[i].l1.width);

		if (PointToLineSideCheck(b->position - normal1, p.SubTriangles[i].l1) &&
			PointToLineSideCheck(b->position, p.SubTriangles[i].l2) &&
			PointToLineSideCheck(b->position, p.SubTriangles[i].l3))
		{
			line l = p.SubTriangles[i].l1;



			glm::vec2 CP = b->position - l.normal * b->r; // collision point



			glm::vec2 PointOnLine;
			if ((l.P1.x - l.P2.x) * (l.P1.x - l.P2.x) > (l.P1.y - l.P2.y) * (l.P1.y - l.P2.y))
			{
				float dif_x = CP.x - l.P1.x;
				float rel_pos = dif_x / (l.P1.x - l.P2.x);
				float dif_y = (l.P1.y - l.P2.y) * rel_pos;
				PointOnLine = glm::vec2(dif_x, dif_y) + l.P1;
			}
			else
			{
				float dif_y = CP.y - l.P1.y;
				float rel_pos = dif_y / (l.P1.y - l.P2.y);
				float dif_x = (l.P1.x - l.P2.x) * rel_pos;
				PointOnLine = glm::vec2(dif_x, dif_y) + l.P1;
			}
			glm::vec2 newpos = PointOnLine + l.normal * (b->r + l.width * 1.0f);


			glm::vec2 LineVector = l.P1 - l.P2;
			glm::vec2 BodyVector = l.P1 - b->position;


			if (DOT(LineVector, BodyVector) <= sqrlength(LineVector) && DOT(LineVector, BodyVector) >= 0.0f)
			{
				BallToPointCollision(b, PointOnLine, roughness);
				b->position = newpos;
			}
		}
		else if (sqrlength(b->position - p.SubTriangles[i].l1.P1) < b->r * b->r)BallToPointCollision(b, p.SubTriangles[i].l1.P1, roughness);
		else if (sqrlength(b->position - p.SubTriangles[i].l2.P1) < b->r * b->r)BallToPointCollision(b, p.SubTriangles[i].l2.P1, roughness);
		else if (sqrlength(b->position - p.SubTriangles[i].l3.P1) < b->r * b->r)BallToPointCollision(b, p.SubTriangles[i].l3.P1, roughness);

	}
}
bool BallToTriangleCollisionCheck(ball b, Triangle p)
{


	bool colided =
		PointToLineSideCheck(b.position - p.lines[0].normal * b.r, p.lines[0]) &&
		PointToLineSideCheck(b.position - p.lines[1].normal * b.r, p.lines[1]) &&
		PointToLineSideCheck(b.position - p.lines[2].normal * b.r, p.lines[2]);
	if (colided) return true;
	else if (sqrlength(b.position - p.points[0]) < b.r * b.r)return true;
	else if (sqrlength(b.position - p.points[1]) < b.r * b.r)return true;
	else if (sqrlength(b.position - p.points[2]) < b.r * b.r)return true;
	return false;

}
bool PointToTriangleCollisionCheck(glm::vec2 b, Triangle p)
{
	if (PointToLineSideCheck(b, p.lines[0]) &&
		PointToLineSideCheck(b, p.lines[1]) &&
		PointToLineSideCheck(b, p.lines[2])) return true;
	return false;

}
bool PointToPolygonCollisionCheck(glm::vec2 b, polygon p)
{
	int i = 0;
	while (i < p.triangles.size())
	{
		if (PointToTriangleCollisionCheck(b, p.triangles[i]))
			return true;
		i++;
	}
	return false;

}
bool BallToPolygonCollisionCheck(ball b, polygon p)
{
	int i = 0;
	while (i < p.triangles.size())
	{
		if (BallToTriangleCollisionCheck(b, p.triangles[i]))
			return true;
		i++;
	}
	return false;

}