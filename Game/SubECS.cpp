
#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"
#include "DamageSphere.h"
#include "Explodion.h"
#include "SubECS.h"
#include "Parts/Base.h"
#include "Parts/CentralPart.h"
#include "ParticleMaterials.h"

void ProcessDamageSpheres(float dt)
{
	int iter = 0;
	while (iter<DestructableStaticBalls.size())
		if(DestructableStaticBalls[iter]->Delete || DestructableStaticBalls[iter]->health <=0.0f)
		{
			DestructableStaticBalls[iter] = DestructableStaticBalls[DestructableStaticBalls.size()-1];
			DestructableStaticBalls.pop_back();
		}	
		else
			iter++;
	iter = 0;
	while (iter<DestructableStaticCubes.size())
		if(DestructableStaticCubes[iter]->Delete || DestructableStaticCubes[iter]->health <=0.0f)
		{
			DestructableStaticCubes[iter] = DestructableStaticCubes[DestructableStaticCubes.size()-1];
			DestructableStaticCubes.pop_back();
		}		
		else
			iter++;
	
	for(auto ds : DamageSpheres)
	{
		if(ds->dead)
			continue;
		//DestructableStaticCubes
		for(auto dcb : DestructableStaticBalls)
			if(BtBCollisionCheck(dcb->b,ds->body))
				{
					dcb->health -= ds->Damage;
					dcb->temperature += ds->Heat;
					if(ds->singleHit)
						{
							ds->dead = true;
							break;
						}
				}
		for(auto dcc : DestructableStaticCubes)
			if(BtCCollisionCheck(ds->body,dcc->c))
				{
					dcc->health -= ds->Damage;
					dcc->temperature += ds->Heat;
					if(ds->singleHit)
						{
							ds->dead = true;
							break;
						}
				}
	}

	DamageSpheres.clear();
	glm::vec3 DamageColor = glm::vec3(0.0f,1.0f,1.0f);
	glm::vec3 HeatColor = glm::vec3(10.0f,2.0f,1.1f);
	glm::vec3 ColdColor = glm::vec3(HeatColor.z, HeatColor.y, HeatColor.x);
	glm::vec3 FrictionColor = glm::vec3(1.0f,1.0f,1.0f);

	for (int i = 0; i < DamageSpheresArray.size(); i++)
	{
		bool del = true;

	

		while (del && i < DamageSpheresArray.size())
		{
			if (!DamageSpheresArray[i].singleHit && !DamageSpheresArray[i].infinite)
			{
				DamageSpheresArray[i].timeLeft -= dt;
				

				
				DrawCircle(DamageSpheresArray[i].body,
					glm::vec4(DamageColor * DamageSpheresArray[i].Damage
						+ (DamageSpheresArray[i].Heat > 0.0f ? DamageSpheresArray[i].Heat * HeatColor: -DamageSpheresArray[i].Heat*ColdColor)
						+ FrictionColor* DamageSpheresArray[i].friction,
						0.1f* DamageSpheresArray[i].timeLeft >1.0f? 1.0f: 0.1f * DamageSpheresArray[i].timeLeft));



				DamageSpheres.push_back(&DamageSpheresArray[i]);
			}
			if (DamageSpheresArray[i].dead || DamageSpheresArray[i].timeLeft <= 0.0f)
			{
				del = true;
				DamageSpheresArray[i] = DamageSpheresArray[DamageSpheresArray.size() - 1];
				DamageSpheresArray.pop_back();
			}
			else
				del = false;
		}
	}


}


