#pragma once
// this system will make them chaos to process per part, but very fast to process all at once (cuz array)
inline int lastid;
// Fuck oop, we ball through arrays only
inline std::array<glm::vec2, 100'000> ballPosition;
inline std::array<glm::vec2, 100'000> ballVelocity;
inline std::array<glm::vec2, 100'000> ballForce;
inline std::array<float,     100'000> ballSoundcd;
inline std::array<float,     100'000> ballTemp;
inline std::array<glm::vec2, 100'000> ballVelocityBuff;
inline std::array<int,       100'000> ballID;
inline std::array<bool,      100'000> IsBall;


inline std::array<glm::vec3, 100'000> StaticBalls;
inline std::array<glm::vec4, 100'000> StaticCubes;
inline int lastStaticBallid;
inline int lastStaticCubeid;

#define PARTSIZE 0.5f
// better to use array backwards, then pop_back() is pop_front();
inline std::list<int> freeBallIDs;

int NewBall();
void DeleteBall(int id);


void Strut(int b1, int b2, float length);
void SpringBetweenBalls(int b1, int b2, float length, float stiffnes, float absorbtion);
void Rope(int b1, int b2, float length);

bool BallToPointCollision(int b, glm::vec2 point);
bool BalltoStaticBallCollision(int id, ball* b);
bool BallToStaticQuadCollision(int id, cube* b);
bool BtBCollision(int id1, int id2);