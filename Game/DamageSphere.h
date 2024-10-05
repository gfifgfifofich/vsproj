#pragma once


struct DamageSphere
{
	ball body;

	float Damage = 1.0f;
	float Heat = 1.0f;
	float friction = 1.0f;
	float timeLeft = 1.0f;

	float recoil = 600.0f;

	bool infinite = false;
	bool singleHit = true;
	bool dead = false;
	int id = 0;
	bool neutral = false;

};

inline std::vector<DamageSphere>DamageSpheresArray;
inline std::vector<DamageSphere*>DamageSpheres;

void ProcessDamageSpheres(float dt);