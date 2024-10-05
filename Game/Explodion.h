#pragma once
struct Explodion
{
	DamageSphere DS;

	float timeLeft = 0.1f;
	float lifet = 1.0f;


};

inline std::vector<Explodion>ExplodionArray;
inline std::vector<Explodion*>Explodions;


Explodion NewExplodion(glm::vec2 position, float r, float dmg, float lifetime,float recoil = 40.0f);

void SpawnExplodion(glm::vec2 position, float r, float dmg, float lifetime, float recoil = 40.0f);

void ProcessExplodions(float dt);

void DrawExplodions(float dt);
