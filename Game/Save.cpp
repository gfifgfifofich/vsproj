#include <filesystem>

#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"

#include "Textures.h"
#include "Helper.h"
#include "Radar.h"

#include "Bullet.h";
#include "Rocket.h";
#include "Laser.h";
#include "Mission.h";

void QuickSave(std::string SaveFileName)
{
	std::string fullpath = "Saves/";
	fullpath += SaveFileName;
	
	DataStorage PlayerDataDS;
	PlayerDataDS.SetProperty(SaveFileName, "lastEntityName", lastEntityName);
	PlayerDataDS.SetProperty(SaveFileName, "EntityCount", int(Entities.size()));
	PlayerDataDS.SetProperty(SaveFileName, "Materials", Materials);
	PlayerDataDS.SetProperty(SaveFileName, "EntityBackUpName", EntityBackUpName);
	PlayerDataDS.SetProperty(SaveFileName, "storyint", storyint);
	PlayerDataDS.SetProperty(SaveFileName, "lastEntityID", lastEntityID);
	PlayerDataDS.SetProperty(SaveFileName, "MissionSelectMenu.missionPosition", MissionSelectMenu.missionPosition);
	PlayerDataDS.SetProperty(SaveFileName, "MissionSelectMenu.Hub", MissionSelectMenu.Hub);
	PlayerDataDS.SetProperty("LastSave", "LastSaveName", "Save0");
	

	// came up with a better idea about saving ships (best - load the contents of folder from filesystem)
	for (int i = 0; i < ShipInfos.size(); i++)
		PlayerDataDS.SetProperty("ShipFileNames", ShipInfos[i].filename, ShipInfos[i].MateralCost);

	PlayerDataDS.Save(fullpath + "/main.sav");


	DataStorage RadarDataDS;
	RadarDataDS.SetProperty("Radar", "offset", ActiveRadar.offset);
	RadarDataDS.Save(fullpath + "/radar.sav");


	int EntitiesSize = Entities.size();
	for (int i = 0; i < Entities.size(); i++)
	{
		std::string filename = fullpath + "/";
		filename += std::to_string(i) + ".sav";
		Entities[i]->SaveTo(filename, true, true);
	}
	GameScene->SaveAs(fullpath + "/Scene.sav");


	DataStorage MissionDataDS;

	MissionDataDS.SetProperty("Mission", "type", CurrnetMission.type);
	MissionDataDS.SetProperty("Mission", "dificulty", CurrnetMission.dificulty);
	MissionDataDS.SetProperty("Mission", "size", CurrnetMission.size);
	MissionDataDS.SetProperty("Mission", "materialReward", CurrnetMission.materialReward);
	MissionDataDS.SetProperty("Mission", "story_mission", CurrnetMission.story_mission);
	MissionDataDS.SetProperty("Mission", "exiting", CurrnetMission.exiting);
	MissionDataDS.SetProperty("Mission", "compleated", CurrnetMission.compleated);
	MissionDataDS.SetProperty("Mission", "missionpos", CurrnetMission.missionpos);
	MissionDataDS.SetProperty("Mission", "planetpos", CurrnetMission.planetpos); // why not? save all the shit, sort out later

	for (int i = 0; i < CurrnetMission.TakenAreas.size(); i++)
		MissionDataDS.SetProperty("TakenAreas", std::to_string(i), CurrnetMission.TakenAreas[i]);

	for (int i = 0; i < CurrnetMission.timers.size(); i++)
		MissionDataDS.SetProperty("timers", std::to_string(i), CurrnetMission.timers[i]);

	for (auto a : CurrnetMission.flags)
		MissionDataDS.SetProperty("flags",	a.first, a.second);

	MissionDataDS.Save(fullpath + "/MissionState.sav");
}

