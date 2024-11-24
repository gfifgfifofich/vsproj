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
#include "Helper.h"





void Radar::Spawn(glm::vec2 pos)
{

}

void Radar::Init()
{
	for (int i = 0; i < 10; i++)
	{
		Bleep bl;
		bl.state = 1;
		bl.position = Rotate(glm::vec2(2000.0f + (rand() % 6000), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.velocity = Rotate(glm::vec2((rand() % 150), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.infinite = true;
		bl.r = 5.0f;
		bl.fogamount = 0.0f;
		bl.SaveString = "Ships/BigBoy.sav";
		bleeps.push_back(bl);
	}
	for (int i = 0; i < 15; i++)
	{
		Bleep bl;
		bl.state = 0;
		bl.position = Rotate(glm::vec2(1000.0f + (rand() % 20000), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.velocity = { 0.0f, 0.0f };
		bl.infinite = true;
		bl.r = 25.0f + 25.0f * (rand() % 1000 * 0.001f);
		bl.fogamount = 0.5f + rand() %1000*0.002f;
		bleeps.push_back(bl);
	}
	for (int i = 0; i < 0; i++)
	{
		Bleep bl;
		bl.state = 3;
		bl.position = Rotate(glm::vec2(9000.0f + (rand() % 22000), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.velocity = { 0.0f,0.0f };
		bl.infinite = true;
		bl.r = 15.0f;
		bl.fogamount = 0.0f;
		bl.colisionR = 5500.0f;
		for (int aaa = 0; aaa < 5; aaa++)
			for (int a = 0; a < bleeps.size(); a++)
			{
				if (bleeps[a].state == -1 || bleeps[a].state >= 2)
				{// get 10k away
					glm::vec2 dif = bl.position - bleeps[a].position;

					if (sqrlength(dif) < (bl.colisionR + bleeps[a].colisionR) * (bl.colisionR + bleeps[a].colisionR))
					{
						glm::vec2 norm = dif / length(dif);
						glm::vec2 dd = norm * ((bl.colisionR + bleeps[a].colisionR) - length(dif));
						bl.position += dd;
					}
				}
			}
		bleeps.push_back(bl);
	}
	for (int i = 0; i < 6; i++)
	{
		Bleep bl;
		bl.state = -1;
		bl.position = Rotate(glm::vec2(5000.0f + (rand() % 22000), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.velocity = { 0.0f,0.0f };
		bl.infinite = true;
		bl.r = 15.0f;
		bl.fogamount = 0.0f;
		bl.colisionR = 5500.0f;
		for (int aaa = 0; aaa < 5; aaa++)
			for (int a = 0; a < bleeps.size(); a++)
			{
				if (bleeps[a].state == -1 || bleeps[a].state >= 2)
				{// get 10k away
					glm::vec2 dif = bl.position - bleeps[a].position;

					if (sqrlength(dif) < (bl.colisionR + bleeps[a].colisionR) * (bl.colisionR + bleeps[a].colisionR))
					{
						glm::vec2 norm = dif / length(dif);
						glm::vec2 dd = norm * ((bl.colisionR + bleeps[a].colisionR) - length(dif));
						bl.position += dd;
					}
				}
			}
		bleeps.push_back(bl);
	}
}

void Radar::Process(float dt)
{
	glm::vec2 pos = Entities[0]->mid + offset;
	int a= 0;
	while (a < bleeps.size())
	{
		if (bleeps[a].t <= 0.0f && !bleeps[a].infinite)
		{
			bleeps[a] = bleeps.back();
			bleeps.pop_back();
		}
		else
			a++;
	}
	int fogbleeps = 0;
	int Enemybleeps = 0;
	int SideMissionbleeps = 0;
	int Stationbleeps = 0;

	for (int i = 0; i < bleeps.size(); i++)
	{
		bleeps[i].t -= dt;
		bleeps[i].position += bleeps[i].velocity * dt;

		glm::vec2 bpos = (bleeps[i].position - pos) * 0.01f;
		if (sqrlength(bpos) < 105 * 105)
		{
			if (bleeps[i].state == 1)
				Enemybleeps++;
			if (bleeps[i].state == 0)
				fogbleeps++;
			if (bleeps[i].state > 2)
				SideMissionbleeps++;
			if (bleeps[i].state ==-1)
				Stationbleeps++;
		}

		if (bleeps[i].t <= 0.0f && bleeps[i].infinite)
		{
			bleeps[i].t = 0.75f + (rand() % 60 * 0.01f);
			if (length(bleeps[i].velocity) > 75.0f)
			{
				Bleep bl;
				bl.state = -100;
				bl.t = 1.0f * bleeps[i].r * (0.8f + rand()%60*0.01f);
				bleeps[i].t = (500.0f / length(bleeps[i].velocity)) + (rand() % 60 * 0.01f) ;
				bl.infinite = false;
				bl.justheat = true;
				bl.r = 5.0f;
				bl.position = bleeps[i].position;
				bleeps.push_back(bl);
			}
		}

		if (bleeps[i].state != 1)
			bleeps[i].velocity = { 0.0f,0.0f };

		if (Entities.size() > 0)
		{
			if (bleeps[i].state==1 && sqrt(playerHeat) * 10.0f > length(bleeps[i].position - (Entities[0]->mid + offset)) * 0.01f)
			{
				bleeps[i].velocity = Normalize((Entities[0]->mid + offset) - bleeps[i].position) * 200.0f;
			}					
		}
	}

	for (int i = 0; i < MaxFogBleeps - fogbleeps; i++)
	{
		Bleep bl;
		bl.state = 0;
		bl.position = Entities[0]->mid + offset + Rotate(Normalize(Entities[0]->avgvel) * (9000.0f + (rand() % 2000)), ((rand() % 2000) * 0.001f - 1.0f) * pi * 0.5f);
		bl.velocity = { 0.0f, 0.0f };
		bl.infinite = true;
		bl.r = 25.0f + 25.0f * (rand() % 1000 * 0.001f);
		bl.fogamount = 0.5f + rand() % 1000 * 0.002f;
		bleeps.push_back(bl);

	}

	for (int i = 0; i < MaxEnemybleeps - Enemybleeps; i++)
	{
		Bleep bl;
		bl.state = 1;
		bl.position = Entities[0]->mid + offset + Rotate(Normalize(Entities[0]->avgvel) * (9000.0f + (rand() % 2000)), ((rand() % 2000) * 0.001f - 1.0f) * pi * 0.5f);
		bl.velocity = Rotate(glm::vec2((rand() % 150), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.infinite = true;
		bl.r = 5.0f;
		bl.fogamount = 0.0f;
		bl.fraction = rand() % 10;
		bl.SaveString = "Ships/BigBoy.sav";
		if (bl.fraction > 4)
			bl.fraction = 4;
		bleeps.push_back(bl);

	}
	for (int i = 0; i < 0 - SideMissionbleeps; i++)
	{
		Bleep bl;
		bl.state = 3;
		bl.position = Entities[0]->mid + offset + Rotate(Normalize(Entities[0]->avgvel) * (9000.0f + (rand() % 2000)), ((rand() % 2000) * 0.001f - 1.0f) * pi * 0.5f);
		bl.velocity = { 0.0f,0.0f };
		bl.infinite = true;
		bl.r = 15.0f;
		bl.fogamount = 0.0f;
		bl.colisionR = 3500.0f;
		for (int aaa = 0; aaa < 5; aaa++)
			for (int a = 0; a < bleeps.size(); a++)
			{
				if (bleeps[a].state == -1 || bleeps[a].state >= 2)
				{// get 10k away
					glm::vec2 dif = bl.position - bleeps[a].position;

					if (sqrlength(dif) < (bl.colisionR + bleeps[a].colisionR)* (bl.colisionR + bleeps[a].colisionR))
					{
						glm::vec2 norm = dif / length(dif);
						glm::vec2 dd = norm * ((bl.colisionR + bleeps[a].colisionR) - length(dif));
						bl.position += dd;
					}
				}
			}
		bleeps.push_back(bl);
	}
	for (int i = 0; i < 1 - Stationbleeps; i++)
	{
		Bleep bl;
		bl.state = -1;
		bl.position = Entities[0]->mid + offset + Rotate(Normalize(Entities[0]->avgvel) * (9000.0f + (rand() % 2000)), ((rand() % 2000) * 0.001f - 1.0f) * pi * 0.5f);
		bl.velocity = { 0.0f,0.0f };
		bl.infinite = true;
		bl.r = 15.0f;
		bl.fogamount = 0.0f;
		bl.colisionR = 5500.0f;
		bl.fraction = rand() % 4;
		if (bl.fraction > 4)
			bl.fraction = 4;
		for (int aaa = 0; aaa < 5; aaa++)
			for (int a =0;a<bleeps.size();a++)
			{
				if (bleeps[a].state == -1 || bleeps[a].state >= 2)
				{// get 10k away
					glm::vec2 dif = bl.position - bleeps[a].position;
					
					if (sqrlength(dif) < (bl.colisionR + bleeps[a].colisionR) * (bl.colisionR + bleeps[a].colisionR))
					{
						glm::vec2 norm = dif / length(dif);
						glm::vec2 dd = norm * ((bl.colisionR + bleeps[a].colisionR) - length(dif));
						bl.position += dd;
					}
				}
			}
		bleeps.push_back(bl);
	}

}

void Radar::Draw(glm::vec2 screenpos)
{
	if (Entities.size() > 0)
	{
		float step = 16.5f;
		glm::vec2 corner = { 0.0f,-step };
		for (int i = 0; i < MissionTexts.size(); i++)
		{
			UI_DrawText(MissionTexts[i].c_str(), screenpos - glm::vec2(100.0f,100.0f) + corner,0.35f);
			corner.y -= step;
		}
		MissionTexts.clear();

		glm::vec2 pos = Entities[0]->mid + offset;
		UI_DrawCircle(screenpos, 100.0f, { 0.0f,0.3f,0.0f,1.0f }, 0, 0, -10);
		UI_DrawCircle(screenpos, 99.0f, { 0.0f,0.0f,0.0f,1.0f }, 0, 0, -10);

		UI_DrawCircle(screenpos, 75.0f, { 0.0f,0.3f,0.0f,1.0f }, 0, 0, -10);
		UI_DrawCircle(screenpos, 74.0f, { 0.0f,0.0f,0.0f,1.0f }, 0, 0, -10);

		UI_DrawCircle(screenpos, 50.0f, { 0.0f,0.3f,0.0f,1.0f }, 0, 0, -10);
		UI_DrawCircle(screenpos, 49.0f, { 0.0f,0.0f,0.0f,1.0f }, 0, 0, -10);

		UI_DrawCircle(screenpos, 25.0f, { 0.0f,0.3f,0.0f,1.0f }, 0, 0, -10);
		UI_DrawCircle(screenpos, 24.0f, { 0.0f,0.0f,0.0f,1.0f }, 0, 0, -10);
		UI_DrawCircle(screenpos, sqrt(playerHeat)*10.0f, { 2.0f,0.4f,0.1f,0.5f }, 0, 0, -9);

		

		glm::vec2 rod = { 100.0f,0.0f };
		float angle = 0.0f;
		UI_DrawLine(screenpos - rod, screenpos + rod, 1.0f, { 0.0f,0.2f,0.0f,1.0f });
		angle += pi * 0.33f * 0.5f;
		UI_DrawLine(screenpos - Rotate(rod, angle), screenpos + Rotate(rod, angle), 1.0f, { 0.0f,0.2f,0.0f,1.0f });
		angle += pi * 0.33f * 0.5f;
		UI_DrawLine(screenpos - Rotate(rod, angle), screenpos + Rotate(rod, angle), 1.0f, { 0.0f,0.2f,0.0f,1.0f });
		angle = pi * 0.5f;
		UI_DrawLine(screenpos - Rotate(rod, angle), screenpos + Rotate(rod, angle), 1.0f, { 0.0f,0.2f,0.0f,1.0f });
		angle += pi * 0.33f * 0.5f;
		UI_DrawLine(screenpos - Rotate(rod, angle), screenpos + Rotate(rod, angle), 1.0f, { 0.0f,0.2f,0.0f,1.0f });
		angle += pi * 0.33f * 0.5f;
		UI_DrawLine(screenpos - Rotate(rod, angle), screenpos + Rotate(rod, angle), 1.0f, { 0.0f,0.2f,0.0f,1.0f });



		for (int i = 0; i < bleeps.size(); i++)
		{
			glm::vec4 color = { 0.4f,1.5f,0.4f,1.0f };
			if (bleeps[i].state == 0)
				color = { 0.1f,0.4f,0.1f,1.0f };
			else if (bleeps[i].state != 2)
				color = { 0.2f,0.6f,0.2f,1.0f };
			else
				color = { 0.2f,0.2f,2.0f,1.0f };
			if (bleeps[i].state > 2)
				color = { 0.2f,0.6f,0.2f,1.0f };
			if (bleeps[i].state ==-1)
				color = { 2.1f,1.0f,3.1f,1.0f };


			glm::vec2 bpos = (bleeps[i].position - pos) * 0.01f;
			if (sqrlength(bpos) > 100 * 100)
			{
				if (bleeps[i].state != 2 && sqrlength(bpos) > 105 * 105)
				{
					continue;
				}
				bpos = Normalize(bpos) * 100.0f;
			}
			bpos += screenpos;
			if (bleeps[i].justheat)
			{
				color = { 5.5f,1.0f,0.6f,1.0f };
				
				DrawLight(bpos, { bleeps[i].t,bleeps[i].t }, color, 2.0f);

			}
			else
				DrawLight(bpos, { bleeps[i].r,bleeps[i].r}, color, 2.0f);
		}

	}
}



