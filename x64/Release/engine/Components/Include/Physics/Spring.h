#pragma once
void Spring(point* p1, point* p2, glm::vec2 Difference, float stiffness, float absorption = 0.0f);
void SpringBetweenBalls(ball* a, ball* b, float springLength, float stiffness, float absorption = 0.0f);
void FixedSpringBetweenBalls(ball* a, ball* b, glm::vec2 SpringVector, float stiffness, float absorption = 0.0f);
void SpringBetweenPoints(point* a, point* b, float springLength, float stiffness, float absorption = 0.0f);
void SpringBallToPoint(ball* a, point* b, float springLength, float stiffness, float absorption = 0.0f);
void DrawSpring(glm::vec2 p1, glm::vec2 p2, float RequeredLength, float width = 1, float Z_Index = 0, bool lighted = false);
