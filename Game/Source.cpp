/*
TODO:
{

	
	more parts
	{
		Shields
		cannon, autocannon
		fix laser
		impulse laser, rapidfire impulse laser
		shileds plates/armour
		bigger rocket engines, multidirectional?, axis alligner/glider
		logic&math modules + input modules2 
	} 

	| Simulations - minigames to unlock new parts? ( race tracks for new engines, wavedeffence for new weapons?)
	| Parts research, like in reassembly is perfect (is it?), just another window
	
	
	story missions are different/ special
	grid missions 

	lots of textures
	AI
	UI

	Instanced lighting 
	Memory Optimizations
	{
		ball[100'000] on stack, parts have int[] ids to the vector; or something

		array of structs instead of Nodes[] (each class has its own vector<Class>, not vector<Class*> ), processor's memory grabber will optimise it good enough
	}
}

resources - one material (matter or whatever)

Missions, base location, resources. Parts cost materials, missions give. Retrieving vehicle returns all resorces.....

Mission ideas:
	retrive part/object/debrie ?? 
	kill N crafts ("Pirates") // mostly implemented
	kill N bugs ("Infestation") // make bug parts, modify central part, piratemission with bug crafts, done
	
	defend building/mine for N minutes/waves (or factory/mine, the longer its alive, the more resources player gets)

Mission
{
	Random objects (static asteroids - DONE, planets, fog fields that slow down...)
	Debrie (Random parts for free mb)
	Matter deposits (geodes?)(Extra reward for exploration)


	bool retrival necessary ( retrieve to get resources or not) // probably always necessary

	int dificulty (tier/amount of enemies)  
	int size (amount of enemies)  
	// mb dificulty * size 
	border - killing fog

	enum int type // mining, retrival, defeat pirates, clear bugs etc

}
*/


/*



CentralPart == Entity;
{
	Connections[]
}


*/


#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"

DataStorage GameSaveFile;
int Materials = 0;
int storyint = 0;
std::string lastEntityName = "Save0.sav";
std::string EntityBackUpName = "PreMissionBackup";
std::string GameSaveName = "Save0";
std::vector<std::string> ConsoleTexts;

float AccumulatedHeat = 0.0f;
bool inside = false;

struct shipInfo
{
	std::string filename = "";
	int MateralCost = 0;

};
std::vector <shipInfo> ShipInfos;
void LoadPlayerData()
{
	ShipInfos.clear();
	GameSaveFile.Load("SaveFile.sav");	
	lastEntityName = GameSaveFile.GetProperty(GameSaveName,"lastEntityName");
	EntityBackUpName = GameSaveFile.GetProperty(GameSaveName,"EntityBackUpName");
	Materials = GameSaveFile.GetPropertyAsFloat(GameSaveName,"Materials");
	storyint = GameSaveFile.GetPropertyAsFloat(GameSaveName,"storyint");
	
	std::vector<std::string> shipfilenames = GameSaveFile.GetProperties("ShipFileNames");
	for(int i=0;i< shipfilenames.size();i++)
	{
		shipInfo si;
		si.filename = shipfilenames[i];
		si.MateralCost = GameSaveFile.GetPropertyAsFloat("ShipFileNames",shipfilenames[i]);
		ShipInfos.push_back(si);
	}
}
void SavePlayerData()
{
	GameSaveFile.SetProperty(GameSaveName,"lastEntityName",lastEntityName);
	GameSaveFile.SetProperty(GameSaveName,"Materials",Materials);
	GameSaveFile.SetProperty(GameSaveName,"EntityBackUpName",EntityBackUpName);
	GameSaveFile.SetProperty(GameSaveName,"storyint",storyint);
	GameSaveFile.SetProperty("LastSave","LastSaveName","Save0");

	for(int i=0;i<ShipInfos.size();i++)
		GameSaveFile.SetProperty("ShipFileNames",ShipInfos[i].filename,ShipInfos[i].MateralCost);

	GameSaveFile.Save("SaveFile.sav");
}




#define PARTSIZE 0.5f

int BackgroundWindowID = -1;
int ForeWindowID = -1;
int TerminalWindowID = -1;
int MenuWindowID = -1;
inline glm::vec2 foregroundMousePosition = {0.0f,0.0f};

inline float TextSize = 1.0f;
inline float UISize = 18.0f;

#include "Textures.h"

void LoadTextures()
{
	LoadTexture("./Textures/Connections/HeatPipe.png", &HeatPipeTexture);
	LoadTexture("./Textures/Connections/HeatPipe NormalMap.png", &HeatPipeNormalMap);

	LoadTexture("./Textures/Connections/Rope.png", &RopeTexture);
	LoadTexture("./Textures/Connections/Rope NormalMap.png", &RopeNormalMap);

	LoadTexture("./Textures/Connections/Strut.png", &StrutTexture);
	LoadTexture("./Textures/Connections/Strut NormalMap.png", &StrutNormalMap);

	LoadTexture("./Textures/Misc/Pipe/Pipe.png", &PipeTexture);
	LoadTexture("./Textures/Misc/Pipe/Pipe NormalMap.png", &PipeNormalMap);

	LoadTexture("./Textures/Parts/RocketEngine/RocketEngine.png", &RocketEngineTexture);
	LoadTexture("./Textures/Parts/RocketEngine/RocketEngine NormalMap.png", &RocketEngineNormalMap);

	LoadTexture("./Textures/Parts/Radiator/Radiator.png", &RadiatorTexture);
	LoadTexture("./Textures/Parts/Radiator/Radiator NormalMap.png", &RadiatorNormalMap);

	LoadTexture("./Textures/Parts/Gun/Gun.png", &GunTexture);
	LoadTexture("./Textures/Parts/Gun/Gun NormalMap.png", &GunNormalMap);

	LoadTexture("./Textures/Parts/LaserGun/LaserGun.png", &LaserGunTexture);
	LoadTexture("./Textures/Parts/LaserGun/LaserGun NormalMap.png", &LaserGunNormalMap);

	LoadTexture("./Textures/Parts/MiniGun/MiniGun.png", &MiniGunTexture);
	LoadTexture("./Textures/Parts/MiniGun/MiniGun NormalMap.png", &MiniGunNormalMap);

	LoadTexture("./Textures/Parts/Gun/GunBase.png", &GunBaseTexture);
	LoadTexture("./Textures/Parts/Gun/GunBase NormalMap.png", &GunBaseNormalMap);

	LoadTexture("./Textures/Parts/RocketLauncher/RocketLauncher.png", &RocketLauncherTexture);
	LoadTexture("./Textures/Parts/RocketLauncher/RocketLauncher NormalMap.png", &RocketLauncherNormalMap);

	LoadTexture("./Textures/Parts/BallBody/BallBody.png", &BallBodyTexture);
	LoadTexture("./Textures/Parts/BallBody/BallBody NormalMap.png", &BallBodyNormalMap);

	LoadTexture("./Textures/Parts/CentralPart/CentralPart.png", &CentralPartTexture);
	LoadTexture("./Textures/Parts/CentralPart/CentralPart NormalMap.png", &CentralPartNormalMap);

	LoadTexture("./Textures/Parts/Rotor/Rotor.png", &RotorTexture);
	LoadTexture("./Textures/Parts/Rotor/Rotor NormalMap.png", &RotorNormalMap);


	LoadTexture("./Textures/Misc/Debrie/debrie0.png", &Debrie0Texture);
	LoadTexture("./Textures/Misc/Debrie/debrie1.png", &Debrie1Texture);


}

glm::vec2 camerapos = {0.0f,0.0f};

const int GridCellSize = PARTSIZE * 2.0f;


bool reloadSources = false;

float brightness = 1.0f;

int SRiter = 0;
glm::ivec2 StandartResolutions[] =
{
	{800,600},
	{1024,768},
	{1280,720},
	{1280,1024},
	{1366,768},
	{1680,1050},
	{1920,1080},
	{2560,1440}
};


std::string RX = std::to_string(s_Resolution.x);
std::string RY = std::to_string(s_Resolution.y);

void ResetSettings()
{
	brightness = 1.0f;
	bloom = true;
}
void SaveSettings()
{
	std::ofstream SaveFile("Settings.sav");
	SaveFile << "Res ";
	SaveFile << s_Resolution.x;
	SaveFile << " ";
	SaveFile << s_Resolution.y;
	SaveFile << "\n";

	SaveFile << "FS ";
	SaveFile << s_Fullscreen;
	SaveFile << "\n";

	SaveFile << "Br ";
	SaveFile << brightness;
	SaveFile << "\n";

	SaveFile << "Bl ";
	SaveFile << bloom;
	SaveFile << "\n";
	SaveFile << "Sh ";
	SaveFile << s_ScreenShake;
	SaveFile << "\n";
	SaveFile << "Ca ";
	SaveFile << s_ChromaticAbberation;
	SaveFile << "\n";
}

inline float Exposure = 0.0f;


struct LightEffect
{
	float time = 1.0f;
	float maxT = 1.0f;
	float S_Scale = 1000.0f;
	float E_Scale = 0.0f;

	glm::vec3 position = glm::vec3(0.0f);

	glm::vec4 S_Color = glm::vec4(1.0f);
	glm::vec4 E_Color = glm::vec4(0.0f);

	float volume = 0.05f;
};

std::vector<LightEffect> LightEffects;

void ProcessLightEffects(float dt)
{

	for (int i = 0; i < LightEffects.size(); i++)
	{
		
		float step = LightEffects[i].time / LightEffects[i].maxT;
		DrawLight(LightEffects[i].position,
			glm::vec2(LightEffects[i].S_Scale * step + LightEffects[i].E_Scale * (1.0f - step)),
			LightEffects[i].S_Color * step + LightEffects[i].E_Color * (1.0f - step),
			LightEffects[i].volume);

		LightEffects[i].time -= dt;

	}
	int i = 0;
	while (i < LightEffects.size())
	{
		if (LightEffects[i].time <= 0.0f)
		{
			LightEffects[i] = LightEffects[LightEffects.size() - 1];
			LightEffects.pop_back();
		}
		else
			i++;
	}


}

float ExplodionLightHeight = 0.010f;
float EngineLightHeight = 0.010f;
float BulletHitLightHeight = 0.010f;

