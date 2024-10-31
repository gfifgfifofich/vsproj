#pragma once

#include "ParticleMaterials.h"
#include "DamageSphere.h"
#include "Parts/smallball.h"
#include "Parts/Base.h"
#include "Parts/CentralPart.h"


inline int BackgroundWindowID;// background cosmetic window
inline int ForeWindowID;// "game" window
inline int ForeGroundWindowID;// foreground cosmetic window
inline int InterfaceWindowID;// player interface window
inline int MenuWindowID;
inline glm::vec2 foregroundMousePosition;

inline float UpdateTime;
inline float DrawTime;

inline float TextSize;
inline float UISize;

#ifndef PARTSIZE 
#define PARTSIZE 0.5f // probably should've just used const floatm idk why i used define
#endif // !PARTSIZE 



inline glm::vec2 camerapos;// cameraposition that sets cameraposition in ProcessCamera() + screenshake
inline int GridCellSize;// gird that doesnt exist anymore
inline bool reloadSources; // reload sound system
inline float brightness;// exposure modifier
inline int SRiter; // screen res iter


inline DataStorage GameSaveFile;
inline int Materials;
inline int storyint;
inline std::string lastEntityName;
inline std::string EntityBackUpName;
inline std::string GameSaveName;
inline std::vector<std::string> ConsoleTexts;
inline bool inside;
inline bool inbase;

struct shipInfo
{
	std::string filename = "";
	int MateralCost = 0;
};
inline std::vector <shipInfo> ShipInfos;
inline std::vector<std::string> shipNames;

inline std::string RX;
inline std::string RY;


inline bool loadedThisFrame;

inline float Speed;

inline int NewConType;
inline int NewConPart1;
inline int NewConPart2;
inline int NewConIndex1;
inline int NewConIndex2;
inline int ConCreationStage;

inline bool NewConDebrie1;
inline bool NewConDebrie2;

inline int newPartType;//-1 = not selected
inline float newPartRotation;

inline bool balltaken;

inline bool absoluteControl;
inline bool BuildingMode;

inline bool bLogicMode;
inline bool fLogicMode;
inline bool vLogicMode;
inline int DataconnectionData[6];

inline int lastEntityID = 0;

inline std::string saveFileName;

inline int in_UI;// <=0 - not, else - browsing ui // compleat bullshit

inline bool createdFreezeDS; // why is that a thing?

inline bool align;
inline bool snapToGrid;

inline int NewConBall1;
inline int NewConBall2;

inline const int StarsAmount = 3000;
inline float StarsSpread;
inline float StarsDepth;
inline glm::vec4 Stars[StarsAmount];
inline glm::vec4 BackgroundStars[StarsAmount];


inline bool MainMenu;
inline bool switchScene;
inline bool switchToMission;
inline bool OpenMenu;
inline bool Settings;
inline int lastsound;
inline int lastsound2;
inline unsigned int lastplayedsound;

inline float Exposure;

inline Scene Background;

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

inline std::vector<LightEffect> LightEffects;


void ChangeMap(std::string FilePath, glm::vec2 lastoffset, glm::vec2 newoffset); // implemented in Source.cpp

void LoadPlayerData();
void SavePlayerData();
void QuickSave(std::string SaveFileName);
void QuickLoad(std::string SaveFileName);

void LoadTextures();

void ResetSettings();
void SaveSettings();

void ProcessLightEffects(float dt);

void ProcessCamera(float dt);


void SpawnPlayer(std::string filename = "PreMissionBackup");
void SpawnPlayer(glm::vec2 position, std::string filename = "PreMissionBackup");

CentralPart* SpawnAiShip(glm::vec2 pos, std::string name, bool fullpath = false);

CentralPart* SpawnShipAllSave(std::string name);
CentralPart* SpawnShipAllSaveFullPath(std::string name);