DestructableStaticBall::DestructableStaticBall()
{
	type = NodeType::LASTNODE - 1;
	Name = "DestructableStaticBall";
	DestructableStaticBalls.push_back(this);
	ObjectPreconstructor();
	UsedAssets.push_back((Asset**)&partAsset);
}
DestructableStaticBall::~DestructableStaticBall()
{
	for(int i=0;i<DestructableStaticBalls.size();i++)
		if(DestructableStaticBalls[i] == this)
			{
				DestructableStaticBalls[i] = DestructableStaticBalls[DestructableStaticBalls.size()-1];
				DestructableStaticBalls.pop_back();
				return;
			}	
}
UI_DataPack DestructableStaticBall::GetUIDataDestructableStaticBall()
{
	UI_DataPack uidp;
	uidp.name = "DestructableStaticBall";
	uidp.fdata.push_back(&health);
	uidp.fdatanames.push_back("Health");	
	uidp.fdata.push_back(&temperature);
	uidp.fdatanames.push_back("temperature");	
	uidp.fdata.push_back(&maxTemperature);
	uidp.fdatanames.push_back("maxTemperature");	
	uidp.fdata.push_back(&trgTemperature);
	uidp.fdatanames.push_back("trgtemperature");	
	uidp.fdata.push_back(&passiveCooling);
	uidp.fdatanames.push_back("passiveCooling");	
	uidp.fdata.push_back(&DestrParticleSpeed);
	uidp.fdatanames.push_back("DestrParticleSpeed");	
	uidp.fdata.push_back(&explodionR);
	uidp.fdatanames.push_back("explodionR");	
	uidp.fdata.push_back(&explodionDamage);
	uidp.fdatanames.push_back("explodionDamage");	
	uidp.fdata.push_back(&explodionLifeTime);
	uidp.fdatanames.push_back("explodionLifeTime");	
	uidp.fdata.push_back(&explodionRecoil);
	uidp.fdatanames.push_back("explodionRecoil");	
	uidp.idata.push_back(&DestrParticleAmount);
	uidp.idatanames.push_back("DestrParticleAmount");	
	uidp.bdata.push_back(&SpawnInShapeOrPoint);
	uidp.bdatanames.push_back("SpawnInShapeOrPoint");	
	uidp.bdata.push_back(&ExplodeOnOverheat);
	uidp.bdatanames.push_back("ExplodeOnOverheat");	
	uidp.bdata.push_back(&ExplodeOnDeath);
	uidp.bdatanames.push_back("ExplodeOnDeath");	
	uidp.bdata.push_back(&DieOnOverheat);
	uidp.bdatanames.push_back("DieOnOverheat");	
	uidp.bdata.push_back(&Invincibility);
	uidp.bdatanames.push_back("Invincibility");	
	uidp.textdata.push_back(&partAssetName);
	uidp.textdatanames.push_back("DestructionParticleAsset");	
	uidp.v4data.push_back(&HeatColor);
	uidp.v4datanames.push_back("HeatColor");	
	uidp.v4data.push_back(&ColdColor);
	uidp.v4datanames.push_back("ColdColor");	
	uidp.v4data.push_back(&color);
	uidp.v4datanames.push_back("color");	
	return uidp;
}
std::vector<UI_DataPack> DestructableStaticBall::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataCollisionObject());
	data.push_back(GetUIDataCO_Ball());
	data.push_back(GetUIDataDestructableStaticBall());
	return data;
};
void DestructableStaticBall::MTProcess(float dt)
{
	ObjectUpdateMaterial();
	b.Force = { 0.0f,0.0f };
	b.velocity = { 0.0f,0.0f };
	b.Process(dt);
	b.position = position;
	b.roughness = roughness;
	b.bounciness = bounciness;
	if (temperature > trgTemperature)
		temperature -= dt * (passiveCooling + temperature/maxTemperature * passiveCooling);
	if (temperature < trgTemperature)
		temperature += dt * (passiveCooling + temperature/maxTemperature * passiveCooling);
	float tmpdiv = 1.0f / maxTemperature;
	if (temperature > 0.0f)
		Color = color + HeatColor * (temperature * tmpdiv);
	else
		Color = color + ColdColor * (temperature * tmpdiv);
	if(Invincibility)
		health = 10.0f;
	if(DieOnOverheat && temperature > maxTemperature)
		health = -10.0f;
	
	
	bool texexists = false;
	for(auto pa : AvailableParticleAssets)
	{
		if(pa->Name == partAssetName && !texexists && !pa->Delete)
		{
			partAsset = pa;
			texexists=true;
			break;
		}
	}
	if(!texexists)
		partAsset = NULL;
	if(health<=0.0f && !Delete)
	{
		health = -100.0f;// dead for shure
		Delete = true;
		
		if(ExplodeOnDeath || (ExplodeOnOverheat && temperature > maxTemperature))
			SpawnExplodion(position,explodionR, explodionDamage,explodionLifeTime,explodionRecoil);
		if(partAsset!=NULL)
		{
			if(SpawnInShapeOrPoint)
			{
				for(int i=0;i<DestrParticleAmount;i++)
				{
					glm::vec2 randvec = {(rand()%1000 * 0.002f) - 1.0f,(rand()%1000 * 0.002f) - 1.0f};
					partAsset->pe.Spawn(Normalize(randvec) * b.r + position,randvec * DestrParticleSpeed,1);
				}
			}
			else
			{
				for(int i=0;i<DestrParticleAmount;i++)
				{
					glm::vec2 randvec = {(rand()%1000 * 0.002f) - 1.0f,(rand()%1000 * 0.002f) - 1.0f};
					partAsset->pe.Spawn(position,randvec * DestrParticleSpeed,1);
				}
			}
		}
	}
}



