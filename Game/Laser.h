#pragma once


#include "Parts/Base.h"


class Laser
{
public:
	DamageSphere body;

	glm::vec4 color = glm::vec4(1.0f, 10.0f, 100.0f, 1.0f);

	
	
	float Intensity = 1.0f;

	float time = 0.0f;
	float maxtime = 0.0f;

	float width = 10.0f;

	bool inf = false;

	int fraction = 0;
	


	Ray RayCast; 
	Laser(glm::vec2 position, glm::vec2 direction, float length, int steps);
	void Process(float dt);
	
	void Draw();
	
	void Dead();
	~Laser();
	
};
inline std::vector<Laser> Lasers;
inline std::vector<Laser*> LaserPtrs;

void ProcessLasers(float dt, bool draw, bool updateRaycasts = false);

void SpawnLaser(int fraction, glm::vec2 position, glm::vec2 direction, float damage, float size, float Heat = 1.0f, float time = 0.0f, float recoil = 600.0f, int id = 0 , float length = 50.0f);