Scene Background;

#include "DamageSphere.h"
#include "Explodion.h"
#include "SubECS.h"


inline float Speed = 1.0f;

int NewConType =0;
int NewConPart1;
int NewConPart2;
int NewConIndex1;
int NewConIndex2;
int ConCreationStage = 0;

bool NewConDebrie1 = false;
bool NewConDebrie2 = false;

int newPartType = -1;//-1 = not selected
float newPartRotation = 0.0f;

bool balltaken = false;

inline bool absoluteControl = true;
inline bool BuildingMode = false;

inline bool bLogicMode = false;
inline bool fLogicMode = false;
inline bool vLogicMode = false;
inline int DataconnectionData[6];


bool editSaveName = false;
//std::string saveFileName =  "Radiat.sav";
std::string saveFileName = "Save0.sav";
//std::string saveFileName = "Save0Laser.sav";
//std::string saveFileName = "BigBoy.sav";
//std::string saveFileName = "Spinner.sav";

int in_UI = 0;// <=0 - not, else - browsing ui

bool createdFreezeDS = false;

bool align = false;
bool snapToGrid = false;



#include "Parts/Base.h"


struct GridCell
{
	int size = 0;
	BallBodyComponent* balls[10];
	void add(BallBodyComponent* b)
	{
		if (size < 10)
		{
			balls[size] = b;
			size++;
		}
	}
};
GridCell Grid[300][300];

int balllbuffersize;
BallBodyComponent* ballbuffer[200];
BallBodyComponent* NewConBall1;
BallBodyComponent* NewConBall2;


#include "ParticleMaterials.h"
#include "Rocket.h"
#include "Bullet.h"
#include "Laser.h"
#include "Parts.h"
#include "Entity.h"

const int StarsAmount = 3000;
float StarsSpread = 2520.0f;
float StarsDepth = 10.0f;
glm::vec4 Stars[StarsAmount];
glm::vec4 BackgroundStars[StarsAmount];


bool MainMenu = false;
bool switchScene = false;
bool switchToMission = false;
bool OpenMenu = false;
bool Settings = false;
int lastsound = 0;
int lastsound2 = 0;
unsigned int lastplayedsound = 1;

void ProcessCamera(float dt)
{
	if (Exposure < 0.85f)
		Exposure += dt;
	else
        Exposure = 0.85f;
	SceneExposure = Exposure * Exposure * brightness;
	CameraPosition = camerapos + glm::vec2(((rand()%100) * ScreenShake - ScreenShake *50.0f )* s_ScreenShake, ((rand() %100) * ScreenShake - ScreenShake * 50.0f )* s_ScreenShake);
	ScreenShake -= ScreenShakeFallOff * dt;
	ChromaticAbberation -= ChromaticAbberationFallOff * dt;
	if (ScreenShake < 0.0f) ScreenShake = 0.0f;
	if (ChromaticAbberation < 0.0f) ChromaticAbberation = 0.0f;

	if (ScreenShake > 1.0f) ScreenShake = 1.0f;
	if (ChromaticAbberation > 0.3f) ChromaticAbberation = 0.3f;
	ChromaticStrength = ChromaticAbberation * s_ChromaticAbberation;
	ListenerPos = CameraPosition;
	UpdateListenerPosition();
}



std::vector <Node> PartSpawnPoints;

void Delete()
{
}

void ChangeMap(std::string FilePath, bool scaleDown = true)
{
	std::cout<<"changing map to: " <<FilePath<<"\n";
	
	ConsoleTexts.clear();	
	MainMenu = false;
	for(int i=0;i<Entities.size();i++)
		Entities[i]->Destroy();
	
	//if(Entities.size()>=1)
	Entities.clear();

	for (int i = 0; i < Debris.Parts.size(); i++)
		Debris.Parts[i]->DeletePart();
	Debris.Parts.clear();

	DamageSpheres.clear();
	DamageSpheresArray.clear();
	bullets.clear();
	Rockets.clear();
	Lasers.clear();
	LightEffects.clear();
	GameScene->LoadFrom(FilePath);


	PartSpawnPoints.clear();
	int currentobject = 0;
	for (int i = 0; i < GameScene->Nodes.size(); i++)
	{
		if (GameScene->Nodes[i]->id == 1 && currentobject <= SpawnablePartAmount)
		{
			if(currentobject < PART::LASTPART && currentobject != PART::CENTRALPART && currentobject != PART::STATICPOINT)
			{
				Node mp;
				mp.id = currentobject;
				mp.position = GameScene->Nodes[i]->position;
				PartSpawnPoints.push_back(mp);
			}
			currentobject++;
		}
	}




	lastEntityID = 0;

	std::cout<<"\nMap Changed to: "<<FilePath;
	SavePlayerData();
}

void SpawnPlayer(std::string filename = "PreMissionBackup")
{

	glm::vec2 position = glm::vec2(-10, 0.0f);
	glm::vec2 Scale = glm::vec2(0.5f, 0.5f);
	Entities.push_back(new CentralPart);
	Entities[0]->Create(glm::vec2(0.0f, 0.0f) * Scale + position, { 0.0f,1.0f }, PARTSIZE);
	Entities[0]->LoadFrom(filename);
	std::cout<<"\nPlayer Spawned";
}
void SpawnPlayer(glm::vec2 position,std::string filename = "PreMissionBackup")
{
	glm::vec2 Scale = glm::vec2(0.5f, 0.5f);
	Entities.push_back(new CentralPart);
	Entities[0]->Create(glm::vec2(0.0f, 0.0f) * Scale + position, { 0.0f,1.0f }, PARTSIZE);
	Entities[0]->LoadFrom(filename);
	std::cout<<"\nPlayer Spawned";
}

CentralPart* SpawnAiShip(glm::vec2 pos, std::string name)
{
	Entities.push_back(new CentralPart);
	Entities[Entities.size() - 1]->Create(pos, { 0.0f,1.0f }, PARTSIZE);
	Entities[Entities.size() - 1]->LoadFrom(name);
	Entities[Entities.size() - 1]->autocontrol = true;
	Entities[Entities.size() - 1]->trgPos = pos;
	lastEntityID++;
	Entities[Entities.size() - 1]->id = lastEntityID;
	return Entities[Entities.size() - 1];

	
}

bool inbase = true;
std::vector<std::string> shipNames;

#include "Mission.h"

Mission CurrnetMission;


void SetupInstances()
{
	PartInstances.clear();
	for(auto i : PurchasableParts)
	{
		PartInstances.push_back(CreatePart(i,{0.0f,0.0f},{0.0f,1.0f},1.0f,1.0f));
		GameScene->Nodes.pop_back();// delete autocreated pointer from scene tree
	}
}

class Shop
{
public:
	int window = -1;

	void Process(float dt)
	{
		Window* sw = GetWindow(ForeWindowID);
		Window* www = GetWindow(window);
		sw->End();
		www->Use();
		www->active = true;
		glm::vec2 WindowMousePosition = ((GetWindow(SceneWindowID)->WindowMousePosition - www->Position) ) / (www->Scale);
		LastJustPressedLMBScrMousePos = (GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos - www->Position )/ (www->Scale);
		ScreenMousePosition = WindowMousePosition;
		foregroundMousePosition =MousePosition; 

		float size = 100;
		int AssetStep = 20.0f;
		int MaxAmountRow = www->ViewportSize.x/(size + AssetStep * 2.0f); 	
		int counterX = 0;
		float AssetstepX = 0.0f;

		float step = 20.0f;
		glm::vec2 Corner =  { WIDTH * -0.5f , HEIGHT * 0.5f - size};
		Corner.x += 15.0f;
		bool b = false;
		
		AssetstepX = 0.0f;
		for(int i=0;i<PartInstances.size();i++)
		{
			
			if(counterX>=MaxAmountRow)
			{
				AssetstepX =0.0f;
				counterX = 0;
			}
			counterX++;
			b = false;
			
			PartInstances[i]->DrawPreview(Corner + glm::vec2(AssetstepX + size*0.5f + 10.0f,0.0f),{size*0.5f,size*0.5f});
			
			glm::vec4 coloron = {1.0f,1.0f,1.0f,0.8f};
			glm::vec4 coloroff = {1.0f,1.0f,1.0f,0.6f};
			glm::vec2 UIObjSize = UI_button(&b, "", Corner + glm::vec2(AssetstepX,0.0f),{size+20.0f,size+20.0f},0.35f,glm::vec4(0.9f),coloron,coloroff);
			std::string text= PartInstances[i]->Name + " " +std::to_string( PartInstances[i]->Cost.Matter);  
			UI_DrawText(text, Corner + glm::vec2(AssetstepX,UIObjSize.y*-0.5f - step*0.5f), 0.35f).x + AssetStep;
			

			if(counterX<MaxAmountRow )
				AssetstepX += UIObjSize.x + AssetStep;
			else
				Corner.y += UIObjSize.y *-1.0f - step;
			if(b )
			{
				Debris.Parts.push_back(CreatePart(PurchasableParts[i],{0.0f,0.0f},{0.0f,1.0f},PARTSIZE,1.0f));
			}
			
		}
		www->End();
		sw->Use();
		WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		ScreenMousePosition = WindowMousePosition;
		foregroundMousePosition =MousePosition; 
	}


	

};
class MissionSelectScreen
{
public:

	int window = -1;
	std::string filenamestring = "";
	std::string erroutputstring = "";
	std::vector<Mission> missions;
	int state = 0;

	glm::vec2 missionPosition = {0.0f,0.0f};
	bool Hub = true;
	bool missionSelected = false;
	bool missionStory = false;
	int missionSize = 0;
	int missionDificulty = 0;
	int missionType = 0;