DestructableStaticCube::DestructableStaticCube()
{
	type = NodeType::LASTNODE - 2;
	Name = "DestructableStaticCube";
	DestructableStaticCubes.push_back(this);
	ObjectPreconstructor();
	UsedAssets.push_back((Asset**)&partAsset);
}
DestructableStaticCube::~DestructableStaticCube()
{
	for(int i=0;i<DestructableStaticCubes.size();i++)
		if(DestructableStaticCubes[i] == this)
			{
				DestructableStaticCubes[i] = DestructableStaticCubes[DestructableStaticCubes.size()-1];
				DestructableStaticCubes.pop_back();
				return;
			}	
}
UI_DataPack DestructableStaticCube::GetUIDataDestructableStaticCube()
{
	UI_DataPack uidp;
	uidp.name = "DestructableStaticCube";
	uidp.fdata.push_back(&health);
	uidp.fdatanames.push_back("Health");	
	uidp.fdata.push_back(&temperature);
	uidp.fdatanames.push_back("temperature");	
	uidp.fdata.push_back(&maxTemperature);
	uidp.fdatanames.push_back("maxTemperature");	
	uidp.fdata.push_back(&trgTemperature);
	uidp.fdatanames.push_back("trgtemperature");	
	uidp.fdata.push_back(&passiveCooling);
	uidp.fdatanames.push_back("passiveCooling");	
	uidp.fdata.push_back(&DestrParticleSpeed);
	uidp.fdatanames.push_back("DestrParticleSpeed");	
	uidp.fdata.push_back(&explodionR);
	uidp.fdatanames.push_back("explodionR");	
	uidp.fdata.push_back(&explodionDamage);
	uidp.fdatanames.push_back("explodionDamage");	
	uidp.fdata.push_back(&explodionLifeTime);
	uidp.fdatanames.push_back("explodionLifeTime");	
	uidp.fdata.push_back(&explodionRecoil);
	uidp.fdatanames.push_back("explodionRecoil");	
	uidp.idata.push_back(&DestrParticleAmount);
	uidp.idatanames.push_back("DestrParticleAmount");	
	uidp.bdata.push_back(&SpawnInShapeOrPoint);
	uidp.bdatanames.push_back("SpawnInShapeOrPoint");	
	uidp.bdata.push_back(&ExplodeOnOverheat);
	uidp.bdatanames.push_back("ExplodeOnOverheat");	
	uidp.bdata.push_back(&ExplodeOnDeath);
	uidp.bdatanames.push_back("ExplodeOnDeath");	
	uidp.bdata.push_back(&DieOnOverheat);
	uidp.bdatanames.push_back("DieOnOverheat");	
	uidp.bdata.push_back(&Invincibility);
	uidp.bdatanames.push_back("Invincibility");	
	uidp.textdata.push_back(&partAssetName);
	uidp.textdatanames.push_back("DestructionParticleAsset");	
	uidp.v4data.push_back(&HeatColor);
	uidp.v4datanames.push_back("HeatColor");	
	uidp.v4data.push_back(&ColdColor);
	uidp.v4datanames.push_back("ColdColor");	
	uidp.v4data.push_back(&color);
	uidp.v4datanames.push_back("color");	
	return uidp;
}
std::vector<UI_DataPack> DestructableStaticCube::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataCollisionObject());
	data.push_back(GetUIDataCO_Cube());
	data.push_back(GetUIDataDestructableStaticCube());
	return data;
};
void DestructableStaticCube::MTProcess(float dt)
{
	ObjectUpdateMaterial();
	c.position = position;
	
	if (temperature > trgTemperature)
		temperature -= dt * (passiveCooling + temperature/maxTemperature * passiveCooling);
	if (temperature < trgTemperature)
		temperature += dt * (passiveCooling + temperature/maxTemperature * passiveCooling);
	float tmpdiv = 1.0f / maxTemperature;
	if (temperature > 0.0f)
		Color = color + HeatColor * (temperature * tmpdiv);
	else
		Color = color + ColdColor * (temperature * tmpdiv);
	if(DieOnOverheat && temperature > maxTemperature)
		health = -10.0f;
	
	
	bool texexists = false;
	for(auto pa : AvailableParticleAssets)
	{
		if(pa->Name == partAssetName && !texexists && !pa->Delete)
		{
			partAsset = pa;
			texexists=true;
			break;
		}
	}
	if(!texexists)
		partAsset = NULL;
	if(Invincibility)
		health = 10.0f;
	if(health<=0.0f && !Delete)
	{
		health = -100.0f;// dead for shure
		Delete = true;
		
		if(ExplodeOnDeath || (ExplodeOnOverheat && temperature > maxTemperature))
			SpawnExplodion(position,explodionR, explodionDamage,explodionLifeTime,explodionRecoil);
		if(partAsset!=NULL)
		{
			if(SpawnInShapeOrPoint)
			{
				for(int i=0;i<DestrParticleAmount;i++)
				{
					glm::vec2 randvec = {(rand()%1000 * 0.002f) - 1.0f,(rand()%1000 * 0.002f) - 1.0f};
					partAsset->pe.Spawn(randvec * glm::vec2(c.width,c.height)+ position,randvec * DestrParticleSpeed,1);
				}
			}
			else
			{
				for(int i=0;i<DestrParticleAmount;i++)
				{
					glm::vec2 randvec = {(rand()%1000 * 0.002f) - 1.0f,(rand()%1000 * 0.002f) - 1.0f};
					partAsset->pe.Spawn(position,randvec * DestrParticleSpeed,1);
				}
			}
		}
	}
}



