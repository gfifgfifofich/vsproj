#pragma once

class Quest
{
public:
	int id = -1;
	// -1 no one, 0,1,2 fractions 4 pirates
	int fraction = -1;
	// -1 no one, 0,1,2 fractions 4 pirates
	int fraction2 = -1;
	//-1 no mission, other - link to quests[]
	int missiontype = -1;

	int state = 0;

	int size = -1;
	int dificulty = -1;
	int materialreward = 0;
	
	std::string name = "";
	std::string Description = "";

	std::map<std::string, bool> flags;
	std::map<std::string, int> counters;
	std::map<std::string, glm::vec2> positions;
	void Start();
	void Process(float dt);
};
inline long unsigned int lastquestid;
inline std::map<int, Quest> quests;

class Radar
{
public:
	float Scale = 5.0f; // 1 unit = 1'000 meters // may have been absolutly not used
	long unsigned int BleepsCounter = 0;
	float minSpawnRadius = 1000.0f;// 1km * Scale
	float maxSpawnRadius = 5000.0f;// 5km * Scale

	int enteredBleep = -1;

	float timer = 0.5f;

	float playerHeat = 0.0f;

	int MaxEnemybleeps = 15;
	int MaxFogBleeps = 15;

	glm::vec2 offset = { 0.0f,0.0f };


	struct Bleep
	{
		glm::vec2 position = { 0.0f,0.0f };
		glm::vec2 velocity = {0.0f,0.0f};
		std::string string = "";
		bool story = false;
		bool justheat = false;
		float fogamount = 0.0f;

		int size = 0;
		int dificulty = 0;
		int materialReward = 0;
		
		// -1 - station, 0 - nothing, 1 - enemy, 2 - story mission, else -  (id -3) = custom missionid
		int state = 0;
		// -1 no one, 0,1,2 fractions 4 pirates
		int fraction = -1;
		//-1 no mission, other - link to quests[]
		int questid = -1;
		//save file for string
		std::string SaveString = "";

		float t = 0.0f;
		float r = 2.5f;
		float colisionR = 10;
		bool infinite = false;
	};

	std::vector<Bleep> bleeps;

	void Spawn(glm::vec2 pos);
	void Init();
	void Process(float dt);
	void Draw(glm::vec2 screenpos);


};
inline Radar ActiveRadar;
