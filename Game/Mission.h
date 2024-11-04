#include <array>
enum MissionType 
{
	mining =1,
	retrival =2,
	pirates =3,
	infestation = 4,
	Transportation = 5,
	Search = 6,
	SupplySabotage = 7,
	DataRecovery = 8,
	PartRecovery = 9,
};



class Mission
{
	public:
	bool exitedmission = false;
	int type = 0;
	int dificulty = 0;
	int size = 0;
	int questid = -1;
	int materialReward = 0;
	bool story_mission = false;
	bool exiting = false;
	bool compleated = true;
	bool outside = false;
	std::vector<std::vector<CentralPart*>> Bots;
	std::vector<std::vector<Node*>> NodeHandles;
	std::vector<glm::vec4> TakenAreas;
	std::vector<float> timers;
	std::map<std::string, bool> flags;
	std::map<std::string, int> Counters;
	std::map<std::string, float> values;

	glm::vec2 missionpos = { 0.0f,0.0f };

	//Shit for missions (one, tutorial.)
	glm::vec2 planetpos = {0.0f,0.0f};


	//int materials = 0;
	////Modifiers to needs
	//int	deliveryQuests = 0;  //[0, 1, 2, 0]
	//int	FightingQuests = 0;  //[1, 2, 0, 1]
	//int	RNDQuests = 0;  //[0, 1, 0, 2]
	//int	TotalSize = 0;  //[1, 1, 1, 0]
	//
	//	//supply of Needs
	//int Housing = 0;  //(3)[0, 0, 0, 0]
	//int Processing = 0;  //(2)[1, 0, 1, 0]
	//int Cargo = 0;  //(2)[0, 0, 0, 0]
	//int Labs = 0;  //(1)[1, 1, 0, 0]

	int GetNextBlockType();
	void SpawnBlock(int bt);


	void Start();
	void UpdateScene();
	void CheckShips(float dt);
	void Process(float dt);
	void exitMission(bool extracted = false);
	void MissionClear();
	void Save(std::string filename);
	void Load(std::string filename, bool ingame = true);
};

inline Mission CurrnetMission;


class Shop
{
public:
	int window = -1;

	void Process(float dt);
	};
class MissionSelectScreen
{
public:

	int window = -1;
	std::string filenamestring = "";
	std::string erroutputstring = "";
	std::vector<Mission> missions;

	glm::vec2 missionPosition = { 0.0f,0.0f };
	bool Hub = true;
	bool missionSelected = false;
	bool missionStory = false;
	int missionSize = 0;
	int missionDificulty = 0;
	int missionType = 0;

	void GenerateNewMissions();
	void Process(float dt);
};
class SaveScreen
{
public:
	int window = -1;
	std::string filenamestring = "";
	std::string erroutputstring = "";
	int state = 0;
	void Process(float dt);
};

inline MissionSelectScreen MissionSelectMenu;
inline SaveScreen SaveScreenmenu;
inline Shop shopmenu;