StaticDamageSphere::StaticDamageSphere()
{
	type = NodeType::LASTNODE - 3;
	Name = "StaticDamageSphere";
}
StaticDamageSphere::~StaticDamageSphere()
{// just kys
}
UI_DataPack StaticDamageSphere::GetUIDataStaticDamageSphere()
{
	UI_DataPack uidp;
	uidp.name = "StaticDamageSphere";
	uidp.fdata.push_back(&attractionForce);
	uidp.fdatanames.push_back("attractionForce");	
	uidp.fdata.push_back(&damage);
	uidp.fdatanames.push_back("damage");	
	uidp.fdata.push_back(&heat);
	uidp.fdatanames.push_back("heat");	
	uidp.fdata.push_back(&radius);
	uidp.fdatanames.push_back("radius");
	uidp.bdata.push_back(&attractive);
	uidp.bdatanames.push_back("attractive");
	uidp.bdata.push_back(&disable);
	uidp.bdatanames.push_back("disable");
	uidp.fdata.push_back(&Friction);
	uidp.fdatanames.push_back("Friction");
	
	return uidp;
}
std::vector<UI_DataPack> StaticDamageSphere::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataStaticDamageSphere());
	return data;
};
void StaticDamageSphere::PreProcess()
{
	ds.body.position = position;
	ds.body.r = radius;
	ds.Damage = damage;
	ds.Heat = heat;
	ds.friction = Friction;
	ds.infinite = true;
	ds.neutral = true;
	ds.singleHit = false;
	ds.recoil = -attractionForce * attractive;
	
	if(!disable)
		DamageSpheres.push_back(&ds); 
	if(attractive && !disable)
		AddSphereOfInfluence(position,radius,{0.0f,0.0f},true,attractionForce * 100.0f);
}
void StaticDamageSphere::DebugDraw()
{
	DrawCircle(position,radius,{0.9f,0.1f,0.1f,0.8f});
}
void StaticDamageSphere::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{
	glm::vec2 dif = mp - prevmp;
	radius -= length(prevdif);
	radius += length(dif);
}
bool StaticDamageSphere::SelectionCheck(glm::vec2 point)
{
	return sqrlength(point - position) < radius*radius; 
}



