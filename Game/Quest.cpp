#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"
#include "ParticleMaterials.h"
#include "Textures.h"
#include "DamageSphere.h"
#include "Explodion.h"
#include "Rocket.h"
#include "Parts/smallball.h"
#include "Parts/Base.h"
#include "Parts/CentralPart.h"
#include "Radar.h"
#include "Mission.h"
#include "Helper.h"
/*
	different colors on map for different station fractions // jsut slap some periodic bleeps
*/

void Quest::Start()
{
	flags["Failed"] = false;
	flags["Done"] = false;
	if (missiontype == MissionType::pirates)
	{
		Radar::Bleep bl;
		bl.state = 3;
		bl.size = size;
		bl.dificulty = dificulty;
		bl.materialReward = materialreward;
		bl.position = Entities[0]->mid + ActiveRadar.offset + Rotate(Normalize(Entities[0]->avgvel) * (9000.0f + (rand() % 2000)), ((rand() % 2000) * 0.001f - 1.0f) * pi * 0.5f);
		bl.velocity = { 0.0f,0.0f };
		bl.infinite = true;
		bl.r = 35.0f;
		bl.fogamount = 0.0f;
		bl.colisionR = 3500.0f;
		bl.questid = id;
		bl.fraction = fraction2;
		for (int aaa = 0; aaa < 5; aaa++)
			for (int a = 0; a < ActiveRadar.bleeps.size(); a++)
			{
				if (ActiveRadar.bleeps[a].state == -1 || ActiveRadar.bleeps[a].state >= 2)
				{
					glm::vec2 dif = bl.position - ActiveRadar.bleeps[a].position;

					if (sqrlength(dif) < (bl.colisionR + ActiveRadar.bleeps[a].colisionR) * (bl.colisionR + ActiveRadar.bleeps[a].colisionR))
					{
						glm::vec2 norm = dif / length(dif);
						glm::vec2 dd = norm * ((bl.colisionR + ActiveRadar.bleeps[a].colisionR) - length(dif));
						bl.position += dd;
					}
				}
			}
		ActiveRadar.bleeps.push_back(bl);
	}
	if (missiontype == MissionType::SupplySabotage)
	{
		glm::vec2 pos =  glm::vec2(3000.0f, 0.0f);
		CentralPart* cp = SpawnAiShip(pos, "Carier");
		cp->questid = id;
		float lastl = 100000000.0f;
		glm::vec2 nearestpos = { 0.0f,0.0f };
		for (int i = 0; i < ActiveRadar.bleeps.size(); i++)
		{
			if (ActiveRadar.bleeps[i].state == -1 && ActiveRadar.bleeps[i].fraction != fraction)
			{
				if (sqrlength((ActiveRadar.offset + pos) - ActiveRadar.bleeps[i].position) < lastl)
				{
					lastl = sqrlength((ActiveRadar.offset + pos) - ActiveRadar.bleeps[i].position);
					nearestpos = ActiveRadar.bleeps[i].position;
				}
			}
		}
		positions["supply_target"] = nearestpos;
		cp->avgvel = Normalize(nearestpos - (ActiveRadar.offset + pos)) * 150.0f;
		for (auto p : cp->Parts)
		{
			for (int i = 0; i < p->body.size(); i++)
			{
				ballVelocity[p->body[i]] = Normalize(nearestpos - (ActiveRadar.offset + pos)) * 150.0f;
			}
		}
	}
	if (missiontype == MissionType::Transportation)
	{
		
		glm::vec2 pos = { 0.0f,0.0f }; 
		float lastl = 100000000.0f;
		glm::vec2 nearestpos = { 0.0f,0.0f };
		for (int i = 0; i < ActiveRadar.bleeps.size(); i++)
		{
			if (ActiveRadar.bleeps[i].state == -1  && sqrlength(ActiveRadar.bleeps[i].position - ActiveRadar.offset) > 2000*2000)
			{
				if (sqrlength(ActiveRadar.bleeps[i].position - ActiveRadar.offset) < lastl)
				{
					lastl = sqrlength(ActiveRadar.bleeps[i].position - ActiveRadar.offset);
					nearestpos = ActiveRadar.bleeps[i].position;
				}
			}
		}

		positions["supply_target"] = nearestpos;
		counters["delivered"] = 0;
		counters["destroyed"] = 0;
		//------------------------------------------------------------------------------------------------------------------------ Hardcoded array access
		pos = { 0.0f,0.0f };
		for (int i = 0; i < size; i++)
		{
			BodyComponent* bc = CreatePart(PurchasableParts[4], pos, { 0.0f,1.0f }, PARTSIZE, 1.0f);
			bc->questid = id;

			Debris.Parts.push_back(bc);
			pos.x += 2.0f;
		}
	}
	if (missiontype == MissionType::PartRecovery)
	{
		Radar::Bleep bl;
		bl.state = MissionType::PartRecovery + 3;
		bl.size = size;
		bl.dificulty = dificulty;
		bl.materialReward = materialreward;
		bl.position = Entities[0]->mid + ActiveRadar.offset + Rotate(Normalize(Entities[0]->avgvel) * (9000.0f + (rand() % 2000)), ((rand() % 2000) * 0.001f - 1.0f) * pi * 0.5f);
		bl.velocity = { 0.0f,0.0f };
		bl.infinite = true;
		bl.r = 35.0f;
		bl.fogamount = 0.0f;
		bl.colisionR = 3500.0f;
		bl.questid = id;
		bl.fraction = fraction2;
		for (int aaa = 0; aaa < 5; aaa++)
			for (int a = 0; a < ActiveRadar.bleeps.size(); a++)
			{
				if (ActiveRadar.bleeps[a].state == -1 || ActiveRadar.bleeps[a].state >= 2)
				{
					glm::vec2 dif = bl.position - ActiveRadar.bleeps[a].position;

					if (sqrlength(dif) < (bl.colisionR + ActiveRadar.bleeps[a].colisionR) * (bl.colisionR + ActiveRadar.bleeps[a].colisionR))
					{
						glm::vec2 norm = dif / length(dif);
						glm::vec2 dd = norm * ((bl.colisionR + ActiveRadar.bleeps[a].colisionR) - length(dif));
						bl.position += dd;
					}
				}
			}
		ActiveRadar.bleeps.push_back(bl);
		positions["supply_target"] = ActiveRadar.offset;
		positions["target_bleep_pos"] = bl.position;
		counters["delivered"] = 0;
		counters["destroyed"] = 0;
	}
}
void Quest::Process(float dt)
{
	bool Selected = false;
	if (JournalSelectedQuest == id)
		Selected = true;

	if (Entities.size() <= 0)
		return;
	if (missiontype == MissionType::pirates)
	{
		if (CurrnetMission.questid == id && CurrnetMission.compleated)
		{
			for (int i = 0; i < ActiveRadar.bleeps.size(); i++)
			{
				if (ActiveRadar.bleeps[i].questid == id)
				{
					ActiveRadar.bleeps[i].infinite = false;
					ActiveRadar.bleeps[i].t = -10.0f;
					flags["Done"] = true;
				}
			}
		}
	}
	if (missiontype == MissionType::SupplySabotage)
	{
		flags["Done"] = true;
		if (Selected)
			MissionTexts.push_back(name + "  id = " + std::to_string(id) );
		if (flags["Failed"])
		{
			if (Selected)
				MissionTexts.push_back("Failed");
			return;
		}
		glm::vec2 shipposition = { 0.0f,0.0f };
		for (int i = 0; i < ActiveRadar.bleeps.size(); i++)
		{
			if (ActiveRadar.bleeps[i].questid == id)
			{
				
				flags["Done"] = false;
				ActiveRadar.bleeps[i].velocity= Normalize(positions["supply_target"] - (ActiveRadar.bleeps[i].position)) * 100.0f;
				shipposition = (ActiveRadar.bleeps[i].position);
				if (sqrlength(shipposition - positions["supply_target"]) < 1000.0f * 1000.0f)
				{
					flags["Failed"] = true;
				}
				if (flags["Failed"])
				{
					ActiveRadar.bleeps[i].infinite = false;
					ActiveRadar.bleeps[i].t = -10.0f;
				}
				if (Selected)
					MissionTexts.push_back("bleeps[" + std::to_string(i) + "]");
				if (Selected)
					MissionTexts.push_back("pos = " + std::to_string((ActiveRadar.bleeps[i].position - (ActiveRadar.offset + Entities[0]->mid)).x)
					+ "  " + std::to_string((ActiveRadar.bleeps[i].position - (ActiveRadar.offset + Entities[0]->mid)).x)
				);
			}
		}
		int i = 0;
		for (auto e : Entities)
		{
			if (e->questid == id)
			{

				flags["Done"] = false;
				e->trgPos = positions["supply_target"] - (ActiveRadar.offset);
				if (sqrlength(e->trgPos - e->mid) < 1000.0f * 1000.0f)
				{
					flags["Failed"] = true;
				}
				if (flags["Failed"])
				{
					e->Clear();
					e->Destroy();
				}
				shipposition = e->mid + ActiveRadar.offset;
				if (Selected)
					MissionTexts.push_back("Entities[" + std::to_string(i) + "]");
			}
			i++;
		}
		if (flags["Done"] && !flags["Failed"])
			if (Selected)
				MissionTexts.push_back("Done");

	}
	if (missiontype == MissionType::Transportation)
	{
		if (Selected)
			MissionTexts.push_back(name + "  id = " + std::to_string(id));
		if (Selected)
			MissionTexts.push_back("pos = " + std::to_string((positions["supply_target"] - (ActiveRadar.offset + Entities[0]->mid)).x)
			+ "  " + std::to_string((positions["supply_target"] - (ActiveRadar.offset + Entities[0]->mid)).x));
		if (flags["Failed"])
		{
			if (Selected)
				MissionTexts.push_back("Failed");
			return;
		}

		for (int i = 0; i < Debris.Parts.size(); i++)
		{
			if (Debris.Parts[i]->questid == id && (Debris.Parts[i]->Health < 0 || Debris.Parts[i]->Delete))
			{
				counters["destroyed"]++;
				Debris.Parts[i]->DeletePart();
			}
			if (Debris.Parts[i]->questid == id && Debris.Parts[i]->Health > 0 && !Debris.Parts[i]->Delete && sqrlength(positions["supply_target"] - (ActiveRadar.offset + ballPosition[Debris.Parts[i]->body[0]])) < 1000.0f * 1000.0f)
			{
				counters["delivered"]++;
				Debris.Parts[i]->DeletePart();
			}
		}
		for (int i = 0; i < Entities[0]->Parts.size(); i++)
		{
			if (Entities[0]->Parts[i]->questid == id && (Entities[0]->Parts[i]->Health < 0 || Entities[0]->Parts[i]->Delete))
			{
				counters["destroyed"]++;
				Entities[0]->DestroyPart(i);
			}
			if (Entities[0]->Parts[i]->questid == id)
			{
				if (Selected)
					MissionTexts.push_back("pos = " + std::to_string((positions["supply_target"] - (ActiveRadar.offset + ballPosition[Entities[0]->Parts[i]->body[0]])).x)
						+ "  " + std::to_string((positions["supply_target"] - (ActiveRadar.offset + ballPosition[Entities[0]->Parts[i]->body[0]])).x));
			}
			if (Entities[0]->Parts[i]->questid == id && sqrlength(positions["supply_target"] - (ActiveRadar.offset + ballPosition[Entities[0]->Parts[i]->body[0]])) < 1000.0f * 1000.0f)
			{
				counters["delivered"]++;
				Entities[0]->DestroyPart(i);
			}
		}
		if (counters["delivered"] >= size)
		{
			materialreward = materialreward - materialreward * (counters["destroyed"] / size);
			flags["Done"] = true;
		}
		if (counters["destroyed"] >= size)
		{
			flags["Failed"] = true;
		}
	}
	if (missiontype == MissionType::PartRecovery)
	{

		if (state == 0)
		{
			if (Selected)
				MissionTexts.push_back("goto mission bleep");

			if (Selected)
				MissionTexts.push_back("pos = X" + std::to_string((positions["target_bleep_pos"].x - (ActiveRadar.offset.x + Entities[0]->Parts[0]->mid.x))) + "    Y" + std::to_string((positions["target_bleep_pos"].y - (ActiveRadar.offset.y + Entities[0]->Parts[0]->mid.y))));
		}
		if (state == 1)
		{
			if (Selected)
				MissionTexts.push_back("transfer parts to base");

			if (Selected)
				MissionTexts.push_back("pos = X" + std::to_string((positions["supply_target"].x - Entities[0]->Parts[0]->mid.x)) + "    Y" + std::to_string((positions["supply_target"].y - Entities[0]->Parts[0]->mid.y)));
		}
		for (int i = 0; i < Debris.Parts.size(); i++)
		{
			if (Debris.Parts[i]->questid == id && (Debris.Parts[i]->Health < 0 || Debris.Parts[i]->Delete))
			{
				counters["destroyed"]++;
				Debris.Parts[i]->DeletePart();
			}
			if (Debris.Parts[i]->questid == id && Debris.Parts[i]->Health > 0 && !Debris.Parts[i]->Delete && sqrlength(positions["supply_target"] - (ActiveRadar.offset + ballPosition[Debris.Parts[i]->body[0]])) < 1000.0f * 1000.0f)
			{
				counters["delivered"]++;
				Debris.Parts[i]->DeletePart();
			}
		}
		for (int i = 0; i < Entities[0]->Parts.size(); i++)
		{
			if (Entities[0]->Parts[i]->questid == id && (Entities[0]->Parts[i]->Health < 0 || Entities[0]->Parts[i]->Delete))
			{
				counters["destroyed"]++;
				Entities[0]->DestroyPart(i);
			}
			if (Entities[0]->Parts[i]->questid == id)
			{
				if(Selected)
				MissionTexts.push_back("pos = " + std::to_string((positions["supply_target"] - (ActiveRadar.offset + ballPosition[Entities[0]->Parts[i]->body[0]])).x)
					+ "  " + std::to_string((positions["supply_target"] - (ActiveRadar.offset + ballPosition[Entities[0]->Parts[i]->body[0]])).x));
			}
			if (Entities[0]->Parts[i]->questid == id && sqrlength(positions["supply_target"] - (ActiveRadar.offset + ballPosition[Entities[0]->Parts[i]->body[0]])) < 1000.0f * 1000.0f)
			{
				counters["delivered"]++;
				Entities[0]->DestroyPart(i);
			}
		}
		if (counters["delivered"] >= size)
		{
			materialreward = materialreward - materialreward * (counters["destroyed"] / size);
			flags["Done"] = true;
		}
		if (counters["destroyed"] >= size)
		{
			flags["Failed"] = true;
		}
	}
}