#include "../engine/Components/Redactor.h"
#include "../engine/Components/Include/sounds.h"
#include "../ParticleMaterials.h"
#include "../Textures.h"
#include "../DamageSphere.h"
#include "../Explodion.h"
#include "../Rocket.h"
#include "smallball.h"
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
	for (int i = 0; i < body.size(); i++)
	{
		if (glm::isnan(ballPosition[body[i]]).x || glm::isnan(ballPosition[body[i]]).y)
		{
			Health = 0;
			Delete = true;
			dead = true;
			return;
		}

		if (ballTemp[body[i]] > MinAutocooltemp)
			ballTemp[body[i]] -= dt * CoolingSpeed;
		if (ballTemp[body[i]] <= MinAutocooltemp)
			shutdown = false;
		if (ballTemp[body[i]] > shutdownTemp)
			shutdown = true;
		if (ballTemp[body[i]] < MinAutocooltemp)
			ballTemp[body[i]] += dt * CoolingSpeed;
		if (ballTemp[body[i]] > MaxTemp) ballTemp[body[i]] = MaxTemp;
		if (ballTemp[body[i]] < MinTemp) ballTemp[body[i]] = MinTemp;
		avgT += ballTemp[body[i]];

		ballVelocityBuff[body[i]] = ballVelocity[body[i]];
		for (int a = 0; a < GameScene->Collision_balls.size(); a++)
		{	
			if(BalltoStaticBallCollision(body[i], GameScene->Collision_balls[a]) && GameScene->Collision_balls[a]->id == -10 && debris)
				Health = -10.0f;
		}

		for (int a = 0; a < GameScene->Collision_cubes.size(); a++)
		{
			if (BallToStaticQuadCollision(body[i], GameScene->Collision_cubes[a]) && GameScene->Collision_cubes[a]->id == -10 && debris)
				Health = -10.0f;
		}
		if (sqrlength(ballVelocityBuff[body[i]] - ballVelocity[body[i]]) > 50*50)
		{
			Health -= (length(ballVelocityBuff[body[i]] - ballVelocity[body[i]]) - 50) * 0.005f;
		}
	}
	

	avgT /= bodysize;

	float tmpdiv = 1.0f / MaxTemp;
	if (avgT > 0.0f)
		color = BaseColor + HeatColor * (avgT * tmpdiv);
	else
		color = BaseColor - ColdColor * (avgT * tmpdiv);
	if (Health < 0.0f)
	{
		dead = true;
		return;
	}
	DamageTimer -= dt;
	if (LastHealth > Health)
	{
		DamageTimer = 15.0f;
	}
	LastHealth = Health;
	if (!debris)
	{
		if (DamageTimer <= 0.0f)
		{
			Health += dt * maxHealth*0.03f;
		}
	}

	if (Health > maxHealth)
		Health = maxHealth;
	
}
void BodyComponent::Create(glm::vec2 position, glm::vec2 direction, float size, float mass) {}
void BodyComponent::DeletePart()
{
	DeleteBody();
}
BodyComponent::~BodyComponent()
{
	DeletePart();
}
void BodyComponent::DrawDataConnections(int partid, bool b, bool f, bool v,int Z_Index)
{
	float Scale = (PARTSIZE / 2);
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
				position = ballPosition[body[0]] + dir * float(p) * Scale * 2.0f + norm * Scale;
			else
				position = ballPosition[body[0]] + dir * float(p) * Scale * 2.0f - norm * Scale;
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
				position = ballPosition[body[0]] + dir * float(p) * Scale * 2.0f + norm * Scale;
			else
				position = ballPosition[body[0]] + dir * float(p) * Scale * 2.0f - norm * Scale;
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
				position = ballPosition[body[0]] + dir * float(p) * Scale * 2.0f + norm * Scale;
			else
				position = ballPosition[body[0]] + dir * float(p) * Scale * 2.0f - norm * Scale;
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
		{
			glm::vec2 dif = DS->body.position - ballPosition[body[i]];
			if ( dif.x*dif.x + dif.y*dif.y < (DS->body.r + PARTSIZE) * (DS->body.r + PARTSIZE))
			{
				if (!hit)
				{
					ballTemp[body[i]] += DS->Heat;
					Health -= DS->Damage;
					hit = true;
					if (DS->Damage >= 0.01f)
					{
						WasHitThisFrame = true;
						dmgThisFrame += DS->Damage;
					}
				}
				if (sqrlength(DS->body.position - ballPosition[body[i]]) > 0.0f)
					ballVelocity[body[i]] -= Normalize(DS->body.position - ballPosition[body[i]]) * DS->recoil;
				ballVelocity[body[i]] *= DS->friction;
				if (DS->singleHit)
					DS->dead = true;
			}
		}
	}
}
void BodyComponent::CreateBody(int size, int boolDCsize,int floatDCsize, int vectorDCsize)
{
	bDataConnections.resize(boolDCsize);
	bDCsize = boolDCsize;
	fDataConnections.resize(floatDCsize);
	fDCsize = floatDCsize;
	vDataConnections.resize(vectorDCsize);
	vDCsize = vectorDCsize;

	//Temperatures = new float[size];

	for (int i = 0; i < size; i++)
		body.push_back(NewBall());

	bodysize = size;
}
void BodyComponent::DeleteBody()
{
	
	for(int i= 0;i < body.size();i++)
		DeleteBall(body[i]);
	body.clear();
	bDataConnections.clear();
	fDataConnections.clear();
	vDataConnections.clear();
	bodysize = 0;

}


