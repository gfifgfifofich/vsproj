#include "../engine/Components/Redactor.h"
#include "../engine/Components/Include/sounds.h"
#include "../ParticleMaterials.h"
#include "../Textures.h"
#include "../DamageSphere.h"
#include "../Explodion.h"
#include "../Rocket.h"
#include "Base.h"

inline float TextSize;
inline float UISize;
inline bool bLogicMode;
inline bool fLogicMode;
inline bool vLogicMode;
inline int DataconnectionData[6];

void BodyComponent::OnPartCreate()
{
	GameScene->Nodes.push_back(this);
}
void BodyComponent::ProcessBody(float dt)
{
	WasHitThisFrame = false;
	dmgThisFrame = 0;
	float avgT = 0.0f;
	for (int i = 0; i < bodysize; i++)
	{
		if (body[i].temperature > MinAutocooltemp)
			body[i].temperature -= dt * CoolingSpeed;
		if (body[i].temperature <= MinAutocooltemp)
			shutdown = false;
		if (body[i].temperature > shutdownTemp)
			shutdown = true;
		if (body[i].temperature < MinAutocooltemp)
			body[i].temperature += dt * CoolingSpeed;
		if (body[i].temperature > MaxTemp) body[i].temperature = MaxTemp;
		if (body[i].temperature < MinTemp) body[i].temperature = MinTemp;
		avgT += body[i].temperature;

		body[i].velbuff = body[i].velocity;
		for (int a = 0; a < GameScene->Collision_balls.size(); a++)
		{
			BalltoStaticBallCollision(&body[i], GameScene->Collision_balls[a],0.25f);
			GameScene->Collision_balls[a]->velocity *= 0.0f;
		}
		for (int a = 0; a < GameScene->Collision_cubes.size(); a++)
			if (GameScene->Collision_cubes[a]->id == -1)
				BallToStaticQuadCollision(&body[i], *GameScene->Collision_cubes[a]);
	}
	avgT /= bodysize;
	
	float tmpdiv = 1.0f / MaxTemp;
	if (avgT > 0.0f)
		color = BaseColor + HeatColor * (avgT * tmpdiv);
	else
		color = BaseColor - ColdColor * (avgT * tmpdiv);
	if (Health < 0.0f)
		dead = true;

	
}
void BodyComponent::Create(glm::vec2 position, glm::vec2 direction, float size, float mass) {}
void BodyComponent::DeletePart(){}
BodyComponent::~BodyComponent()
{
	DeletePart();
}
void BodyComponent::DrawDataConnections(int partid, bool b, bool f, bool v,int Z_Index)
{
	float Scale = (body[0].r / 2);
	int i = 0;
	int p = 0;
	bool secondColomn = false;
	glm::vec2 norm = Rotate(dir, pi * 0.5f);
	float csx = 1.0f / CameraScale.x;
	if (b)
	{
		while (i < bDCsize)
		{
			glm::vec2 position;
			if (!secondColomn)
				position = body[0].position + dir * float(p) * Scale * 2.0f + norm * Scale;
			else
				position = body[0].position + dir * float(p) * Scale * 2.0f - norm * Scale;
			float rad = Scale;
			if (sqrlength(MousePosition - position) < rad * rad)
			{
				rad *= 1.1f;/*
				DrawText(bDataConnections[i].name, MousePosition, 0.75f * csx, glm::vec4(1.0f, 0.5f, 0.0f, 0.5f)*3.0f, true);
				glm::vec2 Textsize = getTextSize(bDataConnections[i].name)* 0.75f * csx;
				DrawCube(MousePosition + Textsize * 0.5f, Textsize * 0.8f, 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f),false,NULL,1000);*/
				UI_DrawTextOnPlate(bDataConnections[i].name, ScreenMousePosition, TextSize, glm::vec4(1.0f, 0.5f, 0.0f, 0.5f) * 3.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f), 1200, false);
				if (JustPressedLMB)
				{
					if (DataconnectionData[0] == 0 )
					{
						DataconnectionData[1] = 0;
						DataconnectionData[2] = partid;
						DataconnectionData[3] = i;
						DataconnectionData[0] = 1;
					}
					else if (DataconnectionData[0] == 1 && DataconnectionData[1] == 0)
					{
						DataconnectionData[0] = 2;
						DataconnectionData[4] = partid;
						DataconnectionData[5] = i;
					}
				}
			}
			bDataConnections[i].lastpos = position;
			DrawCircle(position, rad, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index);
			if(!bDataConnections[i].source)
				DrawCircle(position, rad*0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), true, BallNormalMapTexture, Z_Index);
			i++;
			p++;
			if (i >= bDCsize * 0.5f && !secondColomn)
			{
				secondColomn = true;
				p = 0;
			}
		}
	}
	else if (f)
	{
		while (i < fDCsize)
		{
			glm::vec2 position;
			if (!secondColomn)
				position = body[0].position + dir * float(p) * Scale * 2.0f + norm * Scale;
			else
				position = body[0].position + dir * float(p) * Scale * 2.0f - norm * Scale;
			float rad = Scale;
			if (sqrlength(MousePosition - position) < rad * rad)
			{
				rad *= 1.1f;/*
				DrawText(fDataConnections[i].name, MousePosition, 0.75f * csx, glm::vec4(1.0f, 0.5f, 0.5f, 0.5f) * 3.0f, true);
				glm::vec2 Textsize = getTextSize(fDataConnections[i].name) * 0.75f * csx;
				DrawCube(MousePosition + Textsize * 0.5f, Textsize * 0.8f, 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f), false, NULL, 1000);*/
				UI_DrawTextOnPlate(fDataConnections[i].name, ScreenMousePosition, TextSize, glm::vec4(1.0f, 0.5f, 0.5f, 0.5f) * 3.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f), 1200, false);
				if (JustPressedLMB)
				{
					if (DataconnectionData[0] == 0)
					{
						DataconnectionData[1] = 1;
						DataconnectionData[2] = partid;
						DataconnectionData[3] = i;
						DataconnectionData[0] = 1;
					}
					else if (DataconnectionData[0] == 1 && DataconnectionData[1] == 1)
					{
						DataconnectionData[0] = 2;
						DataconnectionData[4] = partid;
						DataconnectionData[5] = i;
					}
				}
			}
			fDataConnections[i].lastpos = position;
			DrawCircle(position, rad, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), true, BallNormalMapTexture, Z_Index);
			if (!fDataConnections[i].source)
				DrawCircle(position, rad * 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), true, BallNormalMapTexture, Z_Index);
			i++;
			p++;
			if (i >= fDCsize * 0.5f && !secondColomn)
			{
				secondColomn = true;
				p = 0;
			}
		}
	}
	else if (v)
	{
		while (i < vDCsize)
		{
			glm::vec2 position;
			if (!secondColomn)
				position = body[0].position + dir * float(p) * Scale * 2.0f + norm * Scale;
			else
				position = body[0].position + dir * float(p) * Scale * 2.0f - norm * Scale;
			float rad = Scale;
			if (sqrlength(MousePosition - position) < rad * rad)
			{
				rad *= 1.1f;/*
				DrawText(vDataConnections[i].name, MousePosition, 0.75f * csx, glm::vec4(0.0f, 0.5f, 1.0f, 0.5f) * 3.0f, true);
				glm::vec2 Textsize = getTextSize(vDataConnections[i].name) * 0.75f * csx;
				DrawCube(MousePosition + Textsize * 0.5f, Textsize * 0.8f, 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f), false, NULL, 1000);*/
				UI_DrawTextOnPlate(vDataConnections[i].name, ScreenMousePosition, TextSize, glm::vec4(1.0f, 0.5f, 0.5f, 0.5f) * 3.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f), 1200, false);
				if (JustPressedLMB)
				{
					if (DataconnectionData[0] == 0)
					{
						DataconnectionData[1] = 2;
						DataconnectionData[2] = partid;
						DataconnectionData[3] = i;
						DataconnectionData[0] = 1;
					}
					else if (DataconnectionData[0] == 1 && DataconnectionData[1] == 2)
					{
						DataconnectionData[0] = 2;
						DataconnectionData[4] = partid;
						DataconnectionData[5] = i;
					}
				}
			}
			vDataConnections[i].lastpos = position;
			DrawCircle(position, rad, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), true, BallNormalMapTexture, Z_Index);
			if (!vDataConnections[i].source)
				DrawCircle(position, rad * 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), true, BallNormalMapTexture, Z_Index);
			i++;
			p++;
			if (i >= vDCsize * 0.5f && !secondColomn)
			{
				secondColomn = true;
				p = 0;
			}
		}
	}
}
void BodyComponent::DamageSphereCollide(DamageSphere* DS)
{
	if (!DS->dead && (DS->id != id || DS->neutral || debris))
	{
		bool hit = false;
		for (int i = 0; i < bodysize; i++)
			if (BtBCollisionCheck(DS->body, body[i]))
			{
				if (!hit)
				{
					body[i].temperature += DS->Heat;
					Health -= DS->Damage;
					hit = true;
					WasHitThisFrame = true;
					dmgThisFrame += DS->Damage;
				}
				if (sqrlength(DS->body.position - body[i].position) > 0.0f)
					body[i].velocity -= Normalize(DS->body.position - body[i].position) * DS->recoil;
				body[i].velocity *= DS->friction;
				if (DS->singleHit)
					DS->dead = true;
			}
	}
}
void BodyComponent::CreateBody(int size, int boolDCsize,int floatDCsize, int vectorDCsize)
{
	bDataConnections = new BoolDataPoint[boolDCsize];
	bDCsize = boolDCsize;
	fDataConnections = new FloatDataPoint[floatDCsize];
	fDCsize = floatDCsize;
	vDataConnections = new VectorDataPoint[vectorDCsize];
	vDCsize = vectorDCsize;
	body = new BallBodyComponent[size];
	//Temperatures = new float[size];
	bodysize = size;
}
void BodyComponent::DeleteBody()
{
	
	delete[] body;
	if(bDCsize>0)
		delete[] bDataConnections;
	if (fDCsize > 0)
		delete[] fDataConnections;
	if (vDCsize > 0)
		delete[] vDataConnections;
}


