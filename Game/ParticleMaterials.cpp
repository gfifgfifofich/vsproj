#include "engine/Components/Redactor.h"
#include "ParticleMaterials.h"
#include "Textures.h"
#include "Radar.h"

void SetupEnginePE()
{

	EngineSmoke.material.Texture = noize;
	EngineSmoke.Type = "TEXTUREDLINE";
	EngineSmoke.VelocityRandomness = { -500,500,-200,200 };
	EngineSmoke.VelocityRandomness *= 0.025f;
	EngineSmoke.StartColor = { 500.0f,100.0f,1.0f,5.0f };
	EngineSmoke.EndColor = { 500.0f,100.0f,1.0f,1.0f };
	EngineSmoke.OrbitalVelocityRandomness = 1.5f;
	EngineSmoke.lighted = true;
	EngineSmoke.RotationRandomness = 10.0f;
	EngineSmoke.InitialRotation = 10.0f;
	EngineSmoke.lifetimeRandomness = 0.2f;
	EngineSmoke.lifetime = 0.2f;
	EngineSmoke.VelocityDamper = 0.1f * 60;
	EngineSmoke.Z_Index = -10;
	EngineSmoke.StartSize = { 20 * 0.025f,2.25 };
	EngineSmoke.EndSize = { 1.0f * 0.025f,0.5f };

	EngineSmoke.Additive = true;
}
void SetupSmokePE()
{

	Smoke.material.Texture = noize;
	Smoke.Type = "TEXTURED";
	Smoke.VelocityRandomness = { -700,700,-700,700 };
	Smoke.VelocityRandomness *= 0.025f;
	Smoke.StartColor = { 3.0f,3.0f,3.0f,3.0f };
	Smoke.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	Smoke.OrbitalVelocityRandomness = 1.5f;
	Smoke.lighted = true;
	Smoke.RotationRandomness = 10.0f;
	Smoke.InitialRotation = 10.0f;
	Smoke.lifetimeRandomness = 1.0f;
	Smoke.lifetime = 0.5f;
	Smoke.Z_Index = 100;
	Smoke.VelocityDamper = 0.2f * 60;
	Smoke.StartSize = { 10.0f,10.0f };
	Smoke.StartSize *= 0.025f;
	Smoke.EndSize = { 50.0f,50.f };
	Smoke.EndSize *= 0.025f;
	Smoke.Additive = true;
}
void SetupSparksPE()
{

	Sparks.Type = "TEXTUREDLINE";
	Sparks.VelocityRandomness = { -1500,1500,-1500,1500 };
	Sparks.VelocityRandomness *= 0.025f;
	Sparks.acceleration = { 0.0f,0.0f };
	Sparks.StartColor = { 30.0f,10.0f,0.0f,1.0f };
	Sparks.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	Sparks.lighted = true;
	Sparks.lifetimeRandomness = 0.2f;
	Sparks.lifetime = 0.5f;
	Sparks.VelocityDamper = 0.0f;
	Sparks.Z_Index = 1000;
	
	Sparks.StartSize = { 2 * 0.025f ,2 };
	Sparks.EndSize = { 2 * 0.025f ,2};
	Sparks.material =PrimitiveQuad;
}
void SetupBulletPE()
{
	bulletFlightPm.Name = "AAA";
	bulletFlightPm.Type = "TEXTUREDLINE";
	bulletFlightPm.VelocityRandomness = { -50,50,-50,50 };
	bulletFlightPm.VelocityRandomness *= 0.025f;
	bulletFlightPm.StartColor = { 300.0f,20.0f,4.0f,1.0f };
	bulletFlightPm.EndColor = { 100.0f,10.0f,4.0f,0.0f };
	bulletFlightPm.OrbitalVelocityRandomness = 1.5f;
	bulletFlightPm.lighted = false;
	bulletFlightPm.RotationRandomness = 10.0f;
	bulletFlightPm.InitialRotation = 10.0f;
	bulletFlightPm.lifetimeRandomness = 0.2f;
	bulletFlightPm.lifetime = 0.4f;
	bulletFlightPm.VelocityDamper = 0.1f * 60;
	bulletFlightPm.material =PrimitiveQuad;

	bulletFlightPm.StartSize = { 2 * 0.025f,1 };
	bulletFlightPm.EndSize = { 0,1 };
	bulletFlightPm.Z_Index = 0;

	bulletColisionPm.material.Texture = noize;
	bulletColisionPm.Type = "TEXTURED";
	bulletColisionPm.VelocityRandomness = { -250,250,-250,250 };
	bulletColisionPm.VelocityRandomness *= 0.025f;
	bulletColisionPm.acceleration = { 0, 0 };
	bulletColisionPm.StartColor = { 10.0f,10.0f,10.0f,3.0f };
	bulletColisionPm.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	bulletColisionPm.OrbitalVelocityRandomness = 1.5f;
	bulletColisionPm.lighted = true;
	bulletColisionPm.RotationRandomness = 10.0f;
	bulletColisionPm.InitialRotation = 10.0f;
	bulletColisionPm.lifetimeRandomness = 0.5f;
	bulletColisionPm.lifetime = 1.0f;
	bulletColisionPm.VelocityDamper = 0.1f * 60;

	bulletColisionPm.StartSize = { 10,10 };
	bulletColisionPm.StartSize *= 0.025f;
	bulletColisionPm.EndSize = { 50,50 };
	bulletColisionPm.EndSize *= 0.025f;
	bulletColisionPm.Z_Index = 0;


}
void SetupLaserPE()
{
	LaserPm.material.Texture = noize;
	LaserPm.Type = "TEXTURED";
	LaserPm.VelocityRandomness = { -150,150,-150,150 };
	LaserPm.VelocityRandomness *= 0.025f;
	LaserPm.acceleration = { 0,150 };
	LaserPm.acceleration *= 0.025f;
	LaserPm.StartColor = { 100.0f,100.0f,100.0f,3.0f };
	LaserPm.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	LaserPm.OrbitalVelocityRandomness = 1.5f;
	LaserPm.lighted = true;
	LaserPm.RotationRandomness = 10.0f;
	LaserPm.InitialRotation = 10.0f;
	LaserPm.lifetimeRandomness = 0.0f;
	LaserPm.lifetime = 1.0f;
	LaserPm.VelocityDamper = 0.1f * 60;

	LaserPm.StartSize = { 10,10 };
	LaserPm.StartSize *= 0.025f;
	LaserPm.EndSize = { 50,50 };
	LaserPm.EndSize *= 0.025f;
	LaserPm.Z_Index = 0;

}
void SetupGunPE()
{
	GunShotPE.material.Texture = noize;
	GunShotPE.Type = "TEXTURED";
	GunShotPE.VelocityRandomness = { -25.0,25.0,-25.0,25.0 };
	GunShotPE.VelocityRandomness *= 0.025f;
	GunShotPE.acceleration = { 0, 0 };
	GunShotPE.StartColor = { 3.0f,3.0f,3.0f,3.0f };
	GunShotPE.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	GunShotPE.OrbitalVelocityRandomness = 1.5f;
	GunShotPE.lighted = true;
	GunShotPE.RotationRandomness = 10.0f;
	GunShotPE.InitialRotation = 10.0f;
	GunShotPE.lifetimeRandomness = 0.5f;
	GunShotPE.lifetime = 1.0f;
	GunShotPE.VelocityDamper = 0.2f * 60;
	GunShotPE.Additive = true;
	GunShotPE.StartSize = { 10,10 };
	GunShotPE.StartSize *= 0.025f;
	GunShotPE.EndSize = { 35,35 };
	GunShotPE.EndSize *= 0.025f;
	GunShotPE.Z_Index = 100;
}
void SetupGunRoundPE()
{
	GunRoundPE.material.Texture = noize;
	GunRoundPE.Type = "TEXTURED";
	GunRoundPE.VelocityRandomness = { -25.0,25.0,-25.0,25.0 };
	GunRoundPE.VelocityRandomness *= 0.025f;
	GunRoundPE.acceleration = { 0, 0 };
	GunRoundPE.StartColor = { 1.0f,0.6f,0.1f,1.0f };
	GunRoundPE.EndColor = { 1.0f,0.6f,0.1f,0.0f };
	GunRoundPE.OrbitalVelocityRandomness = 0.0f;
	GunRoundPE.lighted = true;
	GunRoundPE.RotationRandomness = 10.0f;
	GunRoundPE.InitialRotation = 10.0f;
	GunRoundPE.lifetime = 5.0f;
	GunRoundPE.VelocityDamper = 0.5f;
	GunRoundPE.Additive = false;
	GunRoundPE.material.NormalMap = CubeNormalMapTexture;
	GunRoundPE.DrawToNormalMap = true;
	GunRoundPE.StartSize = { 7,4 };
	GunRoundPE.StartSize *= 0.025f;
	GunRoundPE.EndSize = { 7,4 };
	GunRoundPE.EndSize *= 0.025f;
	GunRoundPE.material =PrimitiveQuad;
	GunRoundPE.Z_Index = 100;
}
void SetupCollisionSmokePE()
{
	CollisionSmoke.material.Texture = noize;
	CollisionSmoke.Type = "TEXTURED";
	CollisionSmoke.VelocityRandomness = { -70,70,-70,70 };
	CollisionSmoke.VelocityRandomness *= 0.025f;
	CollisionSmoke.StartColor = { 3.0f,3.0f,3.0f,3.0f };
	CollisionSmoke.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	CollisionSmoke.OrbitalVelocityRandomness = 1.5f;
	CollisionSmoke.lighted = true;
	CollisionSmoke.RotationRandomness = 10.0f;
	CollisionSmoke.InitialRotation = 10.0f;
	CollisionSmoke.lifetimeRandomness = 1.0f;
	CollisionSmoke.lifetime = 1.5f;
	CollisionSmoke.Z_Index = 1000;
	CollisionSmoke.VelocityDamper = 0.2f * 60;
	CollisionSmoke.StartSize = { 20.0f,20.0f };
	CollisionSmoke.StartSize *= 0.025f;
	CollisionSmoke.EndSize = { 150.0f,150.f };
	CollisionSmoke.EndSize *= 0.025f;
	CollisionSmoke.Additive = true;
}
void SetupCollisionSparksPE()
{

	CollisionSparks.Type = "TEXTUREDLINE";
	CollisionSparks.VelocityRandomness = { -150,150,-150,150 };
	CollisionSparks.VelocityRandomness *= 0.025f;
	CollisionSparks.acceleration = { 0.0f,0.0f };
	CollisionSparks.StartColor = { 30.0f,10.0f,0.0f,1.0f };
	CollisionSparks.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	CollisionSparks.lighted = true;
	CollisionSparks.lifetimeRandomness = 0.2f;
	CollisionSparks.lifetime = 0.5f;
	CollisionSparks.VelocityDamper = 0.0f;
	CollisionSparks.Z_Index = 1000;
	CollisionSparks.material =PrimitiveQuad;

	CollisionSparks.StartSize = { 2 * 0.025f,2 };
	CollisionSparks.EndSize = { 2 * 0.025f,2 };
}
void SetupExplodionPE()
{

	ExplodionPE.material.Texture = noize;
	ExplodionPE.Type = "TEXTURED";
	ExplodionPE.VelocityRandomness = { -150,150,-150,150 };
	ExplodionPE.VelocityRandomness *= 0.025f;
	ExplodionPE.acceleration = { 0.0f,0.0f };
	ExplodionPE.StartColor = { 1.0f,1.0f,1.0f,10.0f };
	ExplodionPE.EndColor = { 1.0f,1.0f,1.0f,1.0f };
	ExplodionPE.lighted = true;
	ExplodionPE.lifetimeRandomness = 0.3f;
	ExplodionPE.lifetime = 3.5f;
	ExplodionPE.Z_Index = 100;
	ExplodionPE.Additive = true;
	ExplodionPE.VelocityDamper = 4.0f;

	ExplodionPE.StartSize = { 15 ,15 };
	ExplodionPE.EndSize = { 25 ,25 };
}
void SetupDebriePE()
{
	DebrieParticles.Type = "TEXTURED";
	DebrieParticles.VelocityRandomness = { -250.0,250.0,-250.0,250.0 };
	DebrieParticles.VelocityRandomness *= 0.025f;
	DebrieParticles.acceleration = { 0, 0 };
	DebrieParticles.StartColor = { 1.0f,1.0f,1.0f,1.0f };
	DebrieParticles.EndColor = { 1.0f,1.0f,1.0f,0.0f };
	DebrieParticles.OrbitalVelocityRandomness = 0.0f;
	DebrieParticles.RotationRandomness = 10.0f;
	DebrieParticles.RotationVelocity = 10.0f;
	DebrieParticles.InitialRotation = 10.0f;
	DebrieParticles.lifetime = 40.0f;
	DebrieParticles.VelocityDamper = 0.75f;
	DebrieParticles.Additive = false;
	DebrieParticles.material.NormalMap = NULL;
	DebrieParticles.material.Texture = Debrie0Texture;
	//DebrieParticles.textures.push_back(Debrie1Texture);
	DebrieParticles.DrawToNormalMap = false;
	DebrieParticles.StartSize = { 25,25};
	DebrieParticles.StartSize *= 0.025f;
	DebrieParticles.EndSize = { 25,25 };
	DebrieParticles.EndSize *= 0.025f;
	DebrieParticles.Z_Index = 100;
}
void SetupforegroundFogPE()
{
	foregroundFog.material.Texture = noize;
	foregroundFog.Type = "TEXTURED";
	foregroundFog.VelocityRandomness = {0.0f,0.0f,0.0f,0.0f};//{ 0,700,0,700 };
	foregroundFog.InitialVelocity =    {0.0f,0.0f};//{ -350,-350};
	foregroundFog.StartColor = { 10.0f,10.0f,10.0f,10.0f };
	foregroundFog.EndColor = { 10.0f,10.0f,10.0f,0.0f };
	foregroundFog.OrbitalVelocityRandomness = 1.5f;
	foregroundFog.RotationRandomness = 10.0f;
	foregroundFog.InitialRotation = 10.0f;
	foregroundFog.lifetimeRandomness = 0.0f;
	foregroundFog.lifetime = 16.5f;
	foregroundFog.Z_Index = 100;
	foregroundFog.VelocityDamper = 0.05f * 60;
	foregroundFog.StartSize = { 0.0f,0.0f };
	foregroundFog.EndSize = { 350.0f,350.f};
	foregroundFog.Additive = false;
	foregroundFog.influenced = true;
	foregroundFog.lighted = true;
	foregroundFog.ShowWindow = false;
	foregroundFog.DrawToNormalMap = false;
	foregroundFog.ShowWindow = true;

}

