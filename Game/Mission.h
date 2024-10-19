#include <array>
enum MissionType 
{
	mining =1,
	retrival =2,
	pirates =3,
	infestation =4
};



class Mission
{
	public:
	bool exitedmission = false;
	int type = 0;
	int dificulty = 0;
	int size = 0;
	int materialReward = 0;
	bool story_mission = false;
	bool exiting = false;
	bool compleated = true;
	std::vector<std::vector<CentralPart*>> Bots;
	std::vector<std::vector<Node*>> NodeHandles;
	std::vector<glm::vec4> TakenAreas;
	std::vector<float> timers;
	std::map<std::string, bool> flags;

	glm::vec2 missionpos = { 0.0f,0.0f };

	//Shit for missions (one, tutorial.)
	glm::vec2 planetpos = {0.0f,0.0f};



	void Start();
	void UpdateScene();
	void CheckShips(float dt);
	void Process(float dt);
	void exitMission(bool extracted = false);
	void MissionClear();
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
	int state = 0;

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