void PartsPile::Process(float dt, int iter,bool lastiter)
{
	
	for (int i = 0; i < Parts.size(); i++)
	{
		
		bool del = true;
		while (del && i < Parts.size())
		{
			del = false;
			if (Parts[i]->Health <= 0.0f)
			{
				DeletePart(i);
				del = true;
			}
		}
	}
	DrawingDistance = (WIDTH * 1.0f) / CameraScale.x;
	if (DrawingDistance < 150)
		DrawingDistance = 150;
	DrawingDistance *= DrawingDistance;
	for (int d = 0; d < DamageSpheres.size(); d++)
	{
		if (DamageSpheres[d]->id >= 0 && sqrlength(DamageSpheres[d]->body.position - CameraPosition) < DamageSpheres[d]->body.r * DamageSpheres[d]->body.r + 150 * 150)
			CloseDamageSpheres.push_back(DamageSpheres[d]);
	}
	for (int i = 0; i < Parts.size(); i++)
	{
		Parts[i]->debris = true;
		if (sqrlength(Parts[i]->body[0].position - CameraPosition) < DrawingDistance)
		{
			for (int d = 0; d < CloseDamageSpheres.size(); d++)
				Parts[i]->DamageSphereCollide(CloseDamageSpheres[d]);
			
			Parts[i]->throtle = 0.0f;
			Parts[i]->deactivated = true;
			//if (!Parts[i]->dead)
			//{

			//	Parts[i]->Process(dt);
			//	if (lastiter)
			//		Parts[i]->Draw();
			//}
			
			//debrie killing cubes, no need for now
			
			//for (int a = 0; a < GameScene->Collision_cubes.size(); a++)
			//	if (GameScene->Collision_cubes[a]->id == 1)
			//		for (int bp = 0; bp < Parts[i]->bodysize; bp++)
			//		{
			//			if (BtCCollisionCheck(Parts[i]->body[bp], *GameScene->Collision_cubes[a]))
			//			{
			//				Parts[i]->Health = -1.0f;
			//			}
			//		}
			//for (int a = 0; a < GameScene->Collision_balls.size(); a++)
			//	if (GameScene->Collision_balls[a]->id == 1)
			//		for (int bp = 0; bp < Parts[i]->bodysize; bp++)
			//		{
			//			if (BtBCollisionCheck(Parts[i]->body[bp], *GameScene->Collision_balls[a]))
			//			{
			//				Parts[i]->Health = -1.0f;
			//			}
			//		}
		}
	}
	CloseDamageSpheres.clear();
}
void PartsPile::DeletePart(int  index)
{
	glm::vec2 mid = {0.0f,0.0f};
	for (int i = 0; i < Parts[index]->bodysize; i++)
	{
		Sparks.Spawn(Parts[index]->body[i].position, 15);
		Smoke.Spawn(Parts[index]->body[i].position, 15);
		DebrieParticles.RotationVelocity = rand() % 100 * 0.1f - 5.0f;
		DebrieParticles.InitialVelocity = Parts[index]->body[i].velocity;
		DebrieParticles.Spawn(Parts[index]->body[i].position, 3);
		mid += Parts[index]->body[i].position;
	}
	mid /= Parts[index]->bodysize;
	for (int bp = 0; bp < Parts[index]->bodysize; bp++)
	{
		if (GrabbedBall != nullptr)
			if (GrabbedBall->position == Parts[index]->body[bp].position)
				GrabbedBall = nullptr;
		if (SelectedPart = index)
			SelectedPart = -1;
	}
	Parts[index]->Delete=true;
	Parts[index] = Parts[Parts.size() - 1];
	Parts.pop_back();
	playsound(PartDestrSOund, mid, 0.25f,0.085f+rand()%100*0.001f,{0.0f,0.0f},false);
}
void PartsPile::SpawnPart(int type, glm::vec2 position, float size)
{
		
	BodyComponent* b = CreatePart(type, position, { 0.0f,1.0f }, size);

	
	if(b!=nullptr)
		Parts.push_back(b);
	
}
void PartsPile::SpawnRandomPart( glm::vec2 position, float size)
{
	int type = rand() % 7;
	SpawnPart(type, position, size);
}


