#include "../engine/Components/Redactor.h"
#include "../engine/Components/Include/sounds.h"
#include "../ParticleMaterials.h"
#include "../Textures.h"
#include "../DamageSphere.h"
#include "../Explodion.h"
#include "../Rocket.h"
#include "Base.h"
#include "CentralPart.h"

#define PARTSIZE 0.5f
inline float Speed;
inline bool absoluteControl;
inline bool BuildingMode;
inline float TextSize;
inline float UISize;
inline bool bLogicMode;
inline bool fLogicMode;
inline bool vLogicMode;
inline int DataconnectionData[6];
inline glm::vec2 foregroundMousePosition;

void CentralPart::DataConnection::Process(CentralPart* ent)
{
	if(ent!=NULL)
	if (part1 < ent->Parts.size() && part2 < ent->Parts.size() &&
		!ent->Parts[part1]->debris && !ent->Parts[part2]->debris)
	{
		failed = false;
		if (type == 0 && index1 < ent->Parts[part1]->bDCsize && index2 < ent->Parts[part2]->bDCsize)
		{
			if (ent->Parts[part1]->bDataConnections[index1].source)
				ent->Parts[part2]->bDataConnections[index2].data = ent->Parts[part1]->bDataConnections[index1].data;
			else
				ent->Parts[part1]->bDataConnections[index1].data = ent->Parts[part2]->bDataConnections[index2].data;
			ent->Parts[part1]->bDataConnections[index1].connected = true;
			ent->Parts[part2]->bDataConnections[index2].connected = true;
			if (bLogicMode)
			{
				float Scale = (ent->Parts[part1]->body[0].r * 0.5f);
				glm::vec2 norm = Rotate(ent->Parts[part1]->dir, pi * 0.5f);
				if (index1 < ent->Parts[part1]->bDCsize * 0.5f)
					position1 = ent->Parts[part1]->body[0].position + ent->Parts[part1]->dir * float(index1) * Scale * 2.0f + norm * Scale;
				else
					position1 = ent->Parts[part1]->body[0].position + ent->Parts[part1]->dir * float(index1 - int(ent->Parts[part1]->bDCsize * 0.5f + 0.5f)) * Scale * 2.0f - norm * Scale;
				float Scale2 = (ent->Parts[part2]->body[0].r * 0.5f);
				glm::vec2 norm2 = Rotate(ent->Parts[part2]->dir, pi * 0.5f);
				if (index2 < ent->Parts[part2]->bDCsize * 0.5f)
					position2 = ent->Parts[part2]->body[0].position + ent->Parts[part2]->dir * float(index2) * Scale2 * 2.0f + norm2 * Scale2;
				else
					position2 = ent->Parts[part2]->body[0].position + ent->Parts[part2]->dir * float(index2 - int(ent->Parts[part2]->bDCsize * 0.5f + 0.5f)) * Scale2 * 2.0f - norm2 * Scale2;
			}
		}
		else if (type == 1 && index1 < ent->Parts[part1]->fDCsize && index2 < ent->Parts[part2]->fDCsize)
		{
			if (ent->Parts[part1]->fDataConnections[index1].source)
				ent->Parts[part2]->fDataConnections[index2].data = ent->Parts[part1]->fDataConnections[index1].data;
			else
				ent->Parts[part1]->fDataConnections[index1].data = ent->Parts[part2]->fDataConnections[index2].data;
			ent->Parts[part1]->fDataConnections[index1].connected = true;
			ent->Parts[part2]->fDataConnections[index2].connected = true;
			if (fLogicMode)
			{
				float Scale = (ent->Parts[part1]->body[0].r * 0.5f);
				glm::vec2 norm = Rotate(ent->Parts[part1]->dir, pi * 0.5f);
				if (index1 < ent->Parts[part1]->fDCsize * 0.5f)
					position1 = ent->Parts[part1]->body[0].position + ent->Parts[part1]->dir * float(index1) * Scale * 2.0f + norm * Scale;
				else
					position1 = ent->Parts[part1]->body[0].position + ent->Parts[part1]->dir * float(index1 - int(ent->Parts[part1]->fDCsize * 0.5f + 0.5f)) * Scale * 2.0f - norm * Scale;
				float Scale2 = (ent->Parts[part2]->body[0].r * 0.5f);
				glm::vec2 norm2 = Rotate(ent->Parts[part2]->dir, pi * 0.5f);
				if (index2 < ent->Parts[part2]->fDCsize * 0.5f)
					position2 = ent->Parts[part2]->body[0].position + ent->Parts[part2]->dir * float(index2) * Scale2 * 2.0f + norm2 * Scale2;
				else
					position2 = ent->Parts[part2]->body[0].position + ent->Parts[part2]->dir * float(index2 - int(ent->Parts[part2]->fDCsize * 0.5f + 0.5f)) * Scale2 * 2.0f - norm2 * Scale2;
			}
		}
		else if (type == 2 && index1 < ent->Parts[part1]->vDCsize && index2 < ent->Parts[part2]->vDCsize)
		{
			if (ent->Parts[part1]->vDataConnections[index1].source)
				ent->Parts[part2]->vDataConnections[index2].data = ent->Parts[part1]->vDataConnections[index1].data;
			else
				ent->Parts[part1]->vDataConnections[index1].data = ent->Parts[part2]->vDataConnections[index2].data;
			ent->Parts[part1]->vDataConnections[index1].connected = true;
			ent->Parts[part2]->vDataConnections[index2].connected = true;
			if (vLogicMode)
			{
				float Scale = (ent->Parts[part1]->body[0].r * 0.5f);
				glm::vec2 norm = Rotate(ent->Parts[part1]->dir, pi * 0.5f);
				if (index1 < ent->Parts[part1]->vDCsize * 0.5f)
					position1 = ent->Parts[part1]->body[0].position + ent->Parts[part1]->dir * float(index1) * Scale * 2.0f + norm * Scale;
				else
					position1 = ent->Parts[part1]->body[0].position + ent->Parts[part1]->dir * float(index1 - int(ent->Parts[part1]->vDCsize * 0.5f + 0.5f)) * Scale * 2.0f - norm * Scale;
				float Scale2 = (ent->Parts[part2]->body[0].r * 0.5f);
				glm::vec2 norm2 = Rotate(ent->Parts[part2]->dir, pi * 0.5f);
				if (index2 < ent->Parts[part2]->vDCsize * 0.5f)
					position2 = ent->Parts[part2]->body[0].position + ent->Parts[part2]->dir * float(index2) * Scale2 * 2.0f + norm2 * Scale2;
				else
					position2 = ent->Parts[part2]->body[0].position + ent->Parts[part2]->dir * float(index2 - int(ent->Parts[part2]->vDCsize * 0.5f + 0.5f)) * Scale2 * 2.0f - norm2 * Scale2;
			}
		}
		else
			failed = true;
	}
	else
		failed = true;
}
void CentralPart::DataConnection::Draw()
{
	if (type == 0)
	{
		if (bLogicMode)
		{
			DrawLine(position1, position2,
				0.0625f, glm::vec4(0.0f, 0.0f, 1.5f, 1.0f), false, 0, 1000);
		}
	}
	else if (type == 1)
	{
		if (fLogicMode)
		{
			DrawLine(position1, position2,
				0.0625f, glm::vec4(0.0f, 1.5f, 0.0f, 1.0f), false, 0, 1000);
		}
	}
	else if (type == 2)
	{
		if (vLogicMode)
		{
			DrawLine(position1, position2,
				0.0625f, glm::vec4(1.5f, 0.0f, 0.0f, 1.0f), false, 0, 1000);
		}
	}
}

