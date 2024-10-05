#pragma once
#include "../Objects.h"
#include "BallToBall.h"
#include "BallToLine.h"
// collision ball-cube, but cube's velocity/position is unchanged
void BallToStaticQuadCollision(ball* b, cube quad);
int BtCCollisionCheckSides(ball b, cube c);
bool BtCCollisionCheck(ball b, cube c);
bool BtCCollisionCheck(ball* b, cube c);
bool CubeContainsBall(ball* b, cube c);
bool PointToQuadCollisionCheck(cube quad1, glm::vec2 p);