Monitor::Monitor()
{
	type = NodeType::LASTNODE - 4;
	Name = "Monitor";
}
UI_DataPack Monitor::GetUIDataMonitor()
{
	UI_DataPack uidp;
	uidp.name = "Monitor";
	uidp.fdata.push_back(&textScale);
	uidp.fdatanames.push_back("textScale");	
	uidp.fdata.push_back(&textStep);
	uidp.fdatanames.push_back("textStep");	
	uidp.v2data.push_back(&textOffset);
	uidp.v2datanames.push_back("textOffset");
	uidp.bdata.push_back(&bleepLastLine);
	uidp.bdatanames.push_back("bleepLastLine");

	uidp.textdata.push_back(&text1);
	uidp.textdatanames.push_back("text1");
	uidp.textdata.push_back(&text2);
	uidp.textdatanames.push_back("text2");
	uidp.textdata.push_back(&text3);
	uidp.textdatanames.push_back("text3");
	uidp.textdata.push_back(&text4);
	uidp.textdatanames.push_back("text4");
	uidp.textdata.push_back(&text5);
	uidp.textdatanames.push_back("text5");
	uidp.textdata.push_back(&text6);
	uidp.textdatanames.push_back("text6");
	uidp.textdata.push_back(&text7);
	uidp.textdatanames.push_back("text7");
	uidp.textdata.push_back(&text8);
	uidp.textdatanames.push_back("text8");
	uidp.textdata.push_back(&text9);
	uidp.textdatanames.push_back("text9");
	uidp.textdata.push_back(&text10);
	uidp.textdatanames.push_back("text10");

	uidp.v4data.push_back(&color1);
	uidp.v4datanames.push_back("color1");
	uidp.v4data.push_back(&color2);
	uidp.v4datanames.push_back("color2");
	uidp.v4data.push_back(&color3);
	uidp.v4datanames.push_back("color3");
	uidp.v4data.push_back(&color4);
	uidp.v4datanames.push_back("color4");
	uidp.v4data.push_back(&color5);
	uidp.v4datanames.push_back("color5");
	uidp.v4data.push_back(&color6);
	uidp.v4datanames.push_back("color6");
	uidp.v4data.push_back(&color7);
	uidp.v4datanames.push_back("color7");
	uidp.v4data.push_back(&color8);
	uidp.v4datanames.push_back("color8");
	uidp.v4data.push_back(&color9);
	uidp.v4datanames.push_back("color9");
	uidp.v4data.push_back(&color10);
	uidp.v4datanames.push_back("color10");
	return uidp;
}
std::vector<UI_DataPack> Monitor::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataMonitor());
	return data;
};
void  Monitor::Draw()
{
	Material m;
	if(Mater!=NULL)
		m = Mater->mater;
	m.flipX = invertX;
	m.flipY = invertY;
	if(m.Texture == NULL)
		m.Texture = FlatColorTexture;
	if(m.NormalMap == NULL)
		m.NormalMap = NULL;
	DrawQuadWithMaterial(position,Scale,m,rotation,Color,Z_Index,Additive,depth);
	if(!disable)
	{
		glm::vec2 tpos = position - glm::vec2(Scale.x,-Scale.y) + textOffset;
		DrawText(text1,tpos,textScale,color1,Z_Index);
		tpos.y += textStep;
		DrawText(text2,tpos,textScale,color2,Z_Index);
		tpos.y += textStep;
		DrawText(text3,tpos,textScale,color3,Z_Index);
		tpos.y += textStep;
		DrawText(text4,tpos,textScale,color4,Z_Index);
		tpos.y += textStep;
		DrawText(text5,tpos,textScale,color5,Z_Index);
		tpos.y += textStep;
		DrawText(text6,tpos,textScale,color6,Z_Index);
		tpos.y += textStep;
		DrawText(text7,tpos,textScale,color7,Z_Index);
		tpos.y += textStep;
		DrawText(text8,tpos,textScale,color8,Z_Index);
		tpos.y += textStep;
		DrawText(text9,tpos,textScale,color9,Z_Index);
		tpos.y += textStep;
		if(bleepLastLine)
		{
			timer += delta;
			if(timer > 0.5f)
			{
				timer = 0.0f;
				flip = !flip;
			}
			if(flip)
				DrawText(text10,tpos,textScale,color10,Z_Index);
		}
		else DrawText(text10,tpos,textScale,color10,Z_Index);
	}

}