void SetupPEs()
{
	
	SetupLaserPE();
	SetupEnginePE();
	SetupSmokePE();
	SetupSparksPE();
	SetupBulletPE();
	SetupGunPE();
	SetupCollisionSparksPE();
	SetupCollisionSmokePE();
	SetupExplodionPE();
	SetupGunRoundPE();
	SetupDebriePE();
	SetupforegroundFogPE();

	emiters.push_back(&EngineSmoke);
	emiters.push_back(&Sparks);
	emiters.push_back(&bulletFlightPm);
	emiters.push_back(&bulletColisionPm);
	emiters.push_back(&LaserPm);
	emiters.push_back(&LaserShotPm);
	emiters.push_back(&Smoke);
	emiters.push_back(&GunShotPE);
	emiters.push_back(&GunRoundPE);
	emiters.push_back(&CollisionSparks);
	emiters.push_back(&CollisionSmoke);
	emiters.push_back(&ExplodionPE);
	emiters.push_back(&DebrieParticles);
	
	for (int i = 0; i < emiters.size(); i++)
	{
		emiters[i]->influenced = true;
		emiters[i]->lighted = true;
		emiters[i]->ShowWindow = false;
	}
}
void clearParticleMaterials()
{
	for (int i = 0; i < emiters.size(); i++)
	{
		emiters[i]->Particles.clear();
	}
	
}

void AddSphereOfInfluence(glm::vec2 position, float r, glm::vec2 velocity, bool attractive, float attractiveStrehgth)
{
	for (int i = 0; i < emiters.size(); i++)
		emiters[i]->AddSpheresOfInfluence(position, r, velocity, attractive, attractiveStrehgth);

	foregroundFog.AddSpheresOfInfluence(position + ActiveRadar.offset, r*1.5f, velocity * 1.5f, attractive, attractiveStrehgth * 2.5f);

}
void AddLightSphere(glm::vec2 position, float r, glm::vec4 color)
{
	for (int i = 0; i < emiters.size(); i++)
		emiters[i]->AddLightSphere(position, r,color);

	foregroundFog.AddLightSphere(position + ActiveRadar.offset, r*1.5f, color * 0.8f);

}
void ProcessPE(float dt)
{
	EngineSmoke.lifetime = 0.2f;
	for (int i = 0; i < emiters.size(); i++)
	{
		emiters[i]->ShowWindow = false;
		emiters[i]->Process(dt);
		emiters[i]->SpheresOfInfluence.clear();
		emiters[i]->LightSpheres.clear();
	}

}