void QuickLoad(std::string SaveFileName)
{
	std::string fullpath = "Saves/";
	fullpath += SaveFileName;

	DataStorage RadarDataDS;
	RadarDataDS.Load(fullpath + "/radar.sav");
	ActiveRadar.offset = RadarDataDS.GetPropertyAsVec2("Radar", "offset");

	int EntitiesSize = 0;
	DataStorage PlayerDataDS;
	PlayerDataDS.Load(fullpath + "/main.sav");
	lastEntityName = PlayerDataDS.GetProperty(SaveFileName, "lastEntityName");
	EntitiesSize = PlayerDataDS.GetPropertyAsInt(SaveFileName, "EntityCount");
	Materials = PlayerDataDS.GetPropertyAsInt(SaveFileName, "Materials");
	EntityBackUpName = PlayerDataDS.GetProperty(SaveFileName, "EntityBackUpName");
	storyint = PlayerDataDS.GetPropertyAsInt(SaveFileName, "storyint");
	lastEntityID = PlayerDataDS.GetPropertyAsInt(SaveFileName, "lastEntityID");
	MissionSelectMenu.missionPosition = PlayerDataDS.GetPropertyAsVec2(SaveFileName, "MissionSelectMenu.missionPosition");
	inbase = PlayerDataDS.GetPropertyAsBool(SaveFileName, "inbase");
	MissionSelectMenu.Hub = PlayerDataDS.GetPropertyAsBool(SaveFileName, "MissionSelectMenu.Hub");
	//PlayerDataDS.GetProperty("LastSave", "LastSaveName","Save0");

	// come up with a better idea about saving ships (best - load the contents of folder from filesystem)
	//for (int i = 0; i < ShipInfos.size(); i++)
	// 	PlayerDataDS.SetProperty("ShipFileNames", ShipInfos[i].filename);






	for (int i = 0; i < Entities.size(); i++)
	{
		for (int a = 0; a < Entities[i]->Parts.size(); a++)
		{
			Entities[i]->Parts[a]->DeletePart();
		}
	}
	//if(Entities.size()>=1)
	Entities.clear();

	for (int i = 0; i < Debris.Parts.size(); i++)
		Debris.Parts[i]->DeletePart();
	Debris.Parts.clear();

	for (int i = 0; i < 100'000; i++)
	{
		IsBall[i] = false;
	}
	lastid = 0;
	lastEntityID = 0;
	freeBallIDs.clear();
	DamageSpheres.clear();
	DamageSpheresArray.clear();
	bullets.clear();
	Rockets.clear();
	Lasers.clear();
	LightEffects.clear();

	GameScene->LoadFrom(fullpath + "/Scene.sav");

	//instantly delete all parts from scene.
	for (int i = 0; i < GameScene->Nodes.size(); i++)
		if (GameScene->Nodes[i]->type >= NodeType::LASTNODE)
			GameScene->Nodes[i]->Delete = true;

	// SpawnEntities
	for (int i = 0; i < EntitiesSize; i++)
	{
		std::string filename = fullpath + "/";
		filename += std::to_string(i) + ".sav";
		SpawnShipAllSaveFullPath(filename);
	}

	DataStorage MissionDataDS;
	MissionDataDS.Load(fullpath + "/MissionState.sav");

	CurrnetMission.type = MissionDataDS.GetPropertyAsInt("Mission", "type");
	CurrnetMission.dificulty = MissionDataDS.GetPropertyAsInt("Mission", "dificulty");
	CurrnetMission.size = MissionDataDS.GetPropertyAsInt("Mission", "size");
	CurrnetMission.materialReward = MissionDataDS.GetPropertyAsInt("Mission", "materialReward");
	CurrnetMission.story_mission = MissionDataDS.GetPropertyAsBool("Mission", "story_mission");
	CurrnetMission.exiting = MissionDataDS.GetPropertyAsBool("Mission", "exiting");
	CurrnetMission.compleated = MissionDataDS.GetPropertyAsBool("Mission", "compleated");
	CurrnetMission.missionpos = MissionDataDS.GetPropertyAsVec2("Mission", "missionpos");
	CurrnetMission.planetpos = MissionDataDS.GetPropertyAsVec2("Mission", "planetpos");

	CurrnetMission.UpdateScene();
	std::map<std::string, std::string> S_TakenAreas;
	std::map<std::string, std::string> S_timers;
	std::map<std::string, std::string> S_flags;

	S_TakenAreas = MissionDataDS.GetObject("TakenAreas");
	S_timers = MissionDataDS.GetObject("timers");
	S_flags = MissionDataDS.GetObject("flags");

	int i = 0;
	for (auto a : S_TakenAreas)
	{
		CurrnetMission.TakenAreas[i] = MissionDataDS.GetPropertyAsVec4("TakenAreas", a.first);
		i++;
	}

	i = 0;
	for (auto a : S_timers)
	{
		CurrnetMission.timers[i] = MissionDataDS.GetPropertyAsFloat("timers", a.first);
		i++;
	}

	i = 0;
	for (auto a : S_flags)
		CurrnetMission.flags[a.first] = MissionDataDS.GetPropertyAsBool("flags", a.first);

}