TriggerCube::TriggerCube()
{
	type = NodeType::LASTNODE - 5;
	Name = "TriggerCube";
	TriggerCubes.push_back(this);	
}
TriggerCube::~TriggerCube()
{
	for(int i=0;i<TriggerCubes.size();i++)
		if(TriggerCubes[i] == this)
		{
			TriggerCubes[i] = TriggerCubes[TriggerCubes.size()-1];
			TriggerCubes.pop_back();
			return;
		}	
}
UI_DataPack TriggerCube::GetUIDataTriggerCube()
{
	UI_DataPack uidp;
	uidp.name = "TriggerCube";
	uidp.bdata.push_back(&disable);
	uidp.bdatanames.push_back("disable");
	uidp.bdata.push_back(&triggered);
	uidp.bdatanames.push_back("triggered");
	uidp.bdata.push_back(&determineInside);
	uidp.bdatanames.push_back("determineInside");

	uidp.v2data.push_back(&Scale);
	uidp.v2datanames.push_back("Scale");

	return uidp;
}
std::vector<UI_DataPack> TriggerCube::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataTriggerCube());
	return data;
};
void TriggerCube::MTPreProcess()
{
	if(Entities.size()>0)
	{
		cube c;
		c.width=  Scale.x;
		c.height=  Scale.y;
		c.position = position;
		if(PointToQuadCollisionCheck(c,Entities[0]->mid))
			triggered = true;
		else
			triggered = false;
	}
}
void TriggerCube::DebugDraw()
{
	DrawCube(position,Scale,0.0f,{0.9f,0.1f,0.1f,0.8f});
}
void TriggerCube::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{
	glm::vec2 dif = mp - prevmp;
	Scale -= prevdif;
	Scale += dif;
}
bool TriggerCube::SelectionCheck(glm::vec2 point)
{
	cube c;
	c.width=  Scale.x;
	c.height=  Scale.y;
	c.position = position;
	return PointToQuadCollisionCheck(c,point); 
}