void PartsPile::Process(float dt, int iter,bool lastiter)
{
	
	for (int i = 0; i < Parts.size(); i++)
	{
		
		bool del = true;
		while (del && i < Parts.size())
		{
			del = false;
			if (Parts[i]->Delete || Parts[i]->Health <= 0.0f)
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
		if (Parts[i]->Delete || Parts[i]->Health <= 0.0f)
			continue;
		Parts[i]->debris = true;
		if (sqrlength(ballPosition[Parts[i]->body[0]] - CameraPosition) < DrawingDistance)
		{
			for (int d = 0; d < CloseDamageSpheres.size(); d++)
				Parts[i]->DamageSphereCollide(CloseDamageSpheres[d]);
			
			Parts[i]->throtle = 0.0f;
			Parts[i]->deactivated = true;
		}
	}
	CloseDamageSpheres.clear();
}
void PartsPile::DeletePart(int  index)
{
	if (index >= Parts.size() || index<0)
		return;
	glm::vec2 mid = {0.0f,0.0f};
	for (int i = 0; i < Parts[index]->bodysize; i++)
	{
		Sparks.Spawn(ballPosition[Parts[index]->body[i]], 15);
		Smoke.Spawn(ballPosition[Parts[index]->body[i]], 15);
		DebrieParticles.RotationVelocity = rand() % 100 * 0.1f - 5.0f;
		DebrieParticles.InitialVelocity = ballVelocity[Parts[index]->body[i]];
		DebrieParticles.Spawn(ballPosition[Parts[index]->body[i]], 3);
		mid += ballPosition[Parts[index]->body[i]];
	}
	mid /= Parts[index]->bodysize;
	for (int bp = 0; bp < Parts[index]->bodysize; bp++)
	{
		if (GrabbedBall != -1)
			if (ballPosition[GrabbedBall]== ballPosition[Parts[index]->body[bp]])
				GrabbedBall = -1;
		if (SelectedPart = index)
			SelectedPart = -1;
	}
	if(Parts[index]->body.size()>0)
		playsound(PartDestrSOund, mid, 0.25f, 0.085f + rand() % 100 * 0.001f, ballVelocity[Parts[index]->body[0]]);
	Parts[index]->DeleteBody();
	Parts[index]->Delete = true;
	Parts[index] = Parts[Parts.size() - 1];
	Parts.pop_back();
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


