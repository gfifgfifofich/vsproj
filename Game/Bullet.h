#pragma once

class Bullet
{
public:
	DamageSphere body;


	float dyingtime = 1.5f;
	float lifetime = 3.0f;
	bool wasdead = false;
	bool forcedeath=  false;
	ball ball;
	float t = 0.0f;
	
	void Process(float dt);
	void Draw();
	void Dead();
	
};
inline std::vector<Bullet> bullets;

void ProcessBullets(float dt, bool draw);

void SpawnBullet(glm::vec2 position, glm::vec2 velocity, float damage, float size, float Heat = 1.0f,float recoil = 600.0f, int id = 0);

