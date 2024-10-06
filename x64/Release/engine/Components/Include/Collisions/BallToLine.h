#pragma once
#include "../Objects.h"
#include "BallToBall.h"

bool PointToLineSideCheck(glm::vec2 p, line l);
void BallToLineCollision(ball* b, line l);
//if ball is below line, and in its range, it'll go up. Perfect for ground
void BallToGroundLineCollision(ball* b, line l);