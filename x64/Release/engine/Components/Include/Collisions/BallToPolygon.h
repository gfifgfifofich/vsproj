#pragma once
#include "../Objects.h"
#include "BallToBall.h"
#include "BallToLine.h"

void BallToTriangleCollision(ball* b, Triangle p);
bool BallToTriangleCollisionCheck(ball b, Triangle p);
bool PointToTriangleCollisionCheck(glm::vec2 b, Triangle p);
bool PointToPolygonCollisionCheck(glm::vec2 b, polygon p);
bool BallToPolygonCollisionCheck(ball b, polygon p);