	void GenerateNewMissions()
	{
		missions.clear();

		for(int i=0;i<10;i++)
		{
			Mission m;
			m.size = rand() % 5 + 1;
			m.dificulty = rand() % 5 + 1;
			m.type = rand() %4 + 1;

			float multiplyer = 1.0f;
			if(m.type == MissionType::pirates) multiplyer = 2.0f;
			if(m.type == MissionType::mining) multiplyer = 1.0f;
			if(m.type == MissionType::infestation) multiplyer = 2.0f;
			if(m.type == MissionType::retrival) multiplyer = 1.0f;

			m.materialReward = multiplyer * m.dificulty * m.size * 10;
			missions.push_back(m);
		}

	}
	void Process(float dt)
	{
		Window* sw = GetWindow(ForeWindowID);
		Window* www = GetWindow(window);
		sw->End();
		www->Use();
		www->active = true;
		glm::vec2 WindowMousePosition = ((GetWindow(SceneWindowID)->WindowMousePosition - www->Position) ) / (www->Scale);
		LastJustPressedLMBScrMousePos = (GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos - www->Position )/ (www->Scale);
		ScreenMousePosition = WindowMousePosition;
		foregroundMousePosition =MousePosition; 

		float size = 100;
		int AssetStep = 20.0f;
		int MaxAmountRow = www->ViewportSize.x/(size + AssetStep * 2.0f)-1; 	
		int counterX = 0;
		float AssetstepX = 0.0f;

		float step = 20.0f;
		glm::vec2 Corner =  { WIDTH * -0.5f , HEIGHT * 0.5f - size};
		Corner.x += 15.0f;
		bool b = false;
		
		switch (state)
		{
		case 0:
		{
			int PlayrShipcost =-1;
			if(Entities.size()>0)
				PlayrShipcost = GetShipCost(Entities[0]);
			else
				erroutputstring = "No ship";

			if(PlayrShipcost > Materials)
			{
				erroutputstring = "Cant afford current ship: ";
				erroutputstring += std::to_string(PlayrShipcost);
				erroutputstring += "/";
				erroutputstring += std::to_string(Materials);
			}
			
			bool cango = false;
			if(PlayrShipcost <= Materials && Entities.size()>0)
			{
				std::string ssss = "";
				ssss= "ShipCost: ";
				ssss+= std::to_string(PlayrShipcost);
				ssss+= "/";
				ssss+= std::to_string(Materials);
				erroutputstring = "";
				cango = true;
				Corner.y += UI_DrawText(ssss,Corner,0.35f,{1.0f,1.0f,1.0f,1.0f}).y*-1.0f - step;
				
			}
			if(erroutputstring.size()>0)
			Corner.y += UI_DrawText(erroutputstring,Corner,0.35f,{4.0f,0.0f,0.0f,1.0f}).y*-1.0f - step;
			if(cango)
			{
				Corner.y +=UI_DrawText("Main missions:",Corner,0.45f).y * -1.0f;
				b = false;
				Corner.y += UI_button(&b, "The story mission with wery cool name", Corner,{250,20},0.35f,glm::vec4(0.0f),glm::vec4(0.5f),glm::vec4(0.0f)).y * -1.0f - 0;
				if(b)
				{
					missionPosition = Rotate( glm::vec2(1.0f,0.0f),rand() % 1000000 * 0.001f) * 4000.0f * (1.0f + rand() %1000 * 0.002f);
					missionSelected = true;
					missionStory = true;
					missionSize = 0;
					missionDificulty = 0;
					missionType = 0;
				}

				Corner.y +=UI_DrawText("Missions:",Corner,0.45f).y * -1.0f;

				for(int i=0;i<missions.size();i++)
				{
					b = false;
					std::string namestr = "";
					if(missions[i].type == MissionType::pirates) namestr = "Pirates ";
					if(missions[i].type == MissionType::mining) namestr = "Mining ";
					if(missions[i].type == MissionType::infestation) namestr = "Infestation ";
					if(missions[i].type == MissionType::retrival) namestr = "Retrival ";

					namestr += "Size: ";
					namestr += std::to_string(missions[i].size);
					namestr += "  Difficulty: ";
					namestr += std::to_string(missions[i].dificulty);
					namestr += "  Reward: ";
					namestr += std::to_string(missions[i].materialReward);


					UI_DrawText(namestr.c_str(),Corner - glm::vec2(0.0f,5.0f),0.35f);
					UI_DrawCube(Corner + glm::vec2(250*0.5f,0.0f), glm::vec2(250.0f,20.0f) * 0.5f, 0.0f, glm::vec4(0.07f));
					Corner.y += UI_button(&b, "", Corner,{250,20},0.35f,glm::vec4(0.0f),glm::vec4(0.5f),glm::vec4(0.0f)).y * -1.0f - 0;

					if(b)
					{
						missionPosition = Rotate( glm::vec2(1.0f,0.0f),rand() % 100000 * 0.001f) * 7000.0f * (1.0f + rand() %1000 * 0.002f);
						missionSelected = true;
						missionStory = false;
						missionSize = missions[i].size;
						missionDificulty = missions[i].dificulty;
						missionType = missions[i].type;
						
					}
				}
			}

		}
		break;
	
		default:
			break;
		}
		www->End();
		sw->Use();
		WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		ScreenMousePosition = WindowMousePosition;
		foregroundMousePosition = MousePosition;

		
	}
};

class SaveScreen
{
public:
	int window = -1;
	std::string filenamestring = "";
	std::string erroutputstring = "";
	int state = 0;
	void Process(float dt)
	{
		Window* sw = GetWindow(ForeWindowID);
		Window* www = GetWindow(window);
		sw->End();
		www->Use();
		www->active = true;
		glm::vec2 WindowMousePosition = ((GetWindow(SceneWindowID)->WindowMousePosition - www->Position) ) / (www->Scale);
		LastJustPressedLMBScrMousePos = (GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos - www->Position )/ (www->Scale);
		ScreenMousePosition = WindowMousePosition;
		foregroundMousePosition =MousePosition; 

		float size = 100;
		int AssetStep = 20.0f;
		int MaxAmountRow = www->ViewportSize.x/(size + AssetStep * 2.0f); 	
		int counterX = 0;
		float AssetstepX = 0.0f;

		float step = 20.0f;
		glm::vec2 Corner =  { WIDTH * -0.5f , HEIGHT * 0.5f - size};
		Corner.x += 15.0f;
		bool b = false;
		Corner.x+= UI_button(&b,"Add existing ship",Corner).x + step;
		if(b)
		{
			state = 1;
			filenamestring = "";
			erroutputstring = "";
		}
		b = false;
		Corner.y+= UI_button(&b,"Save as",Corner).y * -2.0f - step;
		if(b)
		{
			state = 2;
			filenamestring = "";
			erroutputstring = "";
		}
		Corner.x =   WIDTH * -0.5f;
		Corner.x += 15.0f;
		switch (state)
		{
		// list of ships
		case 0:
		{
			for(int i=0;i<ShipInfos.size();i++)
			{

				if(counterX>=MaxAmountRow)
				{
					AssetstepX =0.0f;
					counterX = 0;
				}
				counterX++;

				//ShipInfos[i].DrawPreview(Corner + glm::vec2(AssetstepX + size*0.5f + 10.0f,0.0f),{size*0.5f,size*0.5f});

				glm::vec4 coloron = {1.0f,1.0f,1.0f,0.8f};
				glm::vec4 coloroff = {1.0f,1.0f,1.0f,0.6f};
				b = false;
				glm::vec2 UIObjSize = UI_button(&b, "", Corner + glm::vec2(AssetstepX,0.0f),{size+20.0f,size+20.0f},0.35f,glm::vec4(0.9f),coloron,coloroff);
				std::string text= ShipInfos[i].filename + " " +std::to_string( ShipInfos[i].MateralCost);  
				UI_DrawText(text, Corner + glm::vec2(AssetstepX,UIObjSize.y*-0.5f - step*0.5f), 0.35f).x + AssetStep;


				if(counterX<MaxAmountRow )
					AssetstepX += UIObjSize.x + AssetStep;
				else
					Corner.y += UIObjSize.y *-1.0f - step;
				if(b)
				{
					if(Entities.size()==1)
					{
						float cst = 0.0f;
						for (int a = 0; a < Entities[0]->Parts.size(); a++)
						{
							Entities[0]->Parts[a]->Health = -10.0f;
							for(int pi=0;pi<PartInstances.size();pi++)
							{
								if(PartInstances[pi]->type == NodeType::LASTNODE + Entities[0]->Parts[a]->type)
									{
										cst +=PartInstances[pi]->Cost.Matter;
										break;
									}
							}
						}
						Entities[0]->Destroy();
						Entities[0]->Delete=true;
						Entities.pop_back();
						glm::vec2 position = glm::vec2(-10, 0.0f);
						glm::vec2 Scale = glm::vec2(0.5f, 0.5f);
						Entities.push_back(new CentralPart);
						Entities[0]->Create(glm::vec2(0.0f, 0.0f) * Scale + position, { 0.0f,1.0f }, PARTSIZE);
						Entities[0]->LoadFrom(ShipInfos[i].filename);
						
						
					}
					else
					{
						glm::vec2 position = glm::vec2(-10, 0.0f);
						glm::vec2 Scale = glm::vec2(0.5f, 0.5f);
						Entities.push_back(new CentralPart);
						Entities[0]->Create(glm::vec2(0.0f, 0.0f) * Scale + position, { 0.0f,1.0f }, PARTSIZE);
						Entities[0]->LoadFrom(ShipInfos[i].filename);
					}
				}
			}
		}
		break;
		// add by name
		case 1:
		{
			Corner.y+= UI_TextBox(&filenamestring,Corner).y*-1.0f - step;
			if(erroutputstring.size()>0)
				Corner.y+= UI_DrawText(erroutputstring,Corner,0.35f).y*-1.0f - step;
			b = false;
			Corner.x+= UI_button(&b,"Add",Corner).x;
			if(b)
			{
				int newshipcost = CheckShipSaveFile(filenamestring);
				if(newshipcost<0)
					erroutputstring = "Ship file not exists";
				else
				{
					shipInfo si;
					si.filename = filenamestring;
					si.MateralCost = newshipcost;
					ShipInfos.push_back(si);
					state = 0;
					erroutputstring = "";
					SavePlayerData();
				}
			}
			b = false;
			Corner.y+= UI_button(&b,"Cancel",Corner).y * -1.0f - step;
			if(b)
			{
				state = 0;
				erroutputstring = "";
			}
		}
		break;
		// Save as
		case 2:
		{
			Corner.y+= UI_TextBox(&filenamestring,Corner).y*-1.0f - step;
			if(erroutputstring.size()>0)
				Corner.y+= UI_DrawText(erroutputstring,Corner,0.35f).y*-1.0f - step;
			b = false;
			Corner.x+= UI_button(&b,"Save",Corner).x;
			if(b)
			{
				if(Entities.size()>0)
					Entities[0]->SaveTo(filenamestring);
				else
				{
					erroutputstring = "No ship to save";
				}
				int newshipcost = CheckShipSaveFile(filenamestring);
				if(newshipcost<0)
					erroutputstring = "Something wrong with the ship";
				else
				{
					shipInfo si;
					si.filename = filenamestring;
					si.MateralCost = newshipcost;
					ShipInfos.push_back(si);
					state = 0;
					erroutputstring = "";
					SavePlayerData();
				}
			}
			b = false;
			Corner.y+= UI_button(&b,"Cancel",Corner).y * -1.0f - step;
			if(b)
			{
				state = 0;
				erroutputstring = "";
			}

		}
		break;
		
		default:
			break;
		}
		www->End();
		sw->Use();
		WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		ScreenMousePosition = WindowMousePosition;
		foregroundMousePosition =MousePosition; 
	}


	

};
MissionSelectScreen MissionSelectMenu;
SaveScreen SaveScreenmenu;
Shop shopmenu;

