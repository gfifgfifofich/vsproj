#pragma once

#include "../Objects.h"
void BallToPointCollision(ball* b, glm::vec2 point, float roughness = 1.0f);
void BtBCollision(ball* b1, ball* b2, float roughness = 1.0f);
bool BtBCollisionCheck(ball b1, ball b2);
bool BalltoPointCollisionCheck(ball b, glm::vec2 p);
void BalltoStaticBallCollision(ball* b1, ball* b2, float roughness = 1.0f);