void CentralPart::Connection::UpdateLinks(CentralPart* ent)
{
	bod1 = ent->Parts[part1];
	b1 = &ent->Parts[part1]->body[index1];
	bod2 = ent->Parts[part2];
	b2 = &ent->Parts[part2]->body[index2];
	if (bod1->Health <= 0.0f)
	{
		bod1 = NULL;
		b1 = NULL;
	}
	if (bod2->Health <= 0.0f)
	{
		bod2 = NULL;
		b2 = NULL;
	}
}
void CentralPart::Connection::Init()
{
	//rope.Init(b1, b2, length);
}
void CentralPart::Connection::Process(CentralPart* ent, float dt)
{
	UpdateLinks(ent);
	if (b1 != nullptr && b2 != nullptr)
	{
		if (type == CONNECTION::STRUT)
			Strut(b1, b2, length);
		if (type == CONNECTION::SPRING)
			SpringBetweenBalls(b1, b2, length, stiffnes, absorbtion);
		if (type == CONNECTION::HEATPIPE || type == CONNECTION::ROPE)
			Rope(b1, b2, length);
		
	}
	if (b1 != nullptr && b2 != nullptr)
	{
		float change = (b1->temperature - b2->temperature) * dt * HeatTransferSpeed;
		b1->temperature -= change;
		b2->temperature += change;
	}
}
void CentralPart::Connection::Draw(float dt)
{
	glm::vec4 color = (b1->color + b2->color) * 0.5f;
	if (type == CONNECTION::STRUT)
		if (Texture == NULL)
			DrawLine(b1->position, b2->position, width, color, true, NormalMap, -10);
		else
			DrawTexturedLine(Texture, b1->position, b2->position, width, color, NormalMap, -10);
	if (type == CONNECTION::SPRING)
		if (Texture == NULL)
			DrawLine(b1->position, b2->position, width, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), true, true, -10);
		else
			DrawTexturedLine(Texture, b1->position, b2->position, width, color, NormalMap, -10);
	if (type == CONNECTION::HEATPIPE)
	{
		Texture = HeatPipeTexture;
		NormalMap = HeatPipeNormalMap;
	}
	if (type == CONNECTION::ROPE)
	{
		Texture = RopeTexture;
		NormalMap = RopeNormalMap;
	}
	if (type == CONNECTION::STRUT)
	{
		Texture = StrutTexture;
		NormalMap = StrutNormalMap;
	}
	if (type == CONNECTION::HEATPIPE || type == CONNECTION::ROPE)
	{
		rope.b1 = b1;
		rope.b2 = b2;
		rope.length = length;
		rope.width = width;
		rope.Process(dt);
		rope.Texture = Texture;
		rope.NormalMap = NormalMap;
		rope.color = color;
		
		if(frame>10)
			rope.Draw(-9);
		else
			frame++;
	}
}
	


