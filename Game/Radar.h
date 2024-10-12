#pragma once

class Radar
{
	/*
	Strength of enemies will depend on storyint
	the further player in compaign, the harder enemies will spawn

	*/
public:
	float Scale = 5.0f; // 1 unit = 1'000 meters 

	float minSpawnRadius = 1000.0f;// 1km * Scale
	float maxSpawnRadius = 5000.0f;// 5km * Scale

	float timer = 0.5f;

	float playerHeat = 0.0f;

	int MaxEnemybleeps = 5;
	int MaxFogBleeps = 15;

	glm::vec2 offset = {0.0f,0.0f};

	struct Bleep
	{
		glm::vec2 position = { 0.0f,0.0f };
		glm::vec2 velocity = {0.0f,0.0f};
		std::string string = "";
		bool story = false;
		bool justheat = false;
		float fogamount = 0.0f;
		// 0 - nothing, 1 - enemy, 2 - story mission, else -  id -3 = custom missionid
		int state = 0;
		float t = 0.0f;
		float r = 2.5f;
		bool infinite = false;
	};

	std::vector<Bleep> bleeps;

	void Spawn(glm::vec2 pos);
	void Init();
	void Process(float dt);
	void Draw(glm::vec2 screenpos);


};