void ProcessPlayerControls()
{
	AqueredCameraScale *= 1.0f + scrollmovement * 0.1f;

	if (AqueredCameraScale.x > 80.0f)
		AqueredCameraScale = { 80.0f, 80.0f };
	if (AqueredCameraScale.x < 1.0f)
		AqueredCameraScale = { 1.0f ,1.0f };

	CameraScale += (AqueredCameraScale - CameraScale) * 0.25f * (delta > 0.017f ? 0.017f : delta) * 60.0f;

	if (JustPressedkey[GLFW_KEY_V])
		absoluteControl = !absoluteControl;

	if (!absoluteControl)
	{
		glm::vec2 LookAtVec = Normalize(MousePosition - Entities[0]->CenterOfMass);

		if (LookAtVec.x != 0.0f || LookAtVec.y != 0.0f)LookAtVec = Normalize(LookAtVec);
		if (sqrlength(MousePosition - CameraPosition) > 10.0f)
			Entities[0]->LookAt = LookAtVec;

		glm::vec2 velvec = glm::vec2(0.0f);

		if (keys[GLFW_KEY_W]) velvec += LookAtVec;
		if (keys[GLFW_KEY_S]) velvec -= LookAtVec;
		if (keys[GLFW_KEY_D]) velvec += glm::vec2(LookAtVec.y, -LookAtVec.x);
		if (keys[GLFW_KEY_A]) velvec -= glm::vec2(LookAtVec.y, -LookAtVec.x);


		if (velvec.x != 0.0f || velvec.y != 0.0f) velvec = Normalize(velvec);
		Entities[0]->ThrustDirection = velvec;

	}
	else
	{
		glm::vec2 LookAtVec = Entities[0]->direction;

		if (LookAtVec.x != 0.0f || LookAtVec.y != 0.0f)
			LookAtVec = Normalize(LookAtVec);
		else
			LookAtVec = glm::vec2(0.0f, 1.0f);


		if (keys[GLFW_KEY_A]) LookAtVec = Rotate(LookAtVec, pi * 0.5);
		else if (keys[GLFW_KEY_D]) LookAtVec = Rotate(LookAtVec, pi * -0.5);
		else LookAtVec = Entities[0]->direction;

		Entities[0]->LookAt = LookAtVec;

		glm::vec2 velvec = glm::vec2(0.0f);

		if (keys[GLFW_KEY_W]) velvec += Entities[0]->direction;
		if (keys[GLFW_KEY_S]) velvec -= Entities[0]->direction;
		if (keys[GLFW_KEY_E]) velvec += glm::vec2(Entities[0]->direction.y, -Entities[0]->direction.x);
		if (keys[GLFW_KEY_Q]) velvec -= glm::vec2(Entities[0]->direction.y, -Entities[0]->direction.x);

		if (velvec.x != 0.0f || velvec.y != 0.0f) velvec = Normalize(velvec);
		Entities[0]->ThrustDirection = velvec;
	}

	if(Entities.size()>0 && !Entities[0]->dead && !Entities[0]->destroyed)
	{
		camerapos = Entities[0]->mid;
		Entities[0]->player = true;
		Entities[0]->autocontrol = false;
		listenerVel = { Entities[0]->midvel.x ,Entities[0]->midvel.y ,1.0f };
	
	if (BuildingMode || bLogicMode || fLogicMode || vLogicMode)
	{
		BackgroundColor = glm::vec4(0.0f, 0.0f, 0.01f, 1.0f);
		//grid
		glm::vec2 gridpos = glm::vec2(0.0f, 0.0f);
		Entities[0]->FireGuns = false;

		// grid size = 1
		glm::vec2 roundCP = glm::vec2(roundf(CameraPosition.x), roundf(CameraPosition.y));
		if (CameraScale.x > 15.0f)
			for (int i = 0; i < 200; i++)
			{
				DrawLine(glm::vec2(50 * (i * 0.02f - 2.0f), 50 * -2.0f) + gridpos + roundCP,
					glm::vec2(50 * (i * 0.02f - 2.0f), 50 * 2.0f) + gridpos + roundCP, 1.75f * 0.025f,
					glm::vec4(0.15f), false, CubeNormalMapTexture, -100);

				DrawLine(glm::vec2(50 * -2.0f, 50 * (i * 0.02f - 2.0f)) + gridpos + roundCP,
					glm::vec2(50 * 2.0f, 50 * (i * 0.02f - 2.0f)) + gridpos + roundCP, 1.75f * 0.025f,
					glm::vec4(0.15f), false, CubeNormalMapTexture, -100);
			}

		if (JustPressedLMB && in_UI <= 0)
		{
			DamageSphere DS;
			DS.body.r = 5.0f;
			DS.body.position = MousePosition;
			DS.Damage = false;
			DS.timeLeft = 1.0f;
			DS.Heat = 0.0f;
			DS.recoil = 0.0f;
			DS.neutral = true;
			DS.friction = 0.9f;
			DS.singleHit = false;
			DamageSpheresArray.push_back(DS);
		}
	}
	else
	{
		DataconnectionData[0] = 0;

		Entities[0]->GunsTargetrotPoint = MousePosition;
		BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		/*if (buttons[GLFW_MOUSE_BUTTON_2])
		{

			Entities.push_back(new Entity);
			Entities[Entities.size() - 1]->Ready(MousePosition, { 0.0f,1.0f }, PARTSIZE);
			Entities[Entities.size() - 1]->LoadFrom("Cube.sav");
			Entities[Entities.size() - 1]->autocontrol = true;
			Entities[Entities.size() - 1]->trgPos = MousePosition;
			Entities[Entities.size() - 1]->enemy = true;
			Entities[Entities.size() - 1]->CP.Health=-10;


		}*/
	}
	if (bLogicMode || fLogicMode || vLogicMode)
	{
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_2])
			DataconnectionData[0] = 0;


		for (int i = 0; i < Entities[0]->Parts.size(); i++)
		{
			Entities[0]->Parts[i]->DrawDataConnections(i, bLogicMode, fLogicMode, vLogicMode, 1000);

		}
		if (DataconnectionData[0] == 1)
		{
			if (DataconnectionData[1] == 0)
				DrawLine(MousePosition, Entities[0]->Parts[DataconnectionData[2]]->bDataConnections[DataconnectionData[3]].lastpos, 0.125f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0, 1000);
			else if (DataconnectionData[1] == 1)
				DrawLine(MousePosition, Entities[0]->Parts[DataconnectionData[2]]->fDataConnections[DataconnectionData[3]].lastpos, 0.125f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0, 0, 1000);
			else if (DataconnectionData[1] == 2)
				DrawLine(MousePosition, Entities[0]->Parts[DataconnectionData[2]]->vDataConnections[DataconnectionData[3]].lastpos, 0.125f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0, 1000);
		}
		if (DataconnectionData[0] == 2)
		{
			Entities[0]->AddDataConnection(DataconnectionData[1], DataconnectionData[2], DataconnectionData[3], DataconnectionData[4], DataconnectionData[5]);
			DataconnectionData[0] = 0;
		}
	}
	else
		DataconnectionData[0] = 0;

	if (BuildingMode)
	{


		if (JustPressedkey[GLFW_KEY_TAB])
		{
			NewConType++;
			if (NewConType > 3) NewConType = 0;
		}

		if (JustPressedkey[GLFW_KEY_SPACE])
			align = !align;
		if (align)
		{
			//Entities[0].Parts[0] -> central part
			glm::vec2 mid = (Entities[0]->Parts[0]->body[0].position + Entities[0]->Parts[0]->body[1].position) * 0.5f;

			Entities[0]->Parts[0]->body[0].velocity = ((mid + glm::vec2(0.0f, 1.0f) * Entities[0]->Parts[0]->body[0].r) - Entities[0]->Parts[0]->body[0].position) * 15.0f;
			Entities[0]->Parts[0]->body[1].velocity = ((mid - glm::vec2(0.0f, 1.0f) * Entities[0]->Parts[0]->body[1].r) - Entities[0]->Parts[0]->body[1].position) * 15.0f;

		}



		if (keys[GLFW_KEY_LEFT_ALT])
		{
			if (ConCreationStage == 1)
			{
				DrawLine(NewConBall1->position, MousePosition, 0.125f);
			}
			if (JustPressedLMB)
			{
				int prevcon = ConCreationStage;
				for (int i = 0; i < Entities[0]->Parts.size(); i++)
				{
					for (auto a : Entities[0]->Parts[i]->BodyIdsWithCollision)
					{
						if (BalltoPointCollisionCheck(Entities[0]->Parts[i]->body[a], MousePosition))
						{
							if (ConCreationStage == 0)
							{
								NewConBall1 = &Entities[0]->Parts[i]->body[a];
								NewConIndex1 = a;
								NewConPart1 = i;
								ConCreationStage++;
							}
							else if (ConCreationStage == 1)
							{
								NewConBall2 = &Entities[0]->Parts[i]->body[a];
								NewConIndex2 = a;
								NewConPart2 = i;
								ConCreationStage++;
							}
						}
					}
					
				}



				if (prevcon == ConCreationStage && !NewConDebrie1)
				{
					for (int i = 0; i < Debris.Parts.size(); i++)
					{
						for (auto a : Debris.Parts[i]->BodyIdsWithCollision)
						{
							if (BalltoPointCollisionCheck(Debris.Parts[i]->body[a], MousePosition))
							{
								if (ConCreationStage == 0)
								{
									NewConBall1 = &Debris.Parts[i]->body[a];
									NewConIndex1 = a;
									NewConPart1 = i;
									ConCreationStage++;
									NewConDebrie1 = true;
								}
								else if (ConCreationStage == 1)
								{
									NewConBall2 = &Debris.Parts[i]->body[a];
									NewConIndex2 = a;
									NewConPart2 = i;
									ConCreationStage++;
									NewConDebrie2 = true;
								}
							}
						}
						
					}


				}
				if (ConCreationStage == 1 && prevcon != ConCreationStage)
				{
					Sparks.Spawn(NewConBall1->position, rand() % 5 + 5);
					playsound(Clang, MousePosition, 0.3f,2.5f,{0.0f,0.0f},false);
				}
				if (ConCreationStage >= 2)
				{
					playsound(Clang, MousePosition, 0.3f,2.5f,{0.0f,0.0f},false);
					Sparks.Spawn(NewConBall2->position, rand() % 5 + 5);
					ConCreationStage = 0;
					if (NewConDebrie1)
						NewConPart1 = Entities[0]->ClaimPart(NewConPart1);
					if (NewConDebrie2)
						NewConPart2 = Entities[0]->ClaimPart(NewConPart2);
					NewConDebrie1 = false;
					NewConDebrie2 = false;

					if (NewConPart1 != NewConPart2)
						Entities[0]->AddConnection(NewConType, -1, 0.125f, 1500, 2, NewConType == HEATPIPE ? 0.4f : 0.1f, NewConPart1, NewConIndex1, NewConPart2, NewConIndex2);
				}
			}
		}
		else
		{
			ConCreationStage = 0;
			NewConDebrie1 = false;
			NewConDebrie2 = false;
		}
		if (keys[GLFW_KEY_LEFT_SHIFT] && !keys[GLFW_KEY_LEFT_ALT])
		{
			if (JustPressedLMB && in_UI <= 0)
			{
				for (int i = 1; i < Entities[0]->Parts.size(); i++)
				{
					bool detached = true;
					while (detached)
					{
						detached = false;
						for (int a = 0; a < Entities[0]->Parts[i]->bodysize; a++)
						{

							if (i < Entities[0]->Parts.size() && BalltoPointCollisionCheck(Entities[0]->Parts[i]->body[a], MousePosition) && !detached)
							{
								playsound(Detach, MousePosition, 0.3f,3.5f,{0.0f,0.0f},false);
								Entities[0]->DetachPart(i);
								detached = true;
							}
						}
					}
				}
			}
		}

		if (JustPressedbutton[GLFW_MOUSE_BUTTON_3])
		{
			bool found = false;
			for (int i = 0; i < Debris.Parts.size(); i++)
			{
				if (Debris.Parts[i]->bodysize > 1)
				{
					for (int a = 0; a < Debris.Parts[i]->bodysize; a++)
						if (BalltoPointCollisionCheck(Debris.Parts[i]->body[a], MousePosition))
						{

							found = true;
							SelectedPart = i;

							break;
						}
				}

				if (found)
					break;
			}
		}
		if (JustReleasedbutton[GLFW_MOUSE_BUTTON_3])
			SelectedPart = -1;
		if (SelectedPart > 0)
		{
			glm::vec2 mid = { 0.0f,0.0f };
			for (int i = 0; i < Debris.Parts[SelectedPart]->bodysize; i++)
				mid += Debris.Parts[SelectedPart]->body[i].position;
			mid /= Debris.Parts[SelectedPart]->bodysize;

			glm::vec2 dir = Normalize(MousePosition - mid);

			float rot = 0.0f;
			float rotStep = pi * 2.0f / Debris.Parts[SelectedPart]->bodysize;
			for (int i = 0; i < Debris.Parts[SelectedPart]->bodysize; i++)
			{
				Debris.Parts[SelectedPart]->body[i].position = mid + Rotate(dir, rot) * Debris.Parts[SelectedPart]->body[i].r;
				rot += rotStep;
			}

		}

		bool b = (NewConType == STRUT);
		UI_buttonOnlyON(&b, "Strut", { -0.47f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);
		if (b && b != (NewConType == STRUT))
			NewConType = STRUT;

		b = (NewConType == SPRING);
		UI_buttonOnlyON(&b, "Spring", { -0.47f * WIDTH ,-0.40f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);
		if (b && b != (NewConType == SPRING))
			NewConType = SPRING;

		b = (NewConType == HEATPIPE);
		UI_buttonOnlyON(&b, "Heat pipe", { -0.47f * WIDTH ,-0.34f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);
		if (b && b != (NewConType == HEATPIPE))
			NewConType = HEATPIPE;

		b = (NewConType == ROPE);
		UI_buttonOnlyON(&b, "Rope", { -0.47f * WIDTH ,-0.28f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);
		if (b && b != (NewConType == ROPE))
			NewConType = ROPE;





		UI_CheckBox(&align, "Align rotation", { -0.35f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);

		UI_CheckBox(&snapToGrid, "Snap to grid", { -0.20f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);

		UI_TextBox(&saveFileName, { -0.46f * WIDTH ,0.46f * HEIGHT }, 32, { 15 * UISize,UISize * 1.0f }, TextSize, { 0.4f,0.4f,0.4f,0.4f }, { 1.0f,1.0f,1.0f,1.0f }, 1200);

		bool save = false;
		UI_buttonOnlyON(&save, "Save", { -0.46f * WIDTH ,0.40f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);
		if (save)
			Entities[0]->SaveTo(saveFileName);

		bool load = false;
		UI_buttonOnlyON(&load, "Load", { -0.46f * WIDTH ,0.34f * HEIGHT }, UISize, TextSize, glm::vec4(0.9f), glm::vec4(0.5f), 1200);
		if (load)
			Entities[0]->LoadFrom(saveFileName);


			
	}
	}
	
	if(!CurrnetMission.compleated || MissionSelectMenu.missionSelected)
	{
		if(MissionSelectMenu.missionSelected)
			DrawCircle(Entities[0]->mid + Normalize(MissionSelectMenu.missionPosition - Entities[0]->mid)*10.0f,5.0f,{5.0f,1.0f,1.0f,1.0f},0,0,10000);
		else
			DrawCircle(Entities[0]->mid + Normalize(glm::vec2(0.0f,0.0f) - Entities[0]->mid)*10.0f,5.0f,{5.0f,1.0f,1.0f,1.0f},0,0,10000);
	}
	else
	{
		if(!MissionSelectMenu.Hub)
			DrawCircle(Entities[0]->mid + Normalize(-MissionSelectMenu.missionPosition - Entities[0]->mid)*10.0f,5.0f,{1.0f,1.0f,5.0f,1.0f},0,0,10000);
		else
			DrawCircle(Entities[0]->mid + Normalize(glm::vec2(0.0f,0.0f) - Entities[0]->mid)*10.0f,5.0f,{1.0f,1.0f,5.0f,1.0f},0,0,10000);
	}

	if(MissionSelectMenu.missionSelected && Entities.size()>0 && sqrlength(Entities[0]->mid - MissionSelectMenu.missionPosition) < (4000*4000))
	{
		CurrnetMission.story_mission = MissionSelectMenu.missionStory;
		CurrnetMission.size = MissionSelectMenu.missionSize;
		CurrnetMission.dificulty = MissionSelectMenu.missionDificulty;
		CurrnetMission.type = MissionSelectMenu.missionType;
		MissionSelectMenu.missionSelected = false;
		MissionSelectMenu.Hub = false;
		glm::vec2 plPos = (Entities[0]->mid - MissionSelectMenu.missionPosition);
		CurrnetMission.Start(plPos);
		inbase = false;
		switchScene = false;
		OpenMenu = false;
	}
	if(!MissionSelectMenu.Hub && Entities.size()>0 && sqrlength(-MissionSelectMenu.missionPosition - Entities[0]->mid) < (2000*2000))
	{
		MissionSelectMenu.missionSelected = false;
		MissionSelectMenu.Hub = true;
		glm::vec2 plPos = (Entities[0]->mid + MissionSelectMenu.missionPosition);		
		Materials += GetShipCost(Entities[0]);
		Entities[0]->SaveTo(EntityBackUpName);
		glm::vec2 vel = {0.0f,0.0f};
		if(Entities.size()>0)
		{
			vel = Entities[0]->avgvel;
			Entities[0]->SaveTo(EntityBackUpName);
		}
		inbase = true;
		AmbientLight = 0.0f;
		GetWindow(BackgroundWindowID)->w_DirectionalLight = 1.0f;
		ChangeMap("Scenes/base.sav", false);
		SpawnPlayer(plPos, EntityBackUpName);
		for(int i=0;i< Entities[0]->Parts.size();i++)
		{
			for(int a=0;a< Entities[0]->Parts[i]->bodysize;a++)
			{
				Entities[0]->Parts[i]->body[a].velbuff = vel;
				Entities[0]->Parts[i]->body[a].velocity = vel;
			}
		}
	}

	UI_DrawLine({ 0.34f * WIDTH ,0.46f * HEIGHT },{ 0.34f * WIDTH + 0.16f * WIDTH,0.46f * HEIGHT }, 10.0f,glm::vec4(0.1f,0.1f,0.1f,1.0f),0,0,10000);
	UI_DrawLine({ 0.35f * WIDTH ,0.46f * HEIGHT },{ 0.35f * WIDTH + AccumulatedHeat/400.0f * 0.15f * WIDTH,0.46f * HEIGHT }, 8.0f,glm::vec4(AccumulatedHeat / 40.0f,AccumulatedHeat / 200.0f,AccumulatedHeat / 300.0f,1.0f),0,0,10000);

	bool blm = bLogicMode;
	bool flm = fLogicMode;
	bool vlm = vLogicMode;
	bool bm = BuildingMode;

	if (JustPressedkey[GLFW_KEY_B])
		BuildingMode = !BuildingMode;

	glm::vec4 UI_ColorON = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec4 UI_ColorOFF = glm::vec4(0.5f, 0.5f, 0.5f, 0.7f);

	UI_CheckBox(&bLogicMode, "Bools Logic", { 0.40f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
	UI_CheckBox(&fLogicMode, "Number Logic", { 0.40f * WIDTH ,-0.40f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
	UI_CheckBox(&vLogicMode, "Vector Logic", { 0.40f * WIDTH ,-0.34f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
	UI_CheckBox(&BuildingMode, "Building Mode", { 0.40f * WIDTH ,-0.28f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);

	if (bLogicMode && blm != bLogicMode)
	{
		bLogicMode = true;
		fLogicMode = false;
		vLogicMode = false;
		BuildingMode = false;
		DataconnectionData[0] = 0;
	}
	else if (fLogicMode && flm != fLogicMode)
	{
		bLogicMode = false;
		fLogicMode = true;
		vLogicMode = false;
		BuildingMode = false;
		DataconnectionData[0] = 0;
	}
	else if (vLogicMode && vlm != vLogicMode)
	{
		bLogicMode = false;
		fLogicMode = false;
		vLogicMode = true;
		BuildingMode = false;
		DataconnectionData[0] = 0;
	}
	else if (BuildingMode && bm != BuildingMode)
	{
		bLogicMode = false;
		fLogicMode = false;
		vLogicMode = false;
		BuildingMode = true;
		DataconnectionData[0] = 0;
	}

	if (GrabbedBall != NULL)
	{
		point p;
		p.position = MousePosition;
		SpringBallToPoint(GrabbedBall, &p, 0.0f, 3000.0f, 1.0f);
		DrawLine(GrabbedBall->position, MousePosition, 0.025f, { 1.0f,1.0f,0.0f,1.0f });

		DamageSphere DS;
		DS.body.r = 0.25f;
		DS.body.position = MousePosition;
		DS.Damage = false;
		DS.timeLeft = 0.044f;
		DS.Heat = 0.0f;
		DS.recoil = -1.0f;
		DS.neutral = true;
		DS.friction = 0.99f;
		DS.singleHit = false;
		DamageSpheresArray.push_back(DS);

	}

	if(CurrnetMission.exitedmission)
	{
		MissionSelectMenu.GenerateNewMissions();
		CurrnetMission.exitedmission = false;
	}
	//base
	if(inbase)
	{
		Window* shw = GetWindow(shopmenu.window);
		shopmenu.Process(delta);
		shw->Scale = CameraScale * 0.25f;
		shw->Position = (glm::vec2(-255,50) - CameraPosition)* CameraScale;
		UI_DrawTexturedQuad(shw->Position , shw->GetSize(), shw->Texture, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 2, false,false,false,true);

		Window* sshw = GetWindow(SaveScreenmenu.window);
		SaveScreenmenu.Process(delta);
		sshw->Scale = CameraScale * 0.25f;
		sshw->Position = (glm::vec2(0,250) - CameraPosition)* CameraScale;
		UI_DrawTexturedQuad(sshw->Position , sshw->GetSize(), sshw->Texture, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 2, false,false,false,true);

		Window* mshw = GetWindow(MissionSelectMenu.window);
		MissionSelectMenu.Process(delta);
		mshw->Scale = CameraScale * 0.25f;
		mshw->Position = (glm::vec2(0,-250) - CameraPosition)* CameraScale;
		UI_DrawTexturedQuad(mshw->Position , mshw->GetSize(), mshw->Texture, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 2, false,false,false,true);

	}
	else
	{
		
		CurrnetMission.Process(delta);
	}

}

void ProcessMainMenu()
{

	// -900;280


	Window* sw = GetWindow(ForeWindowID);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* mw = GetWindow(MenuWindowID);
	
	mw->Use();
	
	glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
	MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
	MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
	LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
	ScreenMousePosition = WindowMousePosition;

	glm::vec2 Corner = { WIDTH * -0.45f,HEIGHT *0.35f };
	float step = 10.0f;
	glm::vec4 textColor = { 2.0f,2.0f,2.0f,1.0f };
	glm::vec4 textColorOff = { 1.0f,1.0f,1.0f,1.0f };
	int textZ = 2000;
	if (OpenMenu || SettingsWindow)
	{
		UI_DrawCube({ 0.0,0.0f }, { WIDTH * 1.0f,HEIGHT * 1.0f }, 0.0f, { 0.0f, 0.0f, 0.0f, 0.8f }, false, NULL, textZ - 100);
	}

	bool b = false;

	Corner.y += UI_DrawText("HEAT", Corner, TextSize*2.0f, textColor, textZ).y * -1.0f -step;
	
	Corner.y += UI_CheckBox(&b,"Start", Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
	if (b)
	{
		Exposure = -0.85f;
		switchScene = true;
	}
	if (Exposure > 0.0f && switchScene)
	{
		mw->End();
		sw->Use();

		if(storyint >0)
		{
			ChangeMap("Scenes/base.sav", false);

			SpawnPlayer();
			Background.LoadFrom("Scenes/Sun.sav");

			CurrnetMission.story_mission = false;
			inbase = true;
			switchScene = false;
			OpenMenu = false;
		}
		else
		{
			CurrnetMission.story_mission = true;
			CurrnetMission.Start();	
			inbase = false;
			switchScene = false;
			OpenMenu = false;
		}
		SaveSettings();
		sw->End();
		mw->Use();
		
		glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		ScreenMousePosition = WindowMousePosition;
	}

	Corner.y += UI_CheckBox(&Settings, "Settings", Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
	
	if (Settings)
	{
		glm::vec2 s_Corner = { WIDTH * -0.115f,HEIGHT * 0.35f };
		bool b = false;

		s_Corner.y += UI_DrawText("Settings", s_Corner, TextSize * 2.0, textColor, textZ).y * -1.0f - step;

		s_Corner.y += UI_CheckBox(&bloom, "Bloom", s_Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		s_Corner.y += UI_Slider(&brightness, "Brightness", s_Corner, 0.1f,1.5f, { 10 * UISize,UISize * 0.35f }, TextSize, textColor, textColor, textZ).y * -1.0f - step;

		s_Corner.y += UI_CheckBox(&b, "Reset to default", s_Corner , UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		if (b)
			ResetSettings();

		s_Corner.y += UI_CheckBox(&s_Fullscreen, "Full screen", s_Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		b = false;
		float stepX = UI_CheckBox(&b, "-", s_Corner , UISize, TextSize, textColor, textColorOff, textZ).x * 0.5f + step;
		if (b)
		{
			SRiter--;
			if (SRiter < 0)
				SRiter = 7;
			s_Resolution = StandartResolutions[SRiter];

			RX = std::to_string(s_Resolution.x);
			RY = std::to_string(s_Resolution.y);
			RX.resize(4);
			if (RX[3] == '.')
				RX.pop_back();

			RY.resize(4);
			if (RY[3] == '.')
				RY.pop_back();
		}


		std::string BRX = RX;
		std::string BRY = RY;
		stepX += UI_TextBox(&RX, s_Corner + glm::vec2(stepX, 0.0f), -1, { 5 * UISize,UISize * 1.0f }, TextSize, textColorOff, textColor , textZ).x * 0.5f + step;
		
		stepX += UI_TextBox(&RY, s_Corner + glm::vec2(stepX, 0.0f), -1, { 5 * UISize,UISize * 1.0f }, TextSize, textColorOff, textColor, textZ).x * 0.5f + step;
	

		if (BRX != RX || BRY != RY)
			SRiter = 0;
		b = false;
		s_Corner.y += UI_CheckBox(&b,"+", s_Corner + glm::vec2(stepX, 0.0f), UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		if(b)
		{
			SRiter++;
			if (SRiter > 7)
				SRiter = 0;
			s_Resolution = StandartResolutions[SRiter];

			RX = std::to_string(s_Resolution.x);
			RY = std::to_string(s_Resolution.y);
			RX.resize(4);
			if (RX[3] == '.')
				RX.pop_back();

			RY.resize(4);
			if (RY[3] == '.')
				RY.pop_back();
		}
	

		s_Corner.y += -20 - step;
		s_Corner.y += UI_DrawText("Resolution or Full screen changes require a reload", s_Corner, TextSize, textColor, textZ).y * -1.0f - step;


		s_Corner.y += UI_Slider(&s_ChromaticAbberation, "Chromatic abberation", s_Corner, 0.0f, 1.0f, { 10 * UISize,UISize * 0.35f }, TextSize, textColor, textColor, textZ).y * -1.0f - step;

		s_Corner.y += UI_Slider(&s_ScreenShake, "Screen shake", s_Corner, 0.0f, 1.0f, { 10* UISize,UISize * 0.35f }, TextSize, textColor, textColor, textZ).y * -1.0f - step;
		/*b = false;
		s_Corner.y += UI_CheckBox(&b, "Save", s_Corner, 25.0f, 1.0f).y * -1.0f - step;
		if (b)
			SaveSettings();*/



	}
	
	
	
	
	b = false;
	Corner.y += UI_CheckBox(&b, "Exit", Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
	if (b)
	{
		SaveSettings();
		glfwSetWindowShouldClose(window, true);
	}
	mw->End();
}


void Ready()
{
	
	std::cout<<"ready begins\n";
	
	ScreenShake = 0.0f;
	ChromaticAbberation = 0.0f;
	ScreenShakeFallOff = 1.0f;
	ChromaticAbberationFallOff = 1.0f;
	LoadTextures();

	noize = NULL;
	GenNoizeTexture(&noize, 100, 2, 1.0f, 2);

	GenPrimitiveTexture(&PrimitiveQuad.Texture,5,QUAD);
	//LoadFont("engine/fonts/LiberationSansR.ttf", 128);

	
	GenPrimitiveTexture(&MO_Ball.mater.Texture,1000,CIRCLE,true);
	GenPrimitiveTexture(&MO_Cube.mater.Texture,5,QUAD);
	MO_Ball.mater.NormalMap = BallNormalMapTexture;
	MO_Cube.mater.NormalMap = CubeNormalMapTexture;

	float maxsize = fmaxf(WIDTH, HEIGHT);
	TextSize = maxsize * 0.0005f * 0.35f;
	UISize = 25 * maxsize * 0.0005f;

	RX = std::to_string(s_Resolution.x);
	RY = std::to_string(s_Resolution.y);
	RX.resize(4);
	if (RX[3] == '.')
		RX.pop_back();

	RY.resize(4);
	if (RY[3] == '.')
		RY.pop_back();

	soundscale = { 40.0f,40.0f ,1.0f };

	std::cout<<"loading sounds ready\n";
	shopmenu.window = CreateWindow();
	SaveScreenmenu.window = CreateWindow();
	MissionSelectMenu.window = CreateWindow();

	Window* shw = GetWindow(shopmenu.window);
	shw->Init({570,600});
	shw->backgroundColor = {0.0f,0.0f,0.0f,1.0f};
	shw->Position = {-100,50};
	
	Window* sshw = GetWindow(SaveScreenmenu.window);
	sshw->Init({570,600});
	sshw->backgroundColor = {0.0f,0.0f,0.0f,1.0f};
	sshw->Position = {-100,250};

	
	Window* mshw = GetWindow(MissionSelectMenu.window);
	mshw->Init({570,600});
	mshw->backgroundColor = {0.0f,0.0f,0.0f,1.0f};
	mshw->Position = {-100,250};

	RocketEngineSound = LoadSound("Sounds/noize_L.wav");
	SHHSound = LoadSound("Sounds/wind.wav");
	GunSound = LoadSound("Sounds/HeavyHit.wav");
	HeavyHit = LoadSound("Sounds/BigShot.wav");
	LaserGunSound = LoadSound("Sounds/Laser.wav");
	Clang = LoadSound("Sounds/metalhit1.wav");
	Detach = LoadSound("Sounds/detach.wav");
	Scratch = LoadSound("Sounds/Scratch.wav");
	PartDestrSOund = LoadSound("Sounds/Explodion.wav");
	Hit = LoadSound("Sounds/hit.wav");
	BulletHit = LoadSound("Sounds/CleanExplodion.wav");
	ExplodionSound = LoadSound("Sounds/VineBoom.wav");
	MiniGunSound = LoadSound("Sounds/Minigun.wav");
	
	SetupPEs();
	shipNames.clear();
	shipNames.push_back("Bigboy");
	shipNames.push_back("Cube");
	shipNames.push_back("Rotator");
	shipNames.push_back("Save0");
	shipNames.push_back("Save1");
	shipNames.push_back("Spinner");


	std::cout<<"loaded sounds ready\n";
	//SpawnPlayer();

	substeps = 20;
	listenerPos.z = 2.0f;

	

	glfwSwapInterval(0);
	VSync = 0;


	ForeWindowID = CreateWindow();
	BackgroundWindowID = CreateWindow();
	MenuWindowID = CreateWindow();
	TerminalWindowID= CreateWindow();

	Window* bw = GetWindow(BackgroundWindowID);
	Window* sw = GetWindow(ForeWindowID);
	Window* cw = GetWindow(TerminalWindowID);
	Window* mw = GetWindow(MenuWindowID);


	sw->AutoActive = false;
	bw->AutoActive = false;
	cw->AutoActive = false;
	mw->AutoActive = false;

	sw->hdr = true;

	sw->Init(GetWindow(SceneWindowID)->ViewportSize);
	bw->Init(GetWindow(SceneWindowID)->ViewportSize);
	cw->Init(GetWindow(SceneWindowID)->ViewportSize);
	mw->Init(GetWindow(SceneWindowID)->ViewportSize);


    sw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
    bw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	cw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	mw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	EditorColor = {0.0f,0.0f,0.0f,1.0f};
	sw->w_AmbientLight = 0.4f;
	bw->w_AmbientLight = 0.1f;
	cw->w_AmbientLight = 1.4f;
	sw->w_DirectionalLight = 2.0f;

	


	for (int i = 0; i < StarsAmount; i++)
	{
		Stars[i].x = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.x;
		Stars[i].y = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.y;
		Stars[i].z = rand() % 1000 * 0.0001f * StarsDepth;
		Stars[i].w = rand() % 1000 * 0.001f * pi * 0.5f;


		BackgroundStars[i].x = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.x;
		BackgroundStars[i].y = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.y;
		BackgroundStars[i].z = rand() % 1000 * 0.0001f * StarsDepth;
		BackgroundStars[i].w = rand() % 1000 * 0.001f * pi * 0.5f;
	}
	
	std::cout<<"loading pData ready\n";
	LoadPlayerData();
	ChangeMap(MapFileName, false);
	MainMenu = true;
	addsound(SHHSound,true,10);
	addsound(RocketEngineSound,true,20);
	addsound(MiniGunSound,true,10);
	addsound(ExplodionSound,false,10);
	addsound(BulletHit,false,10);
	addsound(Hit,false,20);
	addsound(PartDestrSOund,false,10);
	addsound(Scratch,true,10);
	addsound(Detach,false,10);
	addsound(Clang,false,10);
	addsound(LaserGunSound,true,10);
	addsound(HeavyHit,false,10);
	addsound(GunSound,false,10);
	std::cout<<"added all sounds ready\n";
	//150 sources used
	MissionSelectMenu.GenerateNewMissions();
	SetupInstances();
	VSync = 1;


	GrabbedBall = nullptr;
	SelectedPart = 0;
	
	BuildingMode = false;
	bLogicMode = false;
	fLogicMode = false;
	vLogicMode = false;
	DataconnectionData[6] = {0};
	Speed = 1.0f;
	std::cout<<"ready\n";
}
void SubSteppedProcess(float dt, int SubStep)
{
	Window* sw = GetWindow(ForeWindowID);
	sw->Use();
	
	ProcessEntities(dt, SubStep);

	for(int i=0;i<Sounds.size();i++)
	{
		Sounds[i].Update();
	}
	sw->End();
    UseWindow(SceneWindowID);
}

void Process(float dt)
{
	if (clock() < 100)
		Speed = 0.0f;
	else if (clock() >= 100 && clock() < 1200)
		Speed = 1.0f;

	if (dt > 0.017f)
		dt = 0.017f;

	dt *= Speed;
	in_UI--;

	if (in_UI < 0)
		in_UI = 0;

	Window* sw = GetWindow(ForeWindowID);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* cw = GetWindow(TerminalWindowID);
	Window* mw = GetWindow(MenuWindowID);

	sw->active = true;

	sw->Use();
	glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
	MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
	MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
	ScreenMousePosition = WindowMousePosition;
	LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
	foregroundMousePosition =MousePosition; 
	

	if (JustReleasedkey[GLFW_KEY_ESCAPE])
		OpenMenu = !OpenMenu;

	if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
		LastJustPressedLMBScrMousePos = { 0.0f,0.0f };

	ImGui::Begin("Settings");
	ImGui::SliderFloat("speed", &Speed, 0.0f, 10.0f);
	ImGui::Text("ballssize = %.i", balls.size());
	ImGui::Text("%.1ffps (%.4fms)", 1.0f/delta,delta);


	unsigned int soundsrc;
	GenSource(&soundsrc);
	ImGui::Text("sound source id %.i", soundsrc);
	DeleteSource(&soundsrc);


	ImGui::Text("sources size (%.i)", sources.size());




	
	reloadSources = false;
	bool res = false;
	for (int i = 0; i < sources.size(); i++)
	{
		if (*sources[i] < 0)
			res = true;

	}
	if (sources.size() > 10000 || ImGui::Button("Relopad sound") || res)
	{
		for (int i = 0; i < sources.size();i++)
			DeleteSource(sources[i]);
		sources.clear();
		AL_Reload();
		reloadSources = true;
	}

	ImGui::Text("parts size (%.i)", Debris.Parts.size());

	ImGui::Text("Camera position { %.2f ; %.2f }", CameraPosition.x, CameraPosition.y);
	ImGui::Text("Mission position { %.2f ; %.2f }", MissionSelectMenu.missionPosition.x, MissionSelectMenu.missionPosition.y);
	ImGui::Text("Exposure { %.2f }", Exposure );
	ImGui::Text("AccumulatedHeat { %.2f }", AccumulatedHeat );
	
	if (ImGui::Button("inside"))
	{
		inside = !inside;
	}
	ImGui::SliderFloat("AmbientLight", &AmbientLight, 0.0f, 1.0f);
	

	ExplodionLightHeight *= 10.0f;
	EngineLightHeight *= 10.0f;
	BulletHitLightHeight *= 10.0f;

	ImGui::SliderFloat("Explodion Light Height", &ExplodionLightHeight, -0.1f, 0.2f);
	ImGui::SliderFloat("Engine Light Height", &EngineLightHeight, -0.1f, 0.2f);
	ImGui::SliderFloat("BulletHit Light Height", &BulletHitLightHeight, -0.1f, 0.2f);

	ExplodionLightHeight *= 0.1f;
	EngineLightHeight *= 0.1f;
	BulletHitLightHeight *= 0.1f;

	if (ImGui::Button("VSync"))
	{
		if (VSync)
		{
			glfwSwapInterval(0);
			VSync = 0;
		}
		else
		{
			glfwSwapInterval(1);
			VSync = 1;
		}
	}
	//float a[2] = { Entities[0].direction.x,Entities[0].direction.y };
	//ImGui::DragFloat2("Direction", a);
	ImGui::Text("zoom = %.2f", CameraScale.x);
	ImGui::End();


	int currentObject = 1;
	balls.clear();


	
	if (clock() > 250 && (!inbase || MainMenu))
	{
		for (int i = 0; i < StarsAmount; i++)
		{
			if (Stars[i].x > CameraPosition.x + StarsSpread * 0.5f)
			{
				Stars[i].x = CameraPosition.x - StarsSpread * 0.5f;
				Stars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (Stars[i].x < CameraPosition.x - StarsSpread * 0.5f)
			{
				Stars[i].x = CameraPosition.x + StarsSpread * 0.5f;
				Stars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (Stars[i].y > CameraPosition.y + StarsSpread * 0.5f)
			{
				Stars[i].y = CameraPosition.y - StarsSpread * 0.5f;
				Stars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (Stars[i].y < CameraPosition.y - StarsSpread * 0.5f)
			{
				Stars[i].y = CameraPosition.y + StarsSpread * 0.5f;
				Stars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}

			DrawCube(glm::vec2(Stars[i]), glm::vec2(Stars[i].z * 0.25f), Stars[i].w, glm::vec4(1.0f, 1.0f, 1.0f, Stars[i].z * 10.0f / StarsDepth),false,0,-1000);

		}
		sw->End();
		bw->Use();
		for (int i = 0; i < StarsAmount*0.2; i++)
		{
			if (BackgroundStars[i].x > CameraPosition.x + StarsSpread * 0.5f)
			{
				BackgroundStars[i].x = CameraPosition.x - StarsSpread * 0.5f;
				BackgroundStars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (BackgroundStars[i].x < CameraPosition.x - StarsSpread * 0.5f)
			{
				BackgroundStars[i].x = CameraPosition.x + StarsSpread * 0.5f;
				BackgroundStars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (BackgroundStars[i].y > CameraPosition.y + StarsSpread * 0.5f)
			{
				BackgroundStars[i].y = CameraPosition.y - StarsSpread * 0.5f;
				BackgroundStars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (BackgroundStars[i].y < CameraPosition.y - StarsSpread * 0.5f)
			{
				BackgroundStars[i].y = CameraPosition.y + StarsSpread * 0.5f;
				BackgroundStars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}

			DrawCube(glm::vec2(BackgroundStars[i]*2.0f), glm::vec2(BackgroundStars[i].z * 1.25f), BackgroundStars[i].w, glm::vec4(1.0f, 1.0f, 1.0f, BackgroundStars[i].z * 10.0f / StarsDepth),false,0,-1000);
		}
		bw->End();
		sw->Use();
		glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		ScreenMousePosition = WindowMousePosition;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		foregroundMousePosition =MousePosition; 
	}
	
	if (!Settings) {
		if (keys[GLFW_KEY_1])
		{

			DamageSphere DS;
			DS.body.r = 10.25f;
			DS.body.position = MousePosition;
			DS.Damage = false;
			DS.timeLeft = 0.044f;
			DS.Heat = 0.0f;
			DS.recoil = -1.0f;
			DS.neutral = true;
			//DS.friction = 0.00f;
			DS.singleHit = false;
			DamageSpheresArray.push_back(DS);
		}
		if (JustPressedkey[GLFW_KEY_2])
		{
			Entities.push_back(new CentralPart);
			Entities[Entities.size() - 1]->Create(MousePosition, { 0.0f,1.0f }, PARTSIZE);
			Entities[Entities.size() - 1]->LoadFrom("Save0.sav");
			Entities[Entities.size() - 1]->autocontrol = true;
			Entities[Entities.size() - 1]->trgPos = MousePosition;
			lastEntityID++;
			Entities[Entities.size() - 1]->id = lastEntityID;
			//Entities[Entities.size() - 1]->CP.Health = -10;

		}
		if (JustPressedkey[GLFW_KEY_3])
		{
			Entities.push_back(new CentralPart);
			Entities[Entities.size() - 1]->Create(MousePosition, { 0.0f,1.0f }, PARTSIZE);
			Entities[Entities.size() - 1]->LoadFrom("Bigboy.sav");
			Entities[Entities.size() - 1]->autocontrol = true;
			Entities[Entities.size() - 1]->trgPos = MousePosition;
			lastEntityID++;
			Entities[Entities.size() - 1]->id = lastEntityID;
			//Entities[Entities.size() - 1]->CP.Health = -10;

		}
		if (JustPressedkey[GLFW_KEY_4])
		{
			SpawnExplodion(MousePosition, 10.0f, 0.5f, 0.4f);
		}
	}
	

	ProcessPE(dt);

	ProcessLightEffects(dt);
	DrawExplodions(dt);
	
	if (!OpenMenu && !MainMenu)
		ProcessPlayerControls();
	
	if(MissionSelectMenu.Hub && Entities.size()>0)
	{
		if(sqrlength(Entities[0]->mid)<600*600)
			inside = true;
		else
			inside = false;
	}

	if(!inside && Entities.size()>0)
	{
		AccumulatedHeat += (Entities[0]->sumheat - 5.0f) * dt * 0.1f;
		if(AccumulatedHeat<0.0f)
			AccumulatedHeat = 0.0f;
		
		if(AccumulatedHeat>400.0f)
		{
			AccumulatedHeat-=400.0f;
			SpawnAiShip(Entities[0]->mid + glm::vec2(0.0f,1000.0f),"Bigboy")->AIState = 1;
		}
	}
	else
	{
		AccumulatedHeat -= 10.0f * dt;
		if(AccumulatedHeat<0.0f)
			AccumulatedHeat = 0.0f;
	}
	ProcessCamera(dt);
	
    GameScene->Draw(dt);
	int ent=0;
	
	while (ent < Entities.size())
	{
		if (!Entities[ent]->Alive && !Entities[ent]->destroyed || Entities[ent]->Health<=0.0f || Entities[ent]->Delete)
		{
			
			LightEffect le;
			le.volume = 0.005f;
			le.S_Color = { 1.0f,20.0f,200.0f,1.0f };
			le.S_Scale = 250.0f;
			le.time = 3.0f;
			le.maxT = 3.0f;
			le.position = glm::vec3(Entities[ent]->mid,0.0f);
			LightEffects.push_back(le);
			Entities[ent]->Destroy();
			Entities[ent]->Delete=true;
			Entities[ent] = Entities[Entities.size() - 1];
			Entities.pop_back();
		}
		else
			ent++;
	}
	CurrnetMission.CheckShips(dt);
	sw->End();

	if (OpenMenu || MainMenu)
		ProcessMainMenu();

	if(MissionSelectMenu.Hub)
		bw->w_CameraPosition = (sw->w_CameraPosition )*0.05f;
	else
		bw->w_CameraPosition = (sw->w_CameraPosition + MissionSelectMenu.missionPosition)*0.05f;
	
	bw->Use();
	Background.DrawCollisions = false;
	ProcessScene(&Background,true,false);
	Background.Draw(delta);
	
	bw->End();

	cw->Use();
	
	
	float ConsoleYStep = 15.0f;
	float ConsoleTextSize = 0.4f;
	glm::vec4 consoleTextColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);

	float posy = HEIGHT * 0.5f - ConsoleYStep * 4.0f;
	float posx = WIDTH * -0.5f + ConsoleYStep * 2.0f;
	for(int i=0;i<ConsoleTexts.size();i++)
	{
		DrawText(ConsoleTexts[i],{posx,posy},ConsoleTextSize,consoleTextColor);
		posy-=ConsoleYStep;
	}
	

	cw->End();
	

    UseWindow(SceneWindowID);
	AmbientLight = 1.0f;
    bw->Draw(0);

    sw->Draw(1);

    cw->Draw(2);

    mw->Draw(3);
	

}

void PreReady()
{
    // Remake with DataStorage
	std::cout<<"Pre ready\n";
	std::ifstream f("Settings.sav");
	if (f.is_open())
		while (!f.eof())
		{

			char junk;
			char line[256];
			f.getline(line, 256);
			std::strstream s;
			s << line;



			if (line[0] == 'R' && line[1] == 'e' && line[2] == 's')
				s >> junk >> junk >> junk >> s_Resolution.x >> s_Resolution.y;

			else if (line[0] == 'B' && line[1] == 'r' && line[2] == ' ')
				s >> junk >> junk >> brightness;

			else if (line[0] == 'B' && line[1] == 'l' && line[2] == ' ')
				s >> junk >> junk >> bloom;

			else if (line[0] == 'F' && line[1] == 'S' && line[2] == ' ')
				s >> junk >> junk >> s_Fullscreen;
			else if (line[0] == 'S' && line[1] == 'h' && line[2] == ' ')
				s >> junk >> junk >> s_ScreenShake;
			else if (line[0] == 'C' && line[1] == 'a' && line[2] == ' ')
				s >> junk >> junk >> s_ChromaticAbberation;

		}

	f.close();

	std::cout<<"settings ready\n";
	PartsData.Load("PartsProperties.ds");
	InitParts();
	PartsData.Save("PartsProperties.ds");
	
	std::cout<<"part props ready\n";



	
}

void SceneEnd()
{

	for(int i=0;i<Entities.size();i++)
		Entities[i]->Destroy();
	Entities.clear();

	for (int i = 0; i < Debris.Parts.size(); i++)
		Debris.Parts[i]->DeletePart();
	Debris.Parts.clear();

	DamageSpheres.clear();
	DamageSpheresArray.clear();
	bullets.clear();
	Rockets.clear();
	Lasers.clear();
	LightEffects.clear();
	clearParticleMaterials();
	CurrnetMission.TakenAreas.clear();
	CurrnetMission.TakenAreas.push_back({0,0,50,50});
	CurrnetMission.AIShips.clear();
	BuildingMode = false;
	vLogicMode = false;
	bLogicMode = false;
	fLogicMode = false;
	MainMenu = true;
}
void Rescale(int newWindth,int newHeight)
{

	Window* sw = GetWindow(ForeWindowID);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* mw = GetWindow(MenuWindowID);
	sw->ViewportSize = GetWindow(SceneWindowID)->ViewportSize;
	sw->RecalculateSize();

	bw->ViewportSize = GetWindow(SceneWindowID)->ViewportSize;
	bw->RecalculateSize();

	mw->ViewportSize = GetWindow(SceneWindowID)->ViewportSize;
	mw->RecalculateSize();

	float maxsize = fmaxf(sw->ViewportSize.x, sw->ViewportSize.y);
	TextSize = maxsize * 0.0005f * 0.35f;
	UISize = 25 * maxsize * 0.0005f;

}

void Destroy()
{	
	SavePlayerData();
	CurrnetMission.TakenAreas.clear();
	CurrnetMission.TakenAreas.push_back({0,0,50,50});
	CurrnetMission.AIShips.clear();
	SaveSettings();
}