CentralPart::CentralPart()
{
	
	partid = PART::CENTRALPART;
	parttype = TYPE::STRUCTUREPART;
	type = partid + NodeType::LASTNODE;
	Name = "CentralPart";
	Health = PartsData.GetPropertyAsFloat("CentralPart", "Health");
	CreateBody(2,9,0,3);
	BodyIdsWithCollision.push_back(0);
	BodyIdsWithCollision.push_back(1);
	body[0].position = position + Normalize({0.0f,1.0f}) * PARTSIZE;
	body[1].position = position - Normalize({0.0f,1.0f}) * PARTSIZE;
	body[0].r = PARTSIZE;
	body[1].r = PARTSIZE;
	body[1].mass = 1.0f;
	ProcessConnections();
	OnPartCreate();
	Cost.Matter = 0;
}
void CentralPart::Create(glm::vec2 position, glm::vec2 direction, float size,float mass) 
{
	
	Health = PartsData.GetPropertyAsFloat("CentralPart", "Health");
	body[0].position = position + Normalize(direction) * size;
	body[1].position = position - Normalize(direction) * size;
	body[0].r = size;
	body[1].r = size;
	body[1].mass = mass;
	ProcessConnections();
	Parts.push_back(this);
	
}
void CentralPart::ProcessConnections()
{
	bDataConnections[0].name = "LMB";
	bDataConnections[1].name = "RMB";
	bDataConnections[2].name = "MMB";
	bDataConnections[3].name = "Shift";
	bDataConnections[4].name = "Space";
	bDataConnections[5].name = "W";
	bDataConnections[6].name = "A";
	bDataConnections[7].name = "S";
	bDataConnections[8].name = "D";
	vDataConnections[0].name = "MousePosition";
	vDataConnections[1].name = "Forward";
	vDataConnections[2].name = "Back";
	bDataConnections[0].source = true;
	bDataConnections[1].source = true;
	bDataConnections[2].source = true;
	bDataConnections[3].source = true;
	bDataConnections[4].source = true;
	bDataConnections[5].source = true;
	bDataConnections[6].source = true;
	bDataConnections[7].source = true;
	bDataConnections[8].source = true;
	vDataConnections[0].source = true;
	vDataConnections[1].source = true;
	vDataConnections[2].source = true;
}
void CentralPart::MTProcess (float dt) 
{
	if(Health<=0.0f || destroyed || Delete || !Alive)
		return;
	ProcessConnections();
	ProcessBody(dt);
	body[0].color = color;
	body[1].color = color;
	mid = 0.5f * (body[0].position + body[1].position);
	midvel = 0.5f * (body[0].velocity + body[1].velocity);
	dir = (body[1].position - body[0].position);
	body[0].Process(dt);
	body[1].Process(dt);
	body[1].Force = glm::vec2(0.0f);
	body[0].Force = glm::vec2(0.0f);
	Strut(&body[0], &body[1], body[0].r * 2.0f);
	if (player && !deactivated)
	{
		bDataConnections[0].data = buttons[GLFW_MOUSE_BUTTON_1];
		bDataConnections[1].data = buttons[GLFW_MOUSE_BUTTON_2];
		bDataConnections[2].data = buttons[GLFW_MOUSE_BUTTON_3];
		bDataConnections[3].data = keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT];
		bDataConnections[4].data = keys[GLFW_KEY_SPACE];
		bDataConnections[5].data = keys[GLFW_KEY_W];
		bDataConnections[6].data = keys[GLFW_KEY_A];
		bDataConnections[7].data = keys[GLFW_KEY_S];
		bDataConnections[8].data = keys[GLFW_KEY_D];
		vDataConnections[0].data = foregroundMousePosition;
		
	}
	vDataConnections[1].data = -dir;
	vDataConnections[2].data = dir;
	if (BuildingMode || bLogicMode || fLogicMode || vLogicMode)
	{
		bDataConnections[0].data = false;
		bDataConnections[1].data = false;
		bDataConnections[2].data = false;
		bDataConnections[3].data = false;
		bDataConnections[4].data = false;
		bDataConnections[5].data = false;
		bDataConnections[6].data = false;
		bDataConnections[7].data = false;
		bDataConnections[8].data = false;
		vDataConnections[0].data = mid;
	}
	Engines.clear();
	Balls.clear();
	maxR = 0.0f;
	if (!destroyed)
	{
		front = &body[0];
		back = &body[1];
		
		int deliter = 0;
		while (deliter<Parts.size())
		{	
			if(Parts[deliter]->Delete)
			{
				Parts[deliter] = Parts[Parts.size()-1];
				Parts.pop_back();
			}
			else
				deliter++;
		}
		
		for (int i = 0; i < Parts.size(); i++)
		{
			float len = sqrlength(Parts[i]->body[0].position - CenterOfMass);
			if (len > maxR)
				maxR = sqrt(len);
		}
		maxR += 2.0f;
		for (int d = 0; d < DamageSpheres.size(); d++)
		{
			if (DamageSpheres[d]->id == id)
				continue;
			if (sqrlength(DamageSpheres[d]->body.position - CenterOfMass) < DamageSpheres[d]->body.r * DamageSpheres[d]->body.r + maxR * maxR)
				CloseDamageSpheres.push_back(DamageSpheres[d]);
		}
		mass = 0.0f;
		avgvel = glm::vec2(0.0f);
		CenterOfMass = glm::vec2(0.0f);
		int coniter = 0;
		while (coniter < Connections.size())
		{
			if (Connections[coniter].length > strutMaxLength || Connections[coniter].length < strutMinLength)
			{
				Connections[coniter] = Connections[Connections.size() - 1];
				Connections.pop_back();
			}
			else
			{
				Connections[coniter].Process(this, dt);
				coniter++;
			}
		}
		for (int i = 0; i < DataConnections.size(); i++)
			DataConnections[i].Process(this);
		for (int i = 0; i < DataConnections.size(); i++)
			while (i < DataConnections.size() && DataConnections[i].failed)
			{
				DataConnections[i] = DataConnections[DataConnections.size() - 1];
				DataConnections.pop_back();
			}
		for (int i = 0; i < Parts.size(); i++)
		{
			Parts[i]->id = id;
			Parts[i]->player = player;
			for (int d = 0; d < CloseDamageSpheres.size(); d++)
				Parts[i]->DamageSphereCollide(CloseDamageSpheres[d]);
			if (Parts[i]->WasHitThisFrame)
			{// got hit
				if (Parts[i]->dmgThisFrame > 0)//got gamaged
					DeleteSomeConnections(i, (Parts[i]->dmgThisFrame / Parts[i]->maxHealth) * 0.01f);
			}
			
			Balls.reserve(Parts[i]->BodyIdsWithCollision.size());
			for (int bp = 0; bp < Parts[i]->BodyIdsWithCollision.size(); bp++)
			{				
				int pid = Parts[i]->BodyIdsWithCollision[bp];
				mass += Parts[i]->body[pid].mass;
				avgvel += Parts[i]->body[bp].velocity * Parts[i]->body[pid].mass;
				CenterOfMass += Parts[i]->body[bp].position * Parts[i]->body[pid].mass;
				Parts[i]->body[pid].id = id;
				Balls.push_back(&Parts[i]->body[pid]);
			}
			//if (!Parts[i]->dead||i==0)
			//{
			//	Parts[i]->Process(dt);
			//}
			Parts[i]->freq = freq;
			Parts[i]->deactivated = false;
			Parts[i]->targetrotpoint = GunsTargetrotPoint;
			if (Parts[i]->parttype == TYPE::PROPULSION)
				Engines.push_back(Parts[i]);
		}
		CloseDamageSpheres.clear();
		avgvel /= mass;
		CenterOfMass /= mass;
		if(dt<0.0001f)
			dt = 0.0001f;
		rotationvelocity = length(((front->position - prevfrontpos) - (back->position - prevbackpos)) * 0.5f) /dt; // roughly should do the trick
		
		prevfrontpos = front->position;
		prevbackpos = back->position;
		if (back != nullptr && front != nullptr)
		{
			float len = length((front->position - back->position));
			if(len<0.001f)
			{
				front->position.y+=0.001f;
				len = length((front->position - back->position));
			}
			direction = (front->position - back->position)/len;
			float aligment = DOT(Normalize(direction), Normalize(glm::vec2(-LookAt.y, LookAt.x)));
			glm::vec2 dirTotrg = Normalize((trgPos - (body[0].position + body[1].position) * 0.5f));
			
			
			for (int i = 0; i < Engines.size(); i++)
			{
				Engines[i]->throtle = DOT(-Engines[i]->dir, ThrustDirection);
				float dotTotrg = DOT(-Engines[i]->dir, dirTotrg);
				if (dotTotrg > 0.0f)
					ForceToThrustDirection += dotTotrg * Engines[i]->Power;
				Engines[i]->throtle += DOT(-Engines[i]->dir, Normalize(Rotate(CenterOfMass - Engines[i]->body[1].position, pi * 0.5))) * aligment;
				if (Engines[i]->throtle < 0.0f)	Engines[i]->throtle = 0.0f;
				if (Engines[i]->throtle > 1.0f)	Engines[i]->throtle = 1.0f;
			}
		}
		float distance = length(trgPos - (body[0].position + body[1].position) * 0.5f) + 0.001f;
		float entVelocity = length(avgvel) + 0.001f;
		maxVelocity = distance * ForceToThrustDirection * 0.01f / (mass)+0.001f;
		float friction = 0.0f;
		if (autocontrol && !player)
		{
			if(Parts.size()<5)
				Health-=0.2f;
			if(AIState == 1)
			{
				GunsTargetrotPoint = Playerpos;
				trgPos = Playerpos + Normalize(mid - Playerpos) * (maxR + 30.0f);
			}
			glm::vec2 dir = ((trgPos - (body[0].position + body[1].position) * 0.5f) / distance);
			if (distance < 2)
			{
				body[0].Force = dir * 10.0f;
				body[1].Force = dir * 10.0f;
				friction = 1.0;
				LookAt = LookAtOnTarget;
			}
			else {
				if (distance > 300)
					LookAt = dir;
				else
					LookAt = LookAtOnTarget;
				glm::vec2 Force = { 0.0f,0.0f };
				float sizemult = 0.1f;
				Force.x = sigmoid(((CenterOfMass.x - trgPos.x) * sizemult * -0.496329 + avgvel.x * sizemult * -0.0841397)) * 2.0f - 1.0f;
				Force.y = sigmoid(((CenterOfMass.y - trgPos.y) * sizemult * -0.496329 + avgvel.y * sizemult * -0.0841397)) * 2.0f - 1.0f;
				glm::vec2 rotatedvec = Rotate((mid - trgPos), pi * 0.5f);
				float len = length(rotatedvec);
				if(len >= 0.001f)
				{
					glm::vec2 norm = rotatedvec/len;
					Force -= DOT(norm, body[0].velocity) * norm * 4.0f;
					//float forceNeeded = length(Force);// (maxVelocity - entVelocity)* mass / (ForceToThrustDirection);
					ThrustDirection = Force;
					/*if (DOT(ThrustDirection, dir) < 0.0f && DOT(avgvel, dir) < 0.0f)
						ThrustDirection += dir * 0.5f;*/
					//ThrustDirection -= glm::vec2(-dir.y, dir.x) * DOT(glm::vec2(-dir.y, dir.x), avgvel);
					if (distance < 10)
						friction = 1.0;
				}
			}
			for (int i = 0; i < bDCsize; i++)
			{
				if(AIState == 1)
				{
					if (bDataConnections[i].name == "LMB") bDataConnections[i].data = distance<200;
					if (bDataConnections[i].name == "RMB") bDataConnections[i].data = distance < 200;
					if (bDataConnections[i].name == "MMB") bDataConnections[i].data = distance < 200;
					if (bDataConnections[i].name == "Shift") bDataConnections[i].data = distance > 200; // MB implement a dodge for bots
					if (bDataConnections[i].name == "Space") bDataConnections[i].data = false; 
					if (bDataConnections[i].name == "W") bDataConnections[i].data = ThrustDirection.y > 0.0f;
					if (bDataConnections[i].name == "A") bDataConnections[i].data = ThrustDirection.x < 0.0f;
					if (bDataConnections[i].name == "S") bDataConnections[i].data = ThrustDirection.y < 0.0f;
					if (bDataConnections[i].name == "D") bDataConnections[i].data = ThrustDirection.x > 0.0f;
				}
			}
			if(AIState == 1)
				if (vDataConnections[0].name == "MousePosition") vDataConnections[0].data = GunsTargetrotPoint;
		}
		ForceToThrustDirection = 0.0f;
		sumheat = 0.0f;
		for (int i = 0; i < Balls.size(); i++)
		{
			float len = sqrlength(CenterOfMass - Balls[i]->position);
			if(len<0.001f)
				continue;
			glm::vec2 dif = (CenterOfMass - Balls[i]->position)/len;
			Balls[i]->velocity -= DOT(Balls[i]->velocity - avgvel, glm::vec2(-dif.y, dif.x)) * glm::vec2(-dif.y, dif.x) * RotationalFriction * dt;
			Balls[i]->velocity -= Balls[i]->velocity * friction * dt;
			sumheat += Balls[i]->temperature;
			Balls[i]->velbuff = Balls[i]->velocity;

			for(int a = i+1;a<Balls.size();a++)
				BtBCollision(Balls[i],Balls[a]);
		}
		avgheat = sumheat / Balls.size();
		
	}
	
}
void CentralPart::Process(float dt) 
{
	
	if(Health<=0.0f || destroyed || Delete || !Alive)
		return;
	
	if (player && !deactivated)
	{
		bDataConnections[0].data = buttons[GLFW_MOUSE_BUTTON_1];
		bDataConnections[1].data = buttons[GLFW_MOUSE_BUTTON_2];
		bDataConnections[2].data = buttons[GLFW_MOUSE_BUTTON_3];
		bDataConnections[3].data = keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT];
		bDataConnections[4].data = keys[GLFW_KEY_SPACE];
		bDataConnections[5].data = keys[GLFW_KEY_W];
		bDataConnections[6].data = keys[GLFW_KEY_A];
		bDataConnections[7].data = keys[GLFW_KEY_S];
		bDataConnections[8].data = keys[GLFW_KEY_D];
		vDataConnections[0].data = foregroundMousePosition;
		
	}
	vDataConnections[1].data = -dir;
	vDataConnections[2].data = dir;
	if (BuildingMode || bLogicMode || fLogicMode || vLogicMode)
	{
		bDataConnections[0].data = false;
		bDataConnections[1].data = false;
		bDataConnections[2].data = false;
		bDataConnections[3].data = false;
		bDataConnections[4].data = false;
		bDataConnections[5].data = false;
		bDataConnections[6].data = false;
		bDataConnections[7].data = false;
		bDataConnections[8].data = false;
		vDataConnections[0].data = mid;
	}
	CheckPartsConnections();
	int pt = 0;
	while (pt < Parts.size())
	{
		if (pt < Parts.size() && Parts[pt]->Health <= 0.0f)
		{
			DestroyPart(pt);
		}
		else
			pt++;
	}
}
void CentralPart::Draw() 
{
	
	if(Health<=0.0f || destroyed || Delete || !Alive)
		return;
	if(firstdrawafterload)
	{
		firstdrawafterload = false;
		return;
	}
	glm::vec2 dif = body[0].position - body[1].position;
	DrawLine(body[0].position + dif*0.5f, body[1].position - dif * 0.5f, body[0].r, color, true, CubeNormalMapTexture, Z_Index);
	
	DrawTexturedQuad(mid, glm::vec2(1.0f * body[1].r, 2.0f * body[1].r), CentralPartTexture, get_angle_between_points(mid, body[0].position), color, Z_Index, CentralPartNormalMap);
	
	
	for (int i = 0; i < Connections.size(); i++)
		Connections[i].Draw(delta*Speed);
	for (int i = 0; i < DataConnections.size(); i++)
		DataConnections[i].Draw();
}

