#pragma once
class Rocket
{
public:
	DamageSphere DS;
	float timeLeft = 10.0f;
	float lifet = 10.0f;
	ball body[2];
	bool fired = false;
	bool Exploded = false;
	float dmg = 0.0f;
	float ExplodionForce = 0.0f;
	glm::vec2 target = glm::vec2(0.0f);
	glm::vec2 dir;
	glm::vec2 mid;
	glm::vec2 midvel;
	float throtle = 0.0f;
	float explodionR = 5.0f;
	
	bool ThrowParticles = false;
	float T = 0.015f;

	Rocket();
	void Init(glm::vec2 position,glm::vec2 dir, float r, float dmg, float lifetime);
	void Process(float dt);
	void Explode();
	void Draw();
	
};

inline std::vector<Rocket>RocketArray;
inline std::vector<Rocket*>Rockets;


Rocket* NewRocket(glm::vec2 position,glm::vec2 dir, float r, float dmg, float lifetime);

void ProcessRockets(float dt);
