
#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"

#include "Textures.h"

#include "Helper.h"

void LoadTextures()
{
	LoadTexture("./Textures/HeatPipe.png", &HeatPipeTexture);
	LoadTexture("./Textures/HeatPipe NormalMap.png", &HeatPipeNormalMap);

	LoadTexture("./Textures/Rope.png", &RopeTexture);
	LoadTexture("./Textures/Rope NormalMap.png", &RopeNormalMap);

	LoadTexture("./Textures/Strut.png", &StrutTexture);
	LoadTexture("./Textures/Strut NormalMap.png", &StrutNormalMap);

	LoadTexture("./Textures/RocketEngine.png", &RocketEngineTexture);
	LoadTexture("./Textures/RocketEngine NormalMap.png", &RocketEngineNormalMap);

	LoadTexture("./Textures/Radiator.png", &RadiatorTexture);
	LoadTexture("./Textures/Radiator NormalMap.png", &RadiatorNormalMap);

	LoadTexture("./Textures/Gun.png", &GunTexture);
	LoadTexture("./Textures/Gun NormalMap.png", &GunNormalMap);

	LoadTexture("./Textures/LaserGun.png", &LaserGunTexture);
	LoadTexture("./Textures/LaserGun NormalMap.png", &LaserGunNormalMap);

	LoadTexture("./Textures/MiniGun.png", &MiniGunTexture);
	LoadTexture("./Textures/MiniGun NormalMap.png", &MiniGunNormalMap);

	LoadTexture("./Textures/GunBase.png", &GunBaseTexture);
	LoadTexture("./Textures/GunBase NormalMap.png", &GunBaseNormalMap);

	LoadTexture("./Textures/RocketLauncher.png", &RocketLauncherTexture);
	LoadTexture("./Textures/RocketLauncher NormalMap.png", &RocketLauncherNormalMap);

	LoadTexture("./Textures/BallBody.png", &BallBodyTexture);
	LoadTexture("./Textures/BallBody NormalMap.png", &BallBodyNormalMap);

	LoadTexture("./Textures/CentralPart.png", &CentralPartTexture);
	LoadTexture("./Textures/CentralPart NormalMap.png", &CentralPartNormalMap);

	LoadTexture("./Textures/Rotor.png", &RotorTexture);
	LoadTexture("./Textures/Rotor NormalMap.png", &RotorNormalMap);

	LoadTexture("./Textures/Pipe.png", &PipeTexture);
	LoadTexture("./Textures/Pipe NormalMap.png", &PipeNormalMap);

	LoadTexture("./Textures/debrie0.png", &Debrie0Texture);
	LoadTexture("./Textures/debrie1.png", &Debrie1Texture);


}

void LoadPlayerData()
{
	ShipInfos.clear();
	GameSaveFile.Load("SaveFile.sav");
	lastEntityName = GameSaveFile.GetProperty(GameSaveName, "lastEntityName");
	EntityBackUpName = GameSaveFile.GetProperty(GameSaveName, "EntityBackUpName");
	Materials = GameSaveFile.GetPropertyAsFloat(GameSaveName, "Materials");
	storyint = GameSaveFile.GetPropertyAsFloat(GameSaveName, "storyint");

	std::vector<std::string> shipfilenames = GameSaveFile.GetProperties("ShipFileNames");
	for (int i = 0; i < shipfilenames.size(); i++)
	{
		shipInfo si;
		si.filename = shipfilenames[i];
		si.MateralCost = GameSaveFile.GetPropertyAsFloat("ShipFileNames", shipfilenames[i]);
		ShipInfos.push_back(si);
	}
}
void SavePlayerData()
{
	GameSaveFile.SetProperty(GameSaveName, "lastEntityName", lastEntityName);
	GameSaveFile.SetProperty(GameSaveName, "Materials", Materials);
	GameSaveFile.SetProperty(GameSaveName, "EntityBackUpName", EntityBackUpName);
	GameSaveFile.SetProperty(GameSaveName, "storyint", storyint);
	GameSaveFile.SetProperty("LastSave", "LastSaveName", "Save0");

	// came up with a better idea about saving ships (best - load the contents of folder from filesystem)
	for (int i = 0; i < ShipInfos.size(); i++)
		GameSaveFile.SetProperty("ShipFileNames", ShipInfos[i].filename, ShipInfos[i].MateralCost);

	GameSaveFile.Save("SaveFile.sav");
}


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

void ProcessLightEffects(float dt)
{

	for (int i = 0; i < LightEffects.size(); i++)
	{

		float step = LightEffects[i].time / LightEffects[i].maxT;
		glm::vec2 scale = glm::vec2(LightEffects[i].S_Scale * step + LightEffects[i].E_Scale * (1.0f - step));
		glm::vec4 color = LightEffects[i].S_Color * step + LightEffects[i].E_Color * (1.0f - step);
		DrawLight(LightEffects[i].position,
			scale,
			color,
			LightEffects[i].volume);
		AddLightSphere(LightEffects[i].position, (scale.x + scale.y) * 0.5f, color);
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

void ProcessCamera(float dt)
{
	Window* sw = GetWindow(ForeWindowID);
	sw->Use();
	if (Exposure < 0.85f)
		Exposure += dt;
	else
		Exposure = 0.85f;
	SceneExposure = Exposure * Exposure * brightness;
	CameraPosition = camerapos + glm::vec2(((rand() % 100) * ScreenShake - ScreenShake * 50.0f) * s_ScreenShake, ((rand() % 100) * ScreenShake - ScreenShake * 50.0f) * s_ScreenShake);
	ScreenShake -= ScreenShakeFallOff * dt;
	ChromaticAbberation -= ChromaticAbberationFallOff * dt;
	if (ScreenShake < 0.0f) ScreenShake = 0.0f;
	if (ChromaticAbberation < 0.0f) ChromaticAbberation = 0.0f;

	if (ScreenShake > 1.0f) ScreenShake = 1.0f;
	if (ChromaticAbberation > 0.3f) ChromaticAbberation = 0.3f;
	ChromaticStrength = ChromaticAbberation * s_ChromaticAbberation;
	ListenerPos = CameraPosition;
	UpdateListenerPosition();
	sw->End();
}


void SpawnPlayer(std::string filename)
{

	glm::vec2 position = glm::vec2(-10, 0.0f);
	glm::vec2 Scale = glm::vec2(0.5f, 0.5f);
	Entities.push_back(new CentralPart);
	Entities[0]->Create(glm::vec2(0.0f, 0.0f) * Scale + position, { 0.0f,1.0f }, PARTSIZE);
	Entities[0]->LoadFrom(filename);
	std::cout << "\nPlayer Spawned";
}
void SpawnPlayer(glm::vec2 position, std::string filename)
{
	glm::vec2 Scale = glm::vec2(0.5f, 0.5f);
	Entities.push_back(new CentralPart);
	Entities[0]->Create(glm::vec2(0.0f, 0.0f) * Scale + position, { 0.0f,1.0f }, PARTSIZE);
	Entities[0]->LoadFrom(filename);
	std::cout << "\nPlayer Spawned";
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