void CentralPart::DeletePart() 
{
	Delete = true;
	DeleteBody();
}
void CentralPart::DeleteConnections( int index, int index2)
{
	for (int i = 0; i <Connections.size(); i++)
	{
		bool del = true;
		while (del)
		{
			del = false;
			if (i < Connections.size())
			{
				if ((Connections[i].part1 == index) ||
					(Connections[i].part2 == index))
				{
					del = true;
					Connections[i] = Connections[Connections.size() - 1];
					Connections.pop_back();
				}
				else if (Connections[i].part1 == index2)
					Connections[i].part1 = index;
				else if (Connections[i].part2 == index2)
					Connections[i].part2 = index;
			}
		}
	}
	for (int i = 0; i < DataConnections.size(); i++)
	{
		bool del = true;
		while (del)
		{
			del = false;
			if (i < DataConnections.size())
			{
				if ((DataConnections[i].part1 == index) ||
					(DataConnections[i].part2 == index))
				{
					del = true;
					DataConnections[i] = DataConnections[DataConnections.size() - 1];
					DataConnections.pop_back();
				}
				else if (DataConnections[i].part1 == index2)
					DataConnections[i].part1 = index;
				else if (DataConnections[i].part2 == index2)
					DataConnections[i].part2 = index;
			}
		}
	}
	
}
void CentralPart::DeleteSomeConnections( int Partindex, float chance)
{
	for (int i = 0; i < Connections.size(); i++)
	{
		bool del = true;
		while (del)
		{
			del = false;
			
			if (i < Connections.size())
			{
				if ((Connections[i].part1 == Partindex) ||
					(Connections[i].part2 == Partindex))
				{
					if (rand() % 10000 * 0.0001f < chance)
					{
						del = true;
						Connections[i] = Connections[Connections.size() - 1];
						Connections.pop_back();
					}
				}
			}
		}
	}
}
void CentralPart::DestroyPart(int  index)
{
	glm::vec2 mid = { 0.0f,0.0f };
/*	for (int i = 0; i < Parts[index]->bodysize; i++)
	{
		Sparks.Spawn(Parts[index]->body[i].position, 15);
		Smoke.Spawn(Parts[index]->body[i].position, 15);
		DebrieParticles.RotationVelocity = rand() % 100 * 0.1f - 5.0f;
		DebrieParticles.Spawn(Parts[i]->body[i].position, 3);
		mid += Parts[index]->body[i].position;
	}*/
	mid /= Parts[index]->bodysize;
	for (int bp = 0; bp < Parts[index]->bodysize; bp++)
	{
		if (GrabbedBall != NULL)
			if (GrabbedBall->position == Parts[index]->body[bp].position)
				GrabbedBall = NULL;
		if (SelectedPart = index)
			SelectedPart = -1;
	}
	Parts[index]->Health = -10;
	DetachPart(index);
	playsound(PartDestrSOund, mid, 0.25f,0.085f + rand() % 100 * 0.001f,{0.0f,0.0f},false);
}
void CentralPart::DetachPart( int  index)
{
	Parts[index]->shutdown = false;
	Parts[index]->shot = false;
	for (int i = 0; i < Parts[index]->bDCsize; i++)
		Parts[index]->bDataConnections[i].data = false;
	for (int i = 0; i < Parts[index]->fDCsize; i++)
		Parts[index]->fDataConnections[i].data = 0;
	for (int i = 0; i < Parts[index]->vDCsize; i++)
		Parts[index]->vDataConnections[i].data = {0.0f,0.0f};
	if(!Parts[index]->Delete)
		Debris.Parts.push_back(Parts[index]);
	//Parts[index]->attached = false;
	int index2 = Parts.size() - 1;
	Parts[index] = Parts[Parts.size() - 1];
	Parts.pop_back();
	DeleteConnections( index, index2);
}
void CentralPart::DetachDetachedParts()
{
	for (int i = 0; i < Parts.size(); i++)
	{
		bool del = true;
		while (i < Parts.size() && del)
		{
			del = false;
			if (!Parts[i]->attached)
			{
				DetachPart(i);
				del = true;
			}
		}
	}
}
bool CentralPart::CheckPartConnections(Connection* c)
{
	bool changed = false;
	if(c->bod1 != NULL && c->bod2 != NULL)
	if (
		c->bod1->attached ||
		c->bod2->attached
		)
	{
		if (!c->bod1->attached || !c->bod2->attached)
			changed = true;
		c->bod1->attached = true;
		c->bod2->attached = true;
	}
	return changed;
}
void CentralPart::CheckPartsConnections()
{
	for (int i = 0; i < Parts.size(); i++)
		Parts[i]->attached = false;
	bool End = false;
	Parts[0]->attached = true;// central part
	while (!End)
	{
		End = true;
		for (int i = 0; i < Connections.size(); i++)
			if (CheckPartConnections(&Connections[i]))
				End = false;
	
	}
	DetachDetachedParts();
}
int CentralPart::ClaimPart(int index)
{
	Parts.push_back(Debris.Parts[index]);
	Parts[Parts.size() - 1]->debris = false;
	Debris.Parts[index] = Debris.Parts[Debris.Parts.size() - 1];
	Debris.Parts.pop_back();
	return Parts.size() - 1;
}
void CentralPart::AddConnection(int type, float len, float width, float stiffness, float absorbtion, float HeatTransferSpeed,
	int part1, int  index1,
	int part2, int  index2)
{
	if(part1>=Parts.size() || part2 >= Parts.size())
		return;
	int it = -1;
	for (int i = 0; i < Connections.size(); i++)
	{
		if (
			((Connections[i].part1 == part1 && Connections[i].index1 == index1 || Connections[i].part1 == part2 && Connections[i].index1 == index2) ) &&
			((Connections[i].part2 == part2 && Connections[i].index2 == index2 || Connections[i].part2 == part1 && Connections[i].index2 == index1) )
			)
			it = i;//cut connection [i]
	}
	if (it < 0)
	{
		Connection c;
		c.absorbtion = absorbtion;
		c.stiffnes = stiffness;
		c.width = width;
		c.part1 = part1;
		c.part2 = part2;
		c.index1 = index1;
		c.index2 = index2;
		c.HeatTransferSpeed = HeatTransferSpeed;
		c.type = type;
		Connections.push_back(c);
		int i = Connections.size() - 1;
		Connections[i].Init();
		if (len < 0)
		{
			Connections[i].UpdateLinks(this);
			Connections[i].length = length(Connections[i].b1->position - Connections[i].b2->position);
		}
		else
			Connections[i].length = len;
	}
	else if (it >= 0)
	{
		Connections[it] = Connections[Connections.size() - 1];
		Connections.pop_back();
		PlaySound(&Detach, MousePosition, 3.5f, 0.3f);
		CheckPartsConnections();
	}
	
}
void CentralPart::AddDataConnection(int type, int part1, int  index1, int part2, int  index2)
{
	if(part1>=Parts.size() || part2 >= Parts.size())
		return;
	if (part1 != part2)
	{
		int it = -1;
		int index1amount = 0;
		int index2amount = 0;
		int lasti1 = -1;
		int lasti2 = -1;
		for (int i = 0; i < DataConnections.size(); i++)
		{
			if (DataConnections[i].type == type &&
				(DataConnections[i].part1 == part1 || DataConnections[i].part1 == part2) &&
				DataConnections[i].index1 == index1 &&
				(DataConnections[i].part2 == part2 || DataConnections[i].part2 == part1) &&
				DataConnections[i].index2 == index2)
				it = i;//cut connection [i]
			if (DataConnections[i].type == type &&
				((DataConnections[i].part1 == part1 && DataConnections[i].index1 == index1) ||
				(DataConnections[i].part2 == part1 && DataConnections[i].index2 == index1)))
			{
				index1amount++;
				lasti1 = i;
			}
			if (DataConnections[i].type == type &&
				((DataConnections[i].part1 == part2 && DataConnections[i].index1 == index2) ||
				(DataConnections[i].part2 == part2 && DataConnections[i].index2 == index2)))
			{
				index2amount++;
				lasti2 = i;
			}
		}
		/*std::cout << "{";
		std::cout << "index1amount ";
		std::cout << index1amount;
		std::cout << " index2amount ";
		std::cout << index2amount;*/
		if (it == -1)
		{
			DataConnection dc;
			dc.part1 = part1;
			dc.part2 = part2;
			dc.index1 = index1;
			dc.index2 = index2;
			dc.type = type;
			bool s1 = false;
			bool s2 = false;
			if (Parts[part1]->bDataConnections[index1].source && type == 0 ||
				Parts[part1]->fDataConnections[index1].source && type == 1 ||
				Parts[part1]->vDataConnections[index1].source && type == 2)
				s1 = true;
			if (Parts[part2]->bDataConnections[index2].source && type == 0 ||
				Parts[part2]->fDataConnections[index2].source && type == 1 ||
				Parts[part2]->vDataConnections[index2].source && type == 2)
				s2 = true;
			//std::cout << " s1 ";
			//std::cout << s1;
			//std::cout << " s2 ";
			//std::cout << s2;
			//std::cout << "}";
			if (s1 != s2)
			{
				if (!s1 && index1amount > 0)
				{
					DataConnections[lasti1] = DataConnections[DataConnections.size() - 1];
					DataConnections.pop_back();
					PlaySound(&Detach, MousePosition, 3.5f, 0.3f);
					DataConnections.push_back(dc);
				}
				else if (!s2 && index2amount > 0)
				{
					DataConnections[lasti2] = DataConnections[DataConnections.size() - 1];
					DataConnections.pop_back();
					PlaySound(&Detach, MousePosition, 3.5f, 0.3f);
					DataConnections.push_back(dc);
				}
				else
					DataConnections.push_back(dc);
			}
		}
		else if(it>=0)
		{
			DataConnections[it] = DataConnections[DataConnections.size() - 1];
			DataConnections.pop_back();
			PlaySound(&Detach, MousePosition, 3.5f, 0.3f);
		}
	}
}
void CentralPart::SaveTo(std::string filename)
{
	std::string fn = "./Ships/"+filename + ".sav";
	filename = fn;
	std::ofstream f;
	f.open(filename);
	glm::vec2 mid = 0.5f * (Parts[0]->body[0].position + Parts[0]->body[1].position);
	for (int i = 1; i < Parts.size(); i++) // skip part[0] - central part
	{
		f << "P ";
		f << Parts[i]->partid;
		f << " ";
		f << Parts[i]->bodysize;
		for (int a = 0; a < Parts[i]->bodysize; a++)
		{
			f << " ";
			f << Parts[i]->body[a].position.x - mid.x;
			f << " ";
			f << Parts[i]->body[a].position.y - mid.y;
		}
		f << "\n";
	}
	for (int i = 0; i < Connections.size(); i++)
	{
		f << "C ";
		f << Connections[i].type;
		f << " ";
		f << Connections[i].length;
		f << " ";
		f << Connections[i].width;
		f << " ";
		f << Connections[i].stiffnes;
		f << " ";
		f << Connections[i].absorbtion;
		f << " ";
		f << Connections[i].HeatTransferSpeed;
		f << " ";
		f << Connections[i].part1;
		f << " ";
		f << Connections[i].index1;
		f << " ";
		f << Connections[i].part2;
		f << " ";
		f << Connections[i].index2;
		f << "\n";
	}
	for (int i = 0; i < DataConnections.size(); i++)
	{
		f << "D ";
		f << DataConnections[i].type;
		f << " ";
		f << DataConnections[i].part1;
		f << " ";
		f << DataConnections[i].index1;
		f << " ";
		f << DataConnections[i].part2;
		f << " ";
		f << DataConnections[i].index2;
		f << "\n";
	}
	f.close();
	std::cout<<"\n"<<filename<<" Saved";
}
void CentralPart::LoadFrom(std::string filename)
{
	std::string fn = "./Ships/"+filename + ".sav";
	filename = fn;
	std::cout<<"\nLoading Entity: "<<filename;
	glm::vec2 mid = 0.5f * (Parts[0]->body[0].position + Parts[0]->body[1].position);
	Clear();
	std::ifstream f(filename);
	if (!f.is_open())
	{
		std::cerr << "ERROR LOADING Entity: Unable to open " << filename;
		return;
	}
	while (!f.eof())
	{
		char junk;
		char line[256];
		f.getline(line, 256);
		std::strstream s;
		s << line;
		if (line[0] == 'P')
		{
			std::vector<glm::vec2> positions;

			int type = 0;
			int bodysize = 0;

			s >> junk >> type >> bodysize;
			for (int i = 0; i < bodysize; i++)
			{
				float x = 0;
				float y = 0;

				s >> x >> y;
				positions.push_back({ x,y });
			}
			BodyComponent* b = CreatePart(type, { 0.0f,0.0f }, { 0.0f,1.0f }, PARTSIZE);
			if(b!=NULL)
			{
				b->Create({ 0.0f,0.0f }, { 0.0f,1.0f }, PARTSIZE);
				for (int i = 0; i < bodysize; i++)
					b->body[i].position = positions[i]+ mid;
				Parts.push_back(b);
			}
		}

		if (line[0] == 'C')
		{
			int type;
			float len;
			float width;
			float stiffness;
			float absorbtion;
			float HeatTransferSpeed;
			int part1;
			int index1;
			int part2;
			int index2;

			s >>junk>> type >> len >> width >> stiffness >> absorbtion 
				>> HeatTransferSpeed >> part1 >> index1 >> part2 >> index2;

			AddConnection(type, len, width, stiffness, absorbtion, HeatTransferSpeed,
				part1, index1,
				part2, index2);
		}
		if (line[0] == 'D')
		{
			int type;
			int part1;
			int index1;
			int part2;
			int index2;

			s >> junk >> type >> part1 >> index1 >> part2 >> index2;
			/*std::cout<< "adding dc: ";
			std::cout << type << ' ';
			std::cout << part1 << ' ';
			std::cout << index1 << ' ';
			std::cout << part2 << ' ';
			std::cout << index2 << ' ';*/
			AddDataConnection(type,part1, index1, part2, index2);

			/*std::cout << "consize = " << DataConnections.size();
			std::cout <<  '\n';*/
		}

	}
	f.close();
	firstdrawafterload = true;
	
	std::cout<<"\nLoaded";
}
void CentralPart::Clear()
{
	for (int i = 1; i < Parts.size(); i++)
		Parts[i]->Delete=true;
	Parts.clear();
	Parts.push_back(this);
	Connections.clear();
	DataConnections.clear();
	firstdrawafterload = true;
}
void CentralPart::Destroy()
{
	for (int i = 0; i < Parts.size(); i++)
	{
		if (rand() % 100 < 50)
			for (int b = 0; b < Parts[i]->bodysize; b++)
			{
				Parts[i]->body[b].Force = glm::vec2(rand() % 5000 - 2500, rand() % 5000 - 2500);
				Parts[i]->body[b].temperature = Parts[i]->MaxTemp;
			}	
		else
			Parts[i]->Health = -1.0f;
	}
	Connections.clear();
	DataConnections.clear();
	while(Parts.size()>0)
		DetachPart(Parts.size()-1);
	Balls.clear();
	Engines.clear();
	CloseDamageSpheres.clear();
	destroyed = true;
	firstdrawafterload = true;
}
