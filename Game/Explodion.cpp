#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"
#include "ParticleMaterials.h"
#include "Textures.h"
#include "DamageSphere.h"
#include "Explodion.h"

Explodion NewExplodion(glm::vec2 position, float r, float dmg, float lifetime,float recoil)
{
	Explodion e;
	e.DS.body.position = position;
	e.DS.body.r = r;
	e.DS.Damage = dmg;
	e.DS.neutral = true;
	e.DS.recoil = recoil;
	e.DS.Heat = r *0.005f;
	e.DS.singleHit = false;
	e.DS.timeLeft = 0.001f;

	e.timeLeft = lifetime;
	e.lifet = lifetime;
	ExplodionPE.SpawnInCircle(e.DS.body.position, e.DS.body.r, 100);
	playsound(ExplodionSound, position, 2.0f,0.5f);
	DamageSpheresArray.push_back(e.DS);
	return e;
}

void SpawnExplodion(glm::vec2 position, float r, float dmg, float lifetime, float recoil)
{
	ExplodionArray.push_back(NewExplodion(position, r, dmg, lifetime, recoil));
}

void ProcessExplodions(float dt)
{
	Explodions.clear();
	int iter = 0;
	while (iter < ExplodionArray.size())
	{


		if (ExplodionArray[iter].timeLeft <= 0.0f)
		{
			ExplodionArray[iter] = ExplodionArray[ExplodionArray.size() - 1];
			ExplodionArray.pop_back();
		}
		else
			iter++;
	}


}
void DrawExplodions(float dt)
{	
	glm::vec3 Color = glm::vec3(10.0f, 2.0f, 1.0f);
	for (int i = 0; i < ExplodionArray.size(); i++)
	{
		float stage = ExplodionArray[i].timeLeft / ExplodionArray[i].lifet;
		DrawCircle(ExplodionArray[i].DS.body.position, ExplodionArray[i].DS.body.r * stage *1.2f,glm::vec4(Color.x, Color.y, Color.z,stage), false, NULL, 100, true);
		
		DrawLight(glm::vec3(ExplodionArray[i].DS.body.position.x, ExplodionArray[i].DS.body.position.y,  0.010f ), glm::vec2(ExplodionArray[i].DS.body.r * stage * 15.0f), glm::vec4(Color.x * stage*2.0f, Color.y * stage * 2.0f, Color.z* stage * 2.0f, 1.0f));
		AddSphereOfInfluence(ExplodionArray[i].DS.body.position, ExplodionArray[i].DS.body.r * stage*7.5f, { 0.0f,0.0f },true, -120.0f*stage);
		ExplodionPE.influenced = true;
		bool del = true;
		
		ExplodionArray[i].timeLeft -= dt;
	}

}