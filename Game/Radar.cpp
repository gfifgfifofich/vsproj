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


void Radar::Spawn(glm::vec2 pos)
{

}

void Radar::Init()
{
	for (int i = 0; i < 10; i++)
	{
		Bleep bl;
		bl.state = 1;
		bl.position = Rotate(glm::vec2(1000.0f + (rand() % 4000), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.velocity = Rotate(glm::vec2((rand() % 150), 0.0f), (rand() % 1000) * 0.002f * pi);
		bl.infinite = true;
		bl.r = 5.0f;
		bleeps.push_back(bl);
	}
}

void Radar::Process(float dt)
{
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

	for (int i = 0; i < bleeps.size(); i++)
	{
		bleeps[i].t -= dt;
		bleeps[i].position += bleeps[i].velocity * dt;
		if (bleeps[i].state != 1)
			bleeps[i].velocity = { 0.0f,0.0f };

		if (Entities.size() > 0)
		{
			if (sqrt(playerHeat) * 10.0f > length(bleeps[i].position - Entities[0]->mid) * 0.01f)
			{
				bleeps[i].velocity = Normalize(Entities[0]->mid - bleeps[i].position) * 500.0f;
		
			}
		
			
		}
	}
}

void Radar::Draw(glm::vec2 screenpos)
{
	if (Entities.size() > 0)
	{
		glm::vec2 pos = Entities[0]->mid;
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
				color = { 0.2f,2.5f,0.2f,1.0f };
			if (bleeps[i].state == 1)
				color = { 2.5f,0.2f,0.2f,1.0f };
			if (bleeps[i].state == 2)
				color = { 0.2f,0.2f,2.0f,1.0f };

			DrawLight(screenpos + (bleeps[i].position - pos) * 0.01f, { bleeps[i].r,bleeps[i].r}, color, 2.0f);
		}

	}
}