#pragma once

#include "Parts/smallball.h"
#include "Parts/Base.h"
#include "Parts/CentralPart.h"


// guns
class Gun : public BodyComponent
{
public:
	//HEAT related stuff
	float HeatPerShot = 1.0f;
	float BulletHeat = 1.0f;




	float TargetAngle = 0.0f;

	float speed = 500.0f;

	bool overheated = false;

	float RotationalFriction = 50.5f;

	float bulletSpeed = 150.0f;
	float dmg = 0.2f;
	float bulletSize = 50.0f;
	float recoil = 600.0f * 0.025f;

	float t = 1.0f;
	float shootspeed = 0.2f;



	glm::vec2 prevtrgpos = { 0.0f,0.0f };

	~Gun() override
	{
		DeleteBody();
	}
	Gun()
	{
		partid = PART::GUN;
		parttype = TYPE::WEAPON;
		type = partid + NodeType::LASTNODE;
		Name = "Gun";

		CreateBody(3,1,0,1);
		ballPosition[body[0]]= position;
		ballPosition[body[1]]= position + glm::vec2(0.0f,1.0f) * 3.0f;
		ballPosition[body[2]]= position + glm::vec2(0.0f,1.0f) * 3.0f;
		deactivated = true;



		Health = PartsData.GetPropertyAsFloat("Gun", "Health");
		maxHealth = Health;

		HeatPerShot = PartsData.GetPropertyAsFloat("Gun", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("Gun", "Damage");
		BulletHeat = PartsData.GetPropertyAsFloat("Gun", "BulletHeat");
		bulletSpeed = PartsData.GetPropertyAsFloat("Gun", "BulletSpeed");
		shootspeed = PartsData.GetPropertyAsFloat("Gun", "FireRate");
		recoil = PartsData.GetPropertyAsFloat("Gun", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("Gun", "BulletSize");
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(2);
		shutdownTemp = 15.0f;
		OnPartCreate();
	}

	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{



		ballPosition[body[0]]= position;
		ballPosition[body[1]]= position + direction * 3.0f;
		ballPosition[body[2]]= position + direction * 3.0f;
		deactivated = true;

		Health = PartsData.GetPropertyAsFloat("Gun", "Health");
		maxHealth = Health;

		HeatPerShot = PartsData.GetPropertyAsFloat("Gun", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("Gun", "Damage");
		BulletHeat = PartsData.GetPropertyAsFloat("Gun", "BulletHeat");
		bulletSpeed = PartsData.GetPropertyAsFloat("Gun", "BulletSpeed");
		shootspeed = PartsData.GetPropertyAsFloat("Gun", "FireRate");
		recoil = PartsData.GetPropertyAsFloat("Gun", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("Gun", "BulletSize");
		shutdownTemp = 15.0f;
		Cost.Matter = 20;
		
		ProcessConnections();
	}
	void ProcessConnections()
	{

		bDataConnections[0].name = "Shoot";
		vDataConnections[0].name = "Target";

		vDataConnections[0].source = false;
		bDataConnections[0].source = false;
	}
	void MTProcess(float dt) override
	{
		ProcessConnections();
		ProcessBody(dt);
		


		mid = (ballPosition[body[0]] + ballPosition[body[2]]) * 0.5f;
		if (ballTemp[body[1]] >= shutdownTemp)
			overheated = true;
		if (ballTemp[body[1]] <= 0.25f)
			overheated = false;
		shot = bDataConnections[0].data && bDataConnections[0].connected;
		if (!debris && !overheated)
		{
			targetrotpoint = vDataConnections[0].data;
			if(sqrlength(targetrotpoint) < 2.0f) // its normalized
				targetrotpoint = mid + targetrotpoint;



			if (!deactivated)
			{
				targetrotpoint = vDataConnections[0].data;
				glm::vec2 dif = ballPosition[body[1]] - ballPosition[body[0]];
				glm::vec2 trgvel = (targetrotpoint - prevtrgpos) * (1.0f / dt);
				glm::vec2 trgvec = targetrotpoint - ballPosition[body[0]];
				glm::vec2 rotvec = Normalize(glm::vec2(-dif.y, dif.x));

				float D = length(trgvec);
				float T = D / bulletSpeed;
				float lastT = 0;
				for (int i = 0; i < 3; i++)
				{
					targetrotpoint += trgvel * (T - lastT);
					lastT = T;
					trgvec = targetrotpoint - ballPosition[body[0]];
					D = length(trgvec);
					T = D / bulletSpeed;
				}
				trgvec = trgvec / D;
				ballVelocity[body[1]] -= dt * RotationalFriction * DOT(ballVelocity[body[1]] - ballVelocity[body[0]], rotvec) * rotvec;

				ballForce[body[1]] += speed * rotvec * DOT(rotvec, trgvec);
				prevtrgpos = vDataConnections[0].data;
				if(sqrlength(vDataConnections[0].data) < 2.0f) // its normalized
					prevtrgpos = mid + vDataConnections[0].data * 10.0f;
			}

		}
		glm::vec2 posdif = ballPosition[body[1]] - ballPosition[body[0]];

		glm::vec2 norm = Normalize(posdif);

		glm::vec2 Difference = posdif - norm * PARTSIZE * 3.0f;



		ballPosition[body[0]] += Difference * 0.5f;
		ballPosition[body[1]] += -Difference * 0.5f;

		glm::vec2  velbuf = ballVelocity[body[0]];

		ballVelocity[body[1]]-= DOT(ballVelocity[body[1]]- velbuf, norm) * norm;

		Strut(body[0], body[2], PARTSIZE * 2.0f);


		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;
		ballPosition[body[2]] += ballVelocity[body[2]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;
		ballVelocity[body[2]] += ballForce[body[2]] * dt;


		ballForce[body[1]] = { 0.0f,0.0f };
		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[2]] = { 0.0f,0.0f };

		float change = ((ballTemp[body[0]]+ ballTemp[body[2]])*0.5f- ballTemp[body[1]]);

		ballTemp[body[0]]-= change*0.5f;
		ballTemp[body[2]]-= change*0.5f;
		ballTemp[body[1]]+= change;
	}
	void Process(float dt) override
	{


		if (!debris && !overheated)
		{
			t -= dt;
			if (shot && t <= 0)
				Shoot();
		}
	}

	void Shoot()
	{
		if (ballTemp[body[1]] < shutdownTemp)
		{
			t= shootspeed;

			glm::vec2 dir = Normalize(ballPosition[body[1]]- ballPosition[body[0]]);

			for (int i = 0; i < 15; i++)
				GunShotPE.Spawn(ballPosition[body[1]]+ dir * PARTSIZE * -1.0f + dir * (rand() % 1000 * 0.002f * PARTSIZE), dir * 500.0f * 0.025f, 1);

			for (int i = 0; i < 10; i++)
				GunShotPE.Spawn(ballPosition[body[1]]+ dir * PARTSIZE * -0.9f + Rotate(dir * (rand() % 1000 * 0.0015f * PARTSIZE), -pi * 0.7f), Rotate(dir * 500.0f, -pi * 0.7f) * 0.025f, 1);
			for (int i = 0; i < 10; i++)
				GunShotPE.Spawn(ballPosition[body[1]]+ dir * PARTSIZE * -0.8f + Rotate(dir * (rand() % 1000 * 0.0015f * PARTSIZE), pi * 0.7f), Rotate(dir * 500.0f, pi * 0.7f) * 0.025f, 1);
			
			SpawnBullet(ballPosition[body[1]], bulletSpeed * Normalize(ballPosition[body[1]]- ballPosition[body[0]]), dmg, PARTSIZE * 0.5f, BulletHeat, recoil, id);




			playsound(GunSound, ballPosition[body[1]], freq <= 0.001f ? 0.0f : 1.0f, (1.0f + rand() % 10 * 0.04f - 0.2f) * freq, ballVelocity[body[0]],true);

			ScreenShake += PARTSIZE * 0.2f * bulletSpeed * 0.000001f;
			ChromaticAbberation += PARTSIZE * 0.2f * bulletSpeed * 0.000001f;

			ballForce[body[0]] -= recoil * Normalize(ballPosition[body[1]] - ballPosition[body[0]]) * 2000.0f;

			//body[1].Force -= recoil * Normalize(body[1].position - body[0].position) * 1000.0f;
		}
	}

	void Draw() override
	{
		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		glm::vec2 mid = (ballPosition[body[1]] + ballPosition[body[0]]) * 0.5f;
		//GunTexture
		DrawTexturedQuad(mid, glm::vec2(0.5f * PARTSIZE, 2.0f * PARTSIZE), GunTexture, get_angle_between_points(ballPosition[body[0]], mid), color, Z_Index + 2, GunNormalMap);
		//DrawTexturedQuad(body[0].position, glm::vec2(body[0].r), GunBaseTexture, 0.0f, color, Z_Index+1, GunBaseNormalMap);
		mid = (ballPosition[body[2]] + ballPosition[body[0]]) * 0.5f;
		DrawTexturedQuad(mid, glm::vec2(1.0f * PARTSIZE, 2.0f * PARTSIZE), GunBaseTexture, get_angle_between_points(ballPosition[body[0]], mid), color, Z_Index, GunBaseNormalMap);

	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawTexturedQuad(ui_position, {size.x*0.25f*0.75f,size.y*0.75f}, GunTexture, 0.0f, color, Z_Index+2, GunNormalMap);
		UI_DrawTexturedQuad(ui_position, {size.x*0.5f,size.y}, GunBaseTexture, 0.0f, color, Z_Index, GunBaseNormalMap);
	};
};

class LaserGun : public BodyComponent
{
	float diaglength = 0.0f;
public:
	//HEAT related stuff
	float HeatPerShot = 10.0f;
	float BulletHeat = 3.0f;



	float TargetAngle = 0.0f;

	float speed = 500.0f;

	float RotationalFriction = 50.5f;

	float bulletSize = 50.0f;
	float dmg = 2.0f;
	float recoil = 600.0f * 0.025f;

	float LaserTime = 1.0f;
	float Time = 0.0f;

	float t = 0.0f;

	Laser* lsr;
	LaserGun()
	{
		
		partid = PART::LASERGUN;
		parttype = TYPE::WEAPON;
		type = partid + NodeType::LASTNODE;
		Name = "LaserGun";
		CreateBody(5, 1, 0, 1);
		float ang = 0.25f * pi;
		ballPosition[body[0]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[2]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[3]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[4]] = position + Rotate({ 0.0f,1.0f }, ang);

		ballPosition[body[1]] = position + glm::vec2(0.0f, 1.0f) * 3.0f;

		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);

		deactivated = true;
		lsr = new Laser(ballPosition[body[1]], ballPosition[body[1]] - ballPosition[body[0]],150,175);
		lsr->inf = true;

		lsr->time = 0;
		lsr->maxtime = 1;
		lsr->width = PARTSIZE * 0.75f;
		lsr->body.Damage = 0;
		lsr->body.Heat = 0;
		lsr->body.recoil = 0;
		lsr->body.singleHit = false;
		lsr->fraction = ballID[body[0]];


		Health = PartsData.GetPropertyAsFloat("LaserGun", "Health");
		maxHealth = Health;
		HeatPerShot = PartsData.GetPropertyAsFloat("LaserGun", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("LaserGun", "Damage");
		BulletHeat = PartsData.GetPropertyAsFloat("LaserGun", "LaserHeat");
		recoil = PartsData.GetPropertyAsFloat("LaserGun", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("LaserGun", "BulletSize");



		BodyIdsWithCollision.clear();
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(2);
		BodyIdsWithCollision.push_back(3);
		BodyIdsWithCollision.push_back(4);

		ProcessConnections();
		OnPartCreate();
	}
	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{
		float ang = 0.25f * pi;
		ballPosition[body[0]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[2]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[3]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[4]] = position + Rotate({ 0.0f,1.0f }, ang);

		ballPosition[body[1]] = position + glm::vec2(0.0f, 1.0f) * 3.0f;

		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);

		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);

		deactivated = true;
		ballPosition[body[1]] = position + direction*3.0f;
		lsr = new Laser(ballPosition[body[1]], ballPosition[body[1]]- ballPosition[body[0]],150,175);
		lsr->inf = true;

		lsr->time = 0;
		lsr->maxtime = 1;
		lsr->width = size * 0.75f;
		lsr->body.Damage = 0;
		lsr->body.Heat = 0;
		lsr->body.recoil = 0;
		lsr->body.singleHit = false;
		lsr->fraction = ballID[body[0]];

		Cost.Matter = 50;

		Health = PartsData.GetPropertyAsFloat("LaserGun", "Health");
		maxHealth = Health;
		HeatPerShot = PartsData.GetPropertyAsFloat("LaserGun", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("LaserGun", "Damage");
		BulletHeat = PartsData.GetPropertyAsFloat("LaserGun", "LaserHeat");
		recoil = PartsData.GetPropertyAsFloat("LaserGun", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("LaserGun", "BulletSize");



		BodyIdsWithCollision.clear();
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(2);
		BodyIdsWithCollision.push_back(3);
		BodyIdsWithCollision.push_back(4);

		ProcessConnections();
	}
	void ProcessConnections()
	{
		bDataConnections[0].name = "Shoot";
		vDataConnections[0].name = "Target";
		vDataConnections[0].source = false;
		bDataConnections[0].source = false;

	}
	void MTProcess(float dt) override
	{

		ProcessConnections();
		ProcessBody(dt);
		
		if (!debris)
		{

			lsr->time = t;
			lsr->maxtime = 1.0f;
			lsr->body.Damage = t * dmg * dt;
			lsr->body.Heat = t * HeatPerShot * dt;
			lsr->body.recoil = t * recoil * dt;
			lsr->body.body.r = 2.0f;
			ChromaticAbberation += PARTSIZE * recoil * 0.00001f * t;
			glm::vec2 rec = t * recoil * Normalize(ballPosition[body[1]]- ballPosition[body[0]]) * 1000.1f * dt * 0.25f;

			ballForce[body[0]] -= rec;
			ballForce[body[2]] -= rec;
			ballForce[body[3]] -= rec;
			ballForce[body[4]] -= rec;
			ballForce[body[1]] -= t * recoil * Normalize(ballPosition[body[1]]- ballPosition[body[0]]) * 1000.1f * dt;




			if (!deactivated && vDataConnections[0].connected)
			{

				targetrotpoint = vDataConnections[0].data;
				if(sqrlength(targetrotpoint) < 2.0f) // its normalized
					targetrotpoint = mid + targetrotpoint;
				glm::vec2 dif = ballPosition[body[1]]- mid;
				glm::vec2 trgvec = targetrotpoint - mid;
				glm::vec2 rotvec = Normalize(glm::vec2(-dif.y, dif.x));

				float D = length(trgvec);
				trgvec = trgvec / D;
				ballVelocity[body[1]]-= dt * RotationalFriction * DOT(ballVelocity[body[1]]- ballVelocity[body[0]], rotvec) * rotvec;

				ballForce[body[1]] += speed * rotvec * DOT(rotvec, trgvec);
			}

		}
		mid = (ballPosition[body[0]]+ ballPosition[body[2]]+ ballPosition[body[3]]+ ballPosition[body[4]]) * 0.25f;

		glm::vec2 posdif = ballPosition[body[1]]- mid;

		glm::vec2 norm = Normalize(posdif);

		lsr->RayCast.direction = norm;
		lsr->RayCast.position = ballPosition[body[1]];

		glm::vec2 Difference = posdif - norm * PARTSIZE * 3.0f;



		ballPosition[body[0]]+= Difference * 0.5f * 0.25f;
		ballPosition[body[2]]+= Difference * 0.5f * 0.25f;
		ballPosition[body[3]]+= Difference * 0.5f * 0.25f;
		ballPosition[body[4]]+= Difference * 0.5f * 0.25f;
		ballPosition[body[1]]+= -Difference * 0.5f;

		glm::vec2  velbuf = ballVelocity[body[0]];

		ballVelocity[body[1]] -= DOT(ballVelocity[body[1]]- velbuf, norm) * norm;


		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;
		ballPosition[body[2]] += ballVelocity[body[2]] * dt;
		ballPosition[body[3]] += ballVelocity[body[3]] * dt;
		ballPosition[body[4]] += ballVelocity[body[4]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;
		ballVelocity[body[2]] += ballForce[body[2]] * dt;
		ballVelocity[body[3]] += ballForce[body[3]] * dt;
		ballVelocity[body[4]] += ballForce[body[4]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };
		ballForce[body[2]] = { 0.0f,0.0f };
		ballForce[body[3]] = { 0.0f,0.0f };
		ballForce[body[4]] = { 0.0f,0.0f };


		Strut(body[0], body[2], PARTSIZE*2.0f);
		Strut(body[2], body[3], PARTSIZE*2.0f);
		Strut(body[3], body[4], PARTSIZE*2.0f);
		Strut(body[0], body[4], PARTSIZE*2.0f);

		Strut(body[0], body[3], diaglength);
		Strut(body[4], body[2], diaglength);

		float change = ((ballTemp[body[0]]+ ballTemp[body[2]]+ ballTemp[body[3]]+ ballTemp[body[4]])*0.25f - ballTemp[body[1]]);

		ballTemp[body[0]]-= change * 0.25f;
		ballTemp[body[2]]-= change * 0.25f;
		ballTemp[body[3]]-= change * 0.25f;
		ballTemp[body[4]]-= change * 0.25f;
		ballTemp[body[1]]+= change;

	}
	void Process(float dt) override
	{


		if (!debris)
		{

			shot = bDataConnections[0].data && bDataConnections[0].connected;
			targetrotpoint = vDataConnections[0].data;

			lsr->fraction = ballID[body[0]];


			lsr->body.id = id;


			Time -= dt;
			if (Time < 0.0f) Time = 0.0f;




			if (shot && ballTemp[body[1]] < shutdownTemp)
				t += dt;
			else
				t -= dt;


			if (t > 1.0f)
				t = 1.0f;
			if (t < 0.0f)
				t = 0.0f;

			if (t > 0.0f)
			{			
				playsound(GunSound, ballPosition[body[1]], freq <= 0.001f ? 0.0f : 1.0f, t * freq, ballVelocity[body[0]]);

			}
			ballTemp[body[1]] += t * HeatPerShot * dt;

			ScreenShake += PARTSIZE * recoil * 0.000021f * t;




			if (t > 0.0f)
				LaserPtrs.push_back(lsr);

			

		}
	
	}


	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		mid = (ballPosition[body[0]] + ballPosition[body[2]] + ballPosition[body[3]] + ballPosition[body[4]]) * 0.25f;
		DrawTexturedQuad(mid, glm::vec2(0.5f * PARTSIZE * 0.2f, 4.0f * PARTSIZE * 0.2f), LaserGunTexture, get_angle_between_points(ballPosition[body[1]], mid), color, Z_Index + 2, LaserGunNormalMap);
		DrawTexturedQuad(mid, glm::vec2(PARTSIZE * 2.0f), LaserGunBaseTexture, get_angle_between_points(ballPosition[body[0]],mid) - pi*0.25f, color, Z_Index + 1, LaserGunBaseNormalMap);
	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
		lsr->~Laser();
		free(lsr);
	}
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawTexturedQuad(ui_position, {size.x*0.125f*0.75f,size.y*0.75f}, LaserGunTexture, 0.0f, color, Z_Index+2, LaserGunNormalMap);
		UI_DrawTexturedQuad(ui_position, {size.x,size.y}, LaserGunBaseTexture, 0.0f, color, Z_Index, LaserGunBaseNormalMap);
	};
};

class RocketLauncher : public BodyComponent
{
public:



	float TargetAngle = 0.0f;

	float speed = 500.0f;


	float RotationalFriction = 50.5f;

	float bulletSpeed = 150.0f;
	float dmg = 0.02f;
	float bulletSize = 50.0f;
	float recoil = 600.0f * 0.025f;
	float ExplodionForce = 0.0f;
	float t = 1.0f;
	float shootspeed = 2.0f;
	std::vector<Rocket*> FiredRockets;
	Rocket HoldingRocket;
	RocketLauncher()
	{
		
		partid = PART::ROCKETLAUNCHER;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "RocketLauncher";
		
		Health = PartsData.GetPropertyAsFloat("RocketLauncher", "Health");
		maxHealth = Health;
		//HeatPerShot = PartsData.GetPropertyAsFloat("RocketLauncher", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("RocketLauncher", "Damage");
		ExplodionForce = PartsData.GetPropertyAsFloat("RocketLauncher", "ExlodionForce");
		//BulletHeat = PartsData.GetPropertyAsFloat("RocketLauncher", "RocketHeat");
		bulletSpeed = PartsData.GetPropertyAsFloat("RocketLauncher", "BulletSpeed");
		shootspeed = PartsData.GetPropertyAsFloat("RocketLauncher", "FireRate");
		recoil = PartsData.GetPropertyAsFloat("RocketLauncher", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("RocketLauncher", "BulletSize");

		CreateBody(2, 1, 0, 1);
		ballPosition[body[0]] = position;
		ballPosition[body[1]] = position + glm::vec2(0.0f, 1.0f) * 3.0f;
		deactivated = true;

		ProcessConnections();


		HoldingRocket.DS.Damage = dmg;
		HoldingRocket.DS.singleHit = true;

		HoldingRocket.body[0].position = ballPosition[body[1]];
		HoldingRocket.body[1].position = ballPosition[body[1]] + dir * 3.0f * PARTSIZE * 0.5f;
		HoldingRocket.body[0].r = PARTSIZE * 0.4f;
		HoldingRocket.body[1].r = PARTSIZE * 0.4f;
		HoldingRocket.fired = false;
		HoldingRocket.dir = dir;
		HoldingRocket.timeLeft = 10.0f;
		HoldingRocket.ExplodionForce = ExplodionForce;
		HoldingRocket.lifet = 10.0f;
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(1);
		OnPartCreate();
	}

	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{

		Health = PartsData.GetPropertyAsFloat("RocketLauncher", "Health");
		maxHealth = Health;
		//HeatPerShot = PartsData.GetPropertyAsFloat("RocketLauncher", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("RocketLauncher", "Damage");
		ExplodionForce = PartsData.GetPropertyAsFloat("RocketLauncher", "ExlodionForce");
		//BulletHeat = PartsData.GetPropertyAsFloat("RocketLauncher", "RocketHeat");
		bulletSpeed = PartsData.GetPropertyAsFloat("RocketLauncher", "BulletSpeed");
		shootspeed = PartsData.GetPropertyAsFloat("RocketLauncher", "FireRate");
		recoil = PartsData.GetPropertyAsFloat("RocketLauncher", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("RocketLauncher", "BulletSize");


		ballPosition[body[0]] = position;
		ballPosition[body[1]] = position + direction * 3.0f;
		deactivated = true;

		ProcessConnections();


		Cost.Matter = 75;



		HoldingRocket.DS.Damage = dmg;
		HoldingRocket.DS.singleHit = true;

		HoldingRocket.body[0].position = ballPosition[body[1]];
		HoldingRocket.body[1].position = ballPosition[body[1]] + dir * 3.0f * size * 0.5f;
		HoldingRocket.body[0].r = size * 0.4f;
		HoldingRocket.body[1].r = size * 0.4f;
		HoldingRocket.fired = false;
		HoldingRocket.dir = dir;
		HoldingRocket.timeLeft = 10.0f;
		HoldingRocket.ExplodionForce = ExplodionForce;
		HoldingRocket.lifet = 10.0f;


	}
	void ProcessConnections()
	{

		bDataConnections[0].name = "Shoot";
		vDataConnections[0].name = "Target";

		vDataConnections[0].source = false;
		bDataConnections[0].source = false;
	}
	
	void MTProcess (float dt) override
	{
		ProcessConnections();
		ProcessBody(dt);
		dir = Normalize(ballPosition[body[1]] - ballPosition[body[0]]);


		HoldingRocket.body[1].position = ballPosition[body[1]];
		HoldingRocket.body[0].position = ballPosition[body[1]] + dir * 2.0f * HoldingRocket.body[1].r;
		if (!debris)
		{
			if (t <= 0)
			{
				HoldingRocket.mid = (HoldingRocket.body[1].position + HoldingRocket.body[0].position) * 0.5f;
				HoldingRocket.midvel = (HoldingRocket.body[0].velocity + HoldingRocket.body[1].velocity) * 0.5f;
				HoldingRocket.dir = dir;
				HoldingRocket.ExplodionForce = ExplodionForce;
				HoldingRocket.body[0].Process(dt);
				HoldingRocket.body[1].Process(dt);
				Strut(&HoldingRocket.body[1], &HoldingRocket.body[0], HoldingRocket.body[1].r * 2.0f);
			}
		
			
			
			int iter = 0;
			while (iter < FiredRockets.size())
			{
				if (FiredRockets[iter]->Exploded)
				{
					FiredRockets[iter] = FiredRockets[FiredRockets.size() - 1];
					FiredRockets.pop_back();
				}
				else
					iter++;
			}
		}
		Strut(body[0], body[1], PARTSIZE * 2.0f);
		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };

		float change = (ballTemp[body[0]] - ballTemp[body[1]]);

		ballTemp[body[0]] -= change;
		ballTemp[body[1]] += change;
	}
	void Process(float dt) override
	{
		
		if (!debris)
		{
		
			
			
			shot = bDataConnections[0].data && bDataConnections[0].connected;
			targetrotpoint = vDataConnections[0].data;
		
			if(sqrlength(targetrotpoint) < 2.0f) // its normalized
				targetrotpoint = mid + targetrotpoint;
			t -= dt;
			if (shot && t <= 0 && FiredRockets.size()<2)
				Shoot();
			for (int i = 0; i < FiredRockets.size(); i++)
			{
				FiredRockets[i]->target = targetrotpoint;
				FiredRockets[i]->fired = true;
				FiredRockets[i]->DS.id = id;
				Rockets.push_back(FiredRockets[i]);
				if (FiredRockets[i]->timeLeft <= 0.0f)
					FiredRockets[i]->Explode();
			}

		}
		else
		{
			for (int i = 0; i < FiredRockets.size(); i++)
			{
				FiredRockets[i]->Explode();
			}
			FiredRockets.clear();
		}

	}
	void Shoot()
	{
		t = shootspeed;


		for (int i = 0; i < 15; i++)
			GunShotPE.Spawn(ballPosition[body[1]] + dir * PARTSIZE * -1.0f + dir * (rand() % 1000 * 0.002f * PARTSIZE),
				dir * 500.0f * 0.025f, 1);

		Rocket* R = new Rocket();

		R->Init(HoldingRocket.body[0].position, HoldingRocket.dir, HoldingRocket.body[0].r, dmg, 10.0f);
		R->fired = true;
		R->body[0].velocity = dir * 100.0f + ballVelocity[body[1]];
		R->body[1].velocity = dir * 100.0f + ballVelocity[body[1]];
		R->ExplodionForce = ExplodionForce;
		FiredRockets.push_back(R);
		/*HoldingRocket.body[0].velocity = dir*1.0f;
		HoldingRocket.body[1].velocity = dir*1.0f;
		HoldingRocket.DS.enemy = enemy;
		HoldingRocket.DeleteSrc();*/



		//SpawnBullet(body[1].position, bulletSpeed * Normalize(body[1].position - body[0].position), dmg, body[1].r * 0.5f, BulletHeat, recoil, enemy);
		
		//Position 
		//Velocity 

		playsound(GunSound, ballPosition[body[1]], freq <= 0.001f ? 0.0f : 1.0f, (1.0f + rand() % 10 * 0.04f - 0.2f) * freq, ballVelocity[body[0]], true);

		ScreenShake += PARTSIZE * bulletSpeed * 0.000001f;
		ChromaticAbberation += PARTSIZE * bulletSpeed * 0.000001f;

		ballForce[body[0]] -= recoil * Normalize(ballPosition[body[1]] - ballPosition[body[0]]) * 2000.0f;

		//body[1].Force -= recoil * Normalize(body[1].position - body[0].position) * 1000.0f;

	}

	void Draw() override
	{
		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		//DrawLine(body[0].position, body[1].position, body[1].r, Base.color, true, CubeNormalMapTexture, Z_Index+1);
		for (int i = 0; i < FiredRockets.size(); i++)
		{
			FiredRockets[i]->Draw();
		}
		mid = (ballPosition[body[1]] + ballPosition[body[0]]) * 0.5f;
		//GunTexture
		DrawTexturedQuad(mid, glm::vec2(1.0f * PARTSIZE, 2.0f * PARTSIZE), RocketLauncherTexture, get_angle_between_points(ballPosition[body[0]], mid), color, Z_Index, RocketLauncherNormalMap);
		//DrawTexturedQuad(mid, glm::vec2(1.0f * body[1].r, 2.0f * body[1].r), RocketEngineTexture, get_angle_between_points(body[0].position, mid), color, Z_Index, RocketEngineNormalMap);

		//DrawCircle(body[0], glm::vec4(1.0f), true, BallNormalMapTexture, Z_Index);
		if (t <= 0)
			HoldingRocket.Draw();
	}

	void DeletePart() override
	{
		Delete = true;
		for (int i = 0; i < FiredRockets.size(); i++)
		{
			FiredRockets[i]->Explode();
		}
		FiredRockets.clear();
		DeleteBody();
	}

	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawTexturedQuad(ui_position, {size.x*0.5f,size.y}, RocketLauncherTexture, 0.0f, color, Z_Index+2, RocketLauncherNormalMap);
	};
};

class MiniGun : public BodyComponent
{
	float diaglength = 0.0f;
public:
	//HEAT related stuff
	float HeatPerShot = 0.8f;
	float BulletHeat = 0.5f;


	glm::vec2 prevtrgpos = {0.0f,0.0f};
	float speed = 500.0f;


	float RotationalFriction = 50.5f;

	float bulletSpeed = 1000.0f;
	float dmg = 0.04f;
	float bulletSize = 50.0f;
	float recoil = 60.0f * 0.025f;

	float t = 0.0f;
	float shootspeed = 0.034f;

	bool overheated = false;

	float RotationSpeed = 0.0f;
	float BarrelRotation = 0.0f;	
	~MiniGun() override
	{
		DeleteBody();
	}
	MiniGun()
	{
		
		partid = PART::MINIGUN;
		parttype = TYPE::WEAPON;
		type = partid + NodeType::LASTNODE;
		Name = "MiniGun";
		
		Health = PartsData.GetPropertyAsFloat("MiniGun", "Health");
		maxHealth = Health;
		HeatPerShot = PartsData.GetPropertyAsFloat("MiniGun", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("MiniGun", "Damage");
		BulletHeat = PartsData.GetPropertyAsFloat("MiniGun", "BulletHeat");
		bulletSpeed = PartsData.GetPropertyAsFloat("MiniGun", "BulletSpeed");
		shootspeed = PartsData.GetPropertyAsFloat("MiniGun", "FireRate");
		recoil = PartsData.GetPropertyAsFloat("MiniGun", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("MiniGun", "BulletSize");

		CreateBody(5, 1, 0, 1);
		float ang = 0.25f * pi;
		ballPosition[body[0]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[1]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[2]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[3]] = position + Rotate({ 0.0f,1.0f }, ang);

		ballPosition[body[4]] = position + glm::vec2(0.0f, 1.0f) * 3.0f;

		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);

		deactivated = true;
		Cost.Matter = 75;

		shutdownTemp = 15.0f;
		ProcessConnections();
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(1);
		BodyIdsWithCollision.push_back(2);
		BodyIdsWithCollision.push_back(3);
		OnPartCreate();
	}
	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{


		Health = PartsData.GetPropertyAsFloat("MiniGun", "Health");
		maxHealth = Health;
		HeatPerShot = PartsData.GetPropertyAsFloat("MiniGun", "HeatPerShot");
		dmg = PartsData.GetPropertyAsFloat("MiniGun", "Damage");
		BulletHeat = PartsData.GetPropertyAsFloat("MiniGun", "BulletHeat");
		bulletSpeed = PartsData.GetPropertyAsFloat("MiniGun", "BulletSpeed");
		shootspeed = PartsData.GetPropertyAsFloat("MiniGun", "FireRate");
		recoil = PartsData.GetPropertyAsFloat("MiniGun", "Recoil");
		bulletSize = PartsData.GetPropertyAsFloat("MiniGun", "BulletSize");

		float ang = 0.25f * pi;
		ballPosition[body[0]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[1]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[2]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[3]] = position + Rotate({ 0.0f,1.0f }, ang);

		ballPosition[body[4]] = position + glm::vec2(0.0f, 1.0f) * 3.0f;

		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);

		deactivated = true;

		shutdownTemp = 15.0f;
		ProcessConnections();


		//PlaySource(&source);
	}
	void ProcessConnections()
	{
		bDataConnections[0].name = "Shoot";
		vDataConnections[0].name = "Target";

		vDataConnections[0].source = false;
		bDataConnections[0].source = false;
	}
	
	void MTProcess(float dt) override
	{
		ProcessConnections();
		ProcessBody(dt);
		if(bDataConnections[0].connected)
			shot = bDataConnections[0].data;
		else
			shot = false;

		if (ballTemp[body[1]] >= shutdownTemp)
			overheated = true;
		if (ballTemp[body[1]] <= 0.25f)
			overheated = false;
		mid = (ballPosition[body[0]] + ballPosition[body[1]] + ballPosition[body[2]] + ballPosition[body[3]]) * 0.25f;
		glm::vec2 avgvel = (ballVelocity[body[0]] + ballVelocity[body[1]] + ballVelocity[body[2]] + ballVelocity[body[3]]) * 0.25f;
		
		if (!debris && !deactivated && !overheated)
		{
			glm::vec2 targetrotpoint = vDataConnections[0].data;
			if (!glm::isnan(targetrotpoint).x && !glm::isnan(targetrotpoint).y)
			{
				if (sqrlength(targetrotpoint) < 2.0f) // its normalized
					targetrotpoint = mid + targetrotpoint * 10.0f;

				glm::vec2 dif = ballPosition[body[4]] - mid;
				if (dt < 0.0001f) // 10k fps probably unreachable with enough stability, shouldnt cause speed issues 
					dt = 0.0001f;
				glm::vec2 trgvel = (targetrotpoint - prevtrgpos) * (1.0f / dt); // probably 
				glm::vec2 trgvec = targetrotpoint - mid;
				glm::vec2 rotvec = Normalize(glm::vec2(-dif.y, dif.x)); // probably



				float D = length(trgvec);
				if (D < 1.0f)
					D = 1.0f;
				if (bulletSpeed < 1.0f)
					bulletSpeed = 1.0f;
				float T = D / bulletSpeed;
				float lastT = 0;
				for (int i = 0; i < 4; i++)
				{
					targetrotpoint += trgvel * (T - lastT) - avgvel * 1.4f * (T - lastT);
					lastT = T;
					trgvec = targetrotpoint - mid;
					D = length(trgvec);
					if (D < 1.0f)
						D = 1.0f;
					T = D / bulletSpeed;
				}
				if (D < 1.0f)
					D = 1.0f;
				trgvec = trgvec / D;
				ballVelocity[body[4]] -= dt * RotationalFriction * DOT(ballVelocity[body[4]] - ballVelocity[body[0]], rotvec) * rotvec;

				ballForce[body[4]] += speed * rotvec * DOT(rotvec, trgvec);

				prevtrgpos = vDataConnections[0].data;
				if (sqrlength(vDataConnections[0].data) < 2.0f) // its normalized
					prevtrgpos = mid + vDataConnections[0].data * 10.0f;
			}
		}

		BarrelRotation += RotationSpeed * dt;
		RotationSpeed -= 60.0f*dt;
		if (RotationSpeed <= 0.0f)
			RotationSpeed = 0.0f;
		while (BarrelRotation > pi * 2.0f)
			BarrelRotation = 0.0f;
		glm::vec2 posdif = ballPosition[body[4]] - mid;

		float l = length(posdif); 

		if(l>0.0001f)
			dir = posdif/l;
		else 
			dir = {0.0,1.0f};
	
		glm::vec2 Difference = posdif - dir * PARTSIZE * 3.0f;



		ballPosition[body[0]] += Difference * 0.125f;
		ballPosition[body[1]] += Difference * 0.125f;
		ballPosition[body[2]] += Difference * 0.125f;
		ballPosition[body[3]] += Difference * 0.125f;

		ballPosition[body[4]] += -Difference * 0.5f;

		glm::vec2  velbuf = ballVelocity[body[0]];

		ballVelocity[body[4]] -= DOT(ballVelocity[body[4]] - velbuf, dir) * dir;

		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;
		ballPosition[body[2]] += ballVelocity[body[2]] * dt;
		ballPosition[body[3]] += ballVelocity[body[3]] * dt;
		ballPosition[body[4]] += ballVelocity[body[4]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;
		ballVelocity[body[2]] += ballForce[body[2]] * dt;
		ballVelocity[body[3]] += ballForce[body[3]] * dt;
		ballVelocity[body[4]] += ballForce[body[4]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };
		ballForce[body[2]] = { 0.0f,0.0f };
		ballForce[body[3]] = { 0.0f,0.0f };
		ballForce[body[4]] = { 0.0f,0.0f };


		Strut(body[0], body[1], PARTSIZE * 2.0f);
		Strut(body[1], body[2], PARTSIZE * 2.0f);
		Strut(body[2], body[3], PARTSIZE * 2.0f);
		Strut(body[0], body[3], PARTSIZE * 2.0f);

		Strut(body[0], body[2], diaglength);
		Strut(body[3], body[1], diaglength);

	
		float change = ((ballTemp[body[0]]+ ballTemp[body[1]]+ ballTemp[body[2]]+ ballTemp[body[3]]) * 0.25f - ballTemp[body[4]]);

		ballTemp[body[0]]-= change * 0.25f;
		ballTemp[body[1]]-= change * 0.25f;
		ballTemp[body[2]]-= change * 0.25f;
		ballTemp[body[3]]-= change * 0.25f;
		ballTemp[body[4]]+= change;
	}
	void Process(float dt) override
	{
		
		if (!debris && !deactivated && !overheated)
		{
			if (shot)
			{
				playsound(MiniGunSound, ballPosition[body[1]], freq <= 0.001f ? 0.0f : 1.0f, freq + 0.25f + (((rand() % 100) * 0.01f) - 0.5f) * 0.5f + 0.02f * ballTemp[body[4]], ballVelocity[body[0]]);
			}

			t -= dt;
			if (shot && t <= 0)
				Shoot();

		}
	}


	void Shoot()
	{

		if (!deactivated)
		{
			t = shootspeed;
			RotationSpeed = 20.0f;

			dir = Normalize(ballPosition[body[4]]- mid);
			glm::vec2 avgvel = (ballVelocity[body[0]] + ballVelocity[body[1]] + ballVelocity[body[2]] + ballVelocity[body[3]] ) * 0.25f;

			GunRoundPE.Spawn(mid + dir * PARTSIZE * 0.6f,
				Rotate(dir * 500.0f, pi * -0.5f) * 0.025f + ballVelocity[body[0]] , 1);

			GunShotPE.Spawn(ballPosition[body[4]]+ dir * PARTSIZE * -1.0f + dir * (rand() % 1000 * 0.002f * PARTSIZE),
				dir * 500.0f * 0.025f + ballVelocity[body[0]] , 1);

			for (int i = 0; i < 2; i++)
				GunShotPE.Spawn(ballPosition[body[4]]+ dir * PARTSIZE * -0.9f + Rotate(dir * (rand() % 1000 * 0.0015f * PARTSIZE), -pi * 0.7f),
					Rotate(dir * 500.0f, -pi * 0.7f) * 0.025f + ballVelocity[body[0]] , 1);
			for (int i = 0; i < 2; i++)
				GunShotPE.Spawn(ballPosition[body[4]]+ dir * PARTSIZE * -0.8f + Rotate(dir * (rand() % 1000 * 0.0015f * PARTSIZE), pi * 0.7f),
					Rotate(dir * 500.0f, pi * 0.7f) * 0.025f + ballVelocity[body[0]] , 1);
			SpawnBullet(ballPosition[body[4]], bulletSpeed * Normalize(ballPosition[body[4]]- mid) + avgvel, dmg, PARTSIZE * 0.2f, BulletHeat, recoil * 10.0f, id);


			ballTemp[body[4]] += HeatPerShot;
			//PlaySound(&source, &GunSound, body[1].position, (1.0f + rand() % 10 * 0.04f - 0.2f) * freq, freq <= 0.001f ? 0.0f : 1.0f);


			ScreenShake += PARTSIZE * bulletSpeed * 0.00001f;
			ChromaticAbberation += PARTSIZE * bulletSpeed * 0.000075f;

			ballForce[body[0]] -= recoil * Normalize(ballPosition[body[4]] - mid) * 2000.0f;

			//body[1].Force -= recoil * Normalize(body[1].position - body[0].position) * 1000.0f;
		}
	}

	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		//DrawLine(body[0].position, body[1].position, body[1].r, Base.color, true, CubeNormalMapTexture, Z_Index+1);

		glm::vec2 mid2 = (ballPosition[body[4]] + mid) * 0.5f;
		//GunTexture
		DrawTexturedQuad(mid - dir* PARTSIZE*2.0f, glm::vec2(1.5f *PARTSIZE, 4.0f * PARTSIZE), MiniGunTexture, get_angle_between_points(mid2, mid), color, Z_Index + 5, MiniGunNormalMap);
		DrawTexturedQuad(mid, glm::vec2(PARTSIZE * 2.0f), MiniGunBaseTexture, get_angle_between_points(ballPosition[body[0]], ballPosition[body[2]]) + pi*0.25f, color, Z_Index + 1, MiniGunBaseNormalMap,false);

		for (int i = 0; i < 6; i++)
		{
			float angle = i * pi * 0.3333f + BarrelRotation;

			glm::vec2 berrrelPos = Rotate(glm::vec2(1.0f, 0.0f), angle);

			int z = 0;

			if (berrrelPos.y > 0.0f && berrrelPos.y < 0.6f)
				z = 1;
			else
				if (berrrelPos.y >= 0.6)
					z = 2;
			if (berrrelPos.y < 0.0f && berrrelPos.y > -0.6f)
				z = -1;
			else
				if (berrrelPos.y <= -0.6)
					z = -2;

			;
			z += 2;
			DrawTexturedQuad(ballPosition[body[4]] + Rotate(dir, pi * 0.5f) * berrrelPos.x * PARTSIZE * 0.5f *0.6f, { PARTSIZE * 0.5f * 0.35f ,PARTSIZE * 3.8f }, MiniGunPipeTexture, get_angle_between_points({ 0.0f,0.0f }, dir), color, z + Z_Index, MiniGunPipeNormalMap);
		}
	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}

	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		ui_position.y -= size.y*0.3f; 
		size *= 0.5f;
		UI_DrawTexturedQuad(ui_position + glm::vec2(0.0f,size.y*0.5f), {size.x*0.5f,size.y * 1.25f}, MiniGunTexture, 0.0f, color, Z_Index+2, MiniGunNormalMap);
		UI_DrawTexturedQuad(ui_position, {size.x,size.y}, MiniGunBaseTexture, 0.0f, color, Z_Index, MiniGunBaseNormalMap);
		size *= 0.75f;
		for (int i = 0; i < 6; i++)
		{
			float angle = i * pi * 0.3333f + BarrelRotation;
		
			glm::vec2 berrrelPos = Rotate(glm::vec2(1.0f, 0.0f), angle);
		
			int z = 0;
		
			if (berrrelPos.y > 0.0f && berrrelPos.y < 0.6f)
				z = 1;
			else
				if (berrrelPos.y >= 0.6)
					z = 2;
			if (berrrelPos.y < 0.0f && berrrelPos.y > -0.6f)
				z = -1;
			else
				if (berrrelPos.y <= -0.6)
					z = -2;
		
			z += 2;
			UI_DrawTexturedQuad(ui_position + glm::vec2(0.0f,size.y*0.5f * 3.8f) + Rotate({0.0f,1.0f}, pi * 0.5f) * berrrelPos.x * size.y*0.5f*0.75f *0.6f, {size.y*0.5f*0.75f * 0.35f ,size.y*0.5f * 3.8f }, MiniGunPipeTexture, 0.0f, color, z + Z_Index, MiniGunPipeNormalMap);
		}
		
		
	};
};

// propulsion
class RocketEngine : public BodyComponent
{
public:
	float HeatPerSecond = 2.0f;
	float EngineHeat = 1.0f;

	glm::vec2 Force = { 0.0f,0.0f };
	
	glm::vec2 OutForce = glm::vec2(0.0f);


	float t = 0.0f;
	RocketEngine()
	{
		partid = PART::ROCKETENGINE;
		parttype = TYPE::PROPULSION;
		type = partid + NodeType::LASTNODE;
		Name = "RocketEngine";
		
		Health = PartsData.GetPropertyAsFloat("RocketEngine", "Health");
		maxHealth = Health;
		HeatPerSecond = PartsData.GetPropertyAsFloat("RocketEngine", "HeatPerSecond");
		Power = PartsData.GetPropertyAsFloat("RocketEngine", "Power");

		CreateBody(2,2);
		ballPosition[body[0]] = position + Normalize({ 0.0f,1.0f }) * PARTSIZE;
		ballPosition[body[1]] = position - Normalize({ 0.0f,1.0f }) * PARTSIZE;


		Cost.Matter = 15;

		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(1);
		ProcessConnections();
		OnPartCreate();
	}
	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{

		Health = PartsData.GetPropertyAsFloat("RocketEngine", "Health");
		maxHealth = Health;
		HeatPerSecond = PartsData.GetPropertyAsFloat("RocketEngine", "HeatPerSecond");
		Power = PartsData.GetPropertyAsFloat("RocketEngine", "Power");

		ballPosition[body[0]] = position + Normalize(direction) * size;
		ballPosition[body[1]] = position - Normalize(direction) * size;


		ProcessConnections();
	}
	void ProcessConnections()
	{
		bDataConnections[0].name = "Fire override";
		bDataConnections[0].source = false;
		bDataConnections[1].name = "Boost";
		bDataConnections[1].source = false;
	}
	void MTProcess (float dt) override
	{
		ProcessConnections();
		ProcessBody(dt);
		Strut(body[1], body[0],PARTSIZE * 2.0f);


		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };

		dir = Normalize(ballPosition[body[1]] - ballPosition[body[0]]);

		if ((bDataConnections[0].connected && bDataConnections[0].data))
			throtle = 1.0f;
		if (!debris )
		{



			ballForce[body[1]] = Force;
			ballForce[body[0]] = Force;
			OutForce = Power * -dir * throtle;

			if (abs(throtle) > 0.0f && !shutdown && !deactivated)
			{
				
				ballForce[body[1]] += Power * -dir * throtle;
				ballForce[body[0]] += Power * -dir * throtle;


				glm::vec2 dir = Normalize(ballPosition[body[1]] - ballPosition[body[0]]);


				
				ballTemp[body[0]]+= abs(throtle) * HeatPerSecond * dt;
				ballTemp[body[1]]+= abs(throtle) * HeatPerSecond * dt;


			}
		}
		float change = (ballTemp[body[0]]- ballTemp[body[1]]);

		ballTemp[body[0]]-= change;
		ballTemp[body[1]]+= change;

	}
	void Process(float dt) override
	{
		if (!debris )
		{
			if (abs(throtle) > 0.0f && !shutdown && !deactivated)
			{

				if (throtle > 1.0f)
					throtle = 1.0f;

				if (throtle < -1.0f)
					throtle = -1.0f;
			
				if ((bDataConnections[1].connected && bDataConnections[1].data))
				{
					throtle *= 2.5f;
				}
				
				playsound(RocketEngineSound, (ballPosition[body[1]] + ballPosition[body[0]]) * 0.5f, throtle * 0.25f * (freq <= 0.001f ? 0.0f : 1.0f), freq, ballVelocity[body[0]]);


			}
			else
			{
				ballForce[body[1]] = { 0.0f,0.0f };
				ballForce[body[0]] = {0.0f,0.0f};
			}
		}
		else
		{
			ballForce[body[1]] = { 0.0f,0.0f };
			ballForce[body[0]] = {0.0f,0.0f};

		}
	}
	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		if (abs(throtle) > 0.0f && !shutdown)
		{
			float Particleforce = throtle;
			if ((bDataConnections[1].connected && bDataConnections[1].data))
			{
				Particleforce *= 0.65f;
			}
			if (freq > 0.001f)
			{
				t -= delta;
				if (t <= 0.0f)
				{
					t = 0.016f;


					for (int i = 0; i < 5; i++)
						EngineSmoke.Spawn(ballPosition[body[1]] + dir * PARTSIZE * -1.0f - dir * (rand() % 1000 * 0.002f * PARTSIZE),
							dir * 5000.0f * Particleforce * 0.025f, 1,
							EngineSmoke.lifetime * abs(Particleforce) * (rand() % 1000 * 0.0005f + 0.5f));
				}
			}
			glm::vec2 lp = (ballPosition[body[1]] + dir * PARTSIZE);
			DrawLight(glm::vec3(lp.x, lp.y, 0.01f), glm::vec2(800 * abs(Particleforce) * 0.025f), glm::vec4(4.0f, 0.8f, 0.4f, abs(0.5f + abs(Particleforce) + (rand() % 100 - 50) * 0.01f)), 0.0f);
		}
		glm::vec2 mid = (ballPosition[body[1]] + ballPosition[body[0]]) * 0.5f;
		DrawTexturedQuad(mid,glm::vec2(1.0f * PARTSIZE,2.0f* PARTSIZE), RocketEngineTexture,  get_angle_between_points(mid, mid - dir),color, Z_Index,RocketEngineNormalMap);

		/*DrawLine(
			body[0].position - dir * body[0].r,
			body[1].position + dir * body[1].r,
			body[0].r,
			Base.color, true);*/
	}


	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawTexturedQuad(ui_position,glm::vec2(0.5f * size.x,1.0f* size.y), RocketEngineTexture,  0.0f,color, Z_Index,RocketEngineNormalMap);
	};
};

// structure
class BallBody : public BodyComponent
{
public:


	BallBody()
	{
		partid = PART::BALLBODY;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "BallBody";

		Health = PartsData.GetPropertyAsFloat("BallBody", "Health");
		maxHealth = Health;
		
		CreateBody(1);
		ballPosition[body[0]] = position;



		BodyIdsWithCollision.push_back(0);
		OnPartCreate();
	}

	void Create(glm::vec2 position, glm::vec2 direction, float size,float mass = 1.0f) override
	{
		Health = PartsData.GetPropertyAsFloat("BallBody", "Health");
		maxHealth = Health;
		
		ballPosition[body[0]] = position;

		Cost.Matter = 2;

	}
	
	void MTProcess (float dt) override
	{
		ProcessBody(dt);

		ballPosition[body[0]] += ballVelocity[body[0]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		
	}

	void Process(float dt) override
	{
	}
	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		//DrawTexturedQuad(body[0].position, glm::vec2(body[0].r), BallBodyTexture, 0.0f, color, Z_Index + 1, BallBodyNormalMap);
		DrawCircle(ballPosition[body[0]], PARTSIZE, color, true, BallNormalMapTexture, Z_Index);

	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
	
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawCircle(ui_position, size.x*0.25f, color, true, BallNormalMapTexture, Z_Index);
	};
};

class Rotor : public BodyComponent
{
	float diaglen = 0.0f;
public:


	//DamageSphere Sawdamagespheres[4];
	Rotor()
	{
		partid = PART::ROTOR;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "Rotor";

		Health = PartsData.GetPropertyAsFloat("Rotor", "Health");
		maxHealth = Health;
		
		CreateBody(5,2);

		ballPosition[body[0]] = position;
		ballPosition[body[1]] = position + glm::vec2(PARTSIZE * 2.0f, 0.0f);
		ballPosition[body[2]] = position + glm::vec2(0.0f, PARTSIZE * -2.0f);
		ballPosition[body[3]] = position + glm::vec2(PARTSIZE * -2.0f, 0.0f);
		ballPosition[body[4]] = position + glm::vec2(0.0f, PARTSIZE * 2.0f);
		diaglen = sqrt(((PARTSIZE*2.0f) * (PARTSIZE*2.0f)) * 2.0f);
		ProcessConnections();

		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(1);
		BodyIdsWithCollision.push_back(2);
		BodyIdsWithCollision.push_back(3);
		BodyIdsWithCollision.push_back(4);
		OnPartCreate();

	}
	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{
		Health = PartsData.GetPropertyAsFloat("Rotor", "Health");
		maxHealth = Health;

		ballPosition[body[0]] = position;
		ballPosition[body[1]] = position + glm::vec2(PARTSIZE * 2.0f, 0.0f);
		ballPosition[body[2]] = position + glm::vec2(0.0f, PARTSIZE * -2.0f);
		ballPosition[body[3]] = position + glm::vec2(PARTSIZE * -2.0f, 0.0f);
		ballPosition[body[4]] = position + glm::vec2(0.0f, PARTSIZE * 2.0f);
		diaglen = sqrt(((size*2.0f) * (size*2.0f)) * 2.0f);
		ProcessConnections();

		Cost.Matter = 5;
	}
	void ProcessConnections()
	{
		bDataConnections[0].name = "Rotation(clock)";
		bDataConnections[1].name = "Rotation(counter-clock)";
		bDataConnections[0].source = false;
		bDataConnections[1].source = false;
	}
	
	void MTProcess (float dt) override
	{
		ProcessBody(dt);
		ProcessConnections();
		mid = (ballPosition[body[0]]);
		dir = (ballPosition[body[1]] - ballPosition[body[0]]);


		float vel = 0.0f;
		if (bDataConnections[0].data && !bDataConnections[1].data)
			vel = 1.0f;
		if (!bDataConnections[0].data && bDataConnections[1].data)
			vel = -1.0f;

		if (!debris&&!BuildingMode)
		for (int i = 1; i < 5; i++)
		{
			glm::vec2 force = Rotate(Normalize(ballPosition[body[i]] - ballPosition[body[0]]), pi * 0.5f) * Power*0.625f* vel;
			ballForce[body[i]] += force;
			ballForce[body[0]] -= force;

		}

		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;
		ballPosition[body[2]] += ballVelocity[body[2]] * dt;
		ballPosition[body[3]] += ballVelocity[body[3]] * dt;
		ballPosition[body[4]] += ballVelocity[body[4]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;
		ballVelocity[body[2]] += ballForce[body[2]] * dt;
		ballVelocity[body[3]] += ballForce[body[3]] * dt;
		ballVelocity[body[4]] += ballForce[body[4]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };
		ballForce[body[2]] = { 0.0f,0.0f };
		ballForce[body[3]] = { 0.0f,0.0f };
		ballForce[body[4]] = { 0.0f,0.0f };
		
		Strut(body[0], body[1], PARTSIZE * 2.0f);
		Strut(body[0], body[2], PARTSIZE * 2.0f);
		Strut(body[0], body[3], PARTSIZE * 2.0f);
		Strut(body[0], body[4], PARTSIZE * 2.0f);

		Strut(body[1], body[2], diaglen);
		Strut(body[2], body[3], diaglen);
		Strut(body[3], body[4], diaglen);
		Strut(body[4], body[1], diaglen);

	}
	void Process(float dt) override
	{


	}
	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		DrawTexturedQuad(ballPosition[body[0]], { PARTSIZE * 3.0f ,PARTSIZE * 3.0f }, RotorTexture, get_angle_between_points({ 0.0f,0.0f }, dir), color, Z_Index, RotorNormalMap);

	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawTexturedQuad(ui_position, { size.x ,size.x }, RotorTexture, 0.0f, color, Z_Index , RotorNormalMap);
	};
};

// Misc
class Radiator : public BodyComponent
{
public:
	//HEAT related stuff
	float coolingSpeed;
	float mintemp;
	float t = 0.16f;
	Radiator()
	{
		partid = PART::RADIATOR;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "Radiator";
		
		Health = PartsData.GetPropertyAsFloat("Radiator", "Health");
		maxHealth = Health;
		coolingSpeed = PartsData.GetPropertyAsFloat("Radiator", "coolingSpeed");
		mintemp = PartsData.GetPropertyAsFloat("Radiator", "mintemp");


		
		CreateBody(2);
		ballPosition[body[0]]= position + Normalize({0.0f,1.0f}) * PARTSIZE;
		ballPosition[body[1]]= position - Normalize({0.0f,1.0f}) * PARTSIZE;
		
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(1);
		CoolingSpeed = coolingSpeed;
		MinAutocooltemp = mintemp;
		OnPartCreate();
	}
	void Create(glm::vec2 position,glm::vec2 direction, float size, float mass =1.0f) override
	{

		Health = PartsData.GetPropertyAsFloat("Radiator", "Health");
		maxHealth = Health;
		coolingSpeed = PartsData.GetPropertyAsFloat("Radiator", "coolingSpeed");
		mintemp = PartsData.GetPropertyAsFloat("Radiator", "mintemp");


		
		ballPosition[body[0]] = position + Normalize(direction) * size;
		ballPosition[body[1]] = position - Normalize(direction) * size;

		CoolingSpeed = coolingSpeed;
		MinAutocooltemp = mintemp;

		Cost.Matter = 30;

	}

	void MTProcess (float dt) override
	{
		delt = dt;
		ProcessBody(dt);

		mid = 0.5f * (ballPosition[body[0]] + ballPosition[body[1]]);
		dir = (ballPosition[body[1]] - ballPosition[body[0]]);


		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };

		Strut(body[0], body[1], PARTSIZE * 2.0f);
		float change = (ballTemp[body[0]] - ballTemp[body[1]]);

		ballTemp[body[0]] -= change;
		ballTemp[body[1]] += change;

	}
	float delt = 0.0f;
	void Process(float dt) override
	{

		float gain = 0.0f;

		if (!debris && !deactivated)
		{
			gain = ((ballTemp[body[1]] + ballTemp[body[0]])*0.25f);


			CoolingSpeed = coolingSpeed;
			MinAutocooltemp = mintemp;
			if (gain > 0.1f)
			{
				playsound(SHHSound, (ballPosition[body[0]] + ballPosition[body[1]]) * 0.5f, gain * 0.03f, 1.0f, ballVelocity[body[0]]);
			}

			
			
		}
		else
		{
			CoolingSpeed = 1.0f;
		}
	
	}

	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		glm::vec2 dif = ballPosition[body[0]] - ballPosition[body[1]];
		//DrawLine(body[0].position + dif*0.5f, body[1].position - dif * 0.5f, body[0].r, Base.color, true, CubeNormalMapTexture, Z_Index);
		glm::vec2 mid = (ballPosition[body[1]] + ballPosition[body[0]]) * 0.5f;
		float gain = ((ballTemp[body[1]] + ballTemp[body[0]]) * 0.25f);
		if (gain > 0.0f)
		{
			t -= delt;
			if (t <= 0.0f)
			{
				t = 0.016f;
				Smoke.SpawnInCircle(ballPosition[body[0]], PARTSIZE, 2 + gain);
				Smoke.SpawnInCircle(ballPosition[body[1]], PARTSIZE, 2);
			}
		}
		DrawTexturedQuad(mid, glm::vec2(1.0f * PARTSIZE, 2.0f * PARTSIZE), RadiatorTexture, get_angle_between_points(mid, ballPosition[body[0]] ), color, Z_Index, RadiatorNormalMap);

	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawTexturedQuad(ui_position, glm::vec2(size.x*0.5f,size.y), RadiatorTexture, 0.0f, color, Z_Index, RadiatorNormalMap);
	};
};

class StaticPoint : public BodyComponent
{
public:
	//HEAT related stuff
	glm::vec2 position;
	StaticPoint()
	{
		partid = PART::STATICPOINT;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "StaticPoint";
		

		maxHealth = Health;
		CreateBody(1);
		ballPosition[body[0]] = position;
		this->position = position;

		BodyIdsWithCollision.push_back(0);
		OnPartCreate();
	}
	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{

		

		maxHealth = Health;
		ballPosition[body[0]] = position;
		this->position = position;

	}
	
	void MTProcess (float dt) override
	{
		ProcessBody(dt);
		ballPosition[body[0]] = position;
		ballVelocity[body[0]] = { 0.0f,0.0f };
		ballForce[body[0]]= { 0.0f,0.0f };
	
	}


	void Process(float dt) override
	{
	}
	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		DrawCircle(ballPosition[body[0]],PARTSIZE, color, true, BallNormalMapTexture, Z_Index);
	}

	
	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
};

class AIAim : public BodyComponent
{
public:


	AIAim()
	{
		partid = PART::AIAIM;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "AIAim";

		Health = PartsData.GetPropertyAsFloat("AIAim", "Health");
		maxHealth = Health;
		
		CreateBody(1,2,1,1);
		ballPosition[body[0]] = position;


		BodyIdsWithCollision.push_back(0);
		OnPartCreate();
	}

	void Create(glm::vec2 position, glm::vec2 direction, float size,float mass = 1.0f) override
	{
		Health = PartsData.GetPropertyAsFloat("AIAim", "Health");
		maxHealth = Health;
		
		ballPosition[body[0]]= position;

		Cost.Matter = 5;

	}
	void ProcessConnections()
	{
		vDataConnections[0].name = "ClosestEnemy";
		vDataConnections[0].source = true;

		fDataConnections[0].name = "DistanceToClosestEnemy";
		fDataConnections[0].source = true;

		bDataConnections[0].name = "AimToMouse";
		bDataConnections[0].source = false;

		bDataConnections[1].name = "ClosestToMouse";
		bDataConnections[1].source = false;
	}
	void MTProcess (float dt) override
	{
		ProcessConnections();
		ProcessBody(dt);

		ballPosition[body[0]] += ballVelocity[body[0]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };

		mid = ballPosition[body[0]];
		ballForce[body[0]] = glm::vec2(0.0f);
		glm::vec2 CalculateFrom = mid;
		if(bDataConnections[1].connected && bDataConnections[1].data && player)
			CalculateFrom = foregroundMousePosition;
		glm::vec2 closestmid = mid;
		float lastsqrlength = sqrlength(CalculateFrom - closestmid);
		bool first = true;
		if(player)
		{
			for(int i =1; i<Entities.size();i++)
			{
				if(first)
				{
					closestmid = Entities[i]->mid;
					lastsqrlength = sqrlength(CalculateFrom - closestmid);
					first = false;
				}
				else if(lastsqrlength > sqrlength(CalculateFrom - Entities[i]->mid))
				{
					closestmid = Entities[i]->mid;
					lastsqrlength = sqrlength(CalculateFrom - closestmid);				
				}
			}
			if(Entities.size() == 1)//player is the only ship
			{
				closestmid = Entities[0]->mid;
				lastsqrlength = sqrlength(CalculateFrom - closestmid);
			}

			if(bDataConnections[0].connected &&bDataConnections[0].data)
			{
				closestmid = foregroundMousePosition;
				lastsqrlength = sqrlength(mid - closestmid);
			}
		}
		else if(Entities.size()>0)
		{
			closestmid = Entities[0]->mid;
			lastsqrlength = sqrlength(CalculateFrom - closestmid);
		}



		

		vDataConnections[0].data = closestmid;
		fDataConnections[0].data = sqrt(lastsqrlength);

	}

	void Process(float dt) override
	{
	}
	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}
		//DrawTexturedQuad(body[0].position, glm::vec2(body[0].r), BallBodyTexture, 0.0f, color, Z_Index + 1, BallBodyNormalMap);
		DrawCircle(ballPosition[body[0]], PARTSIZE, color, true, BallNormalMapTexture, Z_Index);

	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}
	
	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
		UI_DrawCircle(ui_position, size.x*0.25f, color, true, BallNormalMapTexture, Z_Index);
	};
};

class Glider : public BodyComponent
{
	float diaglength = 0.0f;
public:	
	float HeatPerSecond = 1.5f;
	float EngineHeat = 0.4f;

	float throtles[4] = {0.0f,0.0f,0.0f,0.0f};

	glm::vec2 Force = { 0.0f,0.0f };
	
	glm::vec2 OutForce = glm::vec2(0.0f);


	float t = 0.0f;

	Glider()
	{
		
		partid = PART::GLIDER;
		parttype = TYPE::STRUCTUREPART;
		type = partid + NodeType::LASTNODE;
		Name = "Glider";
		
		Health = PartsData.GetPropertyAsFloat("Glider", "Health");
		maxHealth = Health;
		HeatPerSecond = PartsData.GetPropertyAsFloat("Glider", "HeatPerSecond");
		Power = PartsData.GetPropertyAsFloat("Glider", "Power");

		CreateBody(4, 1, 0, 1);

		float ang = 0.25f * pi;
		ballPosition[body[0]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[1]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[2]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[3]] = position + Rotate({ 0.0f,1.0f }, ang);


		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);
		Cost.Matter = 25;

		shutdownTemp = 15.0f;
		ProcessConnections();
		BodyIdsWithCollision.push_back(0);
		BodyIdsWithCollision.push_back(1);
		BodyIdsWithCollision.push_back(2);
		BodyIdsWithCollision.push_back(3);
		OnPartCreate();
	}
	void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f) override
	{

		Health = PartsData.GetPropertyAsFloat("Glider", "Health");
		maxHealth = Health;
		HeatPerSecond = PartsData.GetPropertyAsFloat("Glider", "HeatPerSecond");
		Power = PartsData.GetPropertyAsFloat("Glider", "Power");

		float ang = 0.25f * pi;
		ballPosition[body[0]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[1]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[2]] = position + Rotate({ 0.0f,1.0f }, ang);

		ang += pi * 0.5;
		ballPosition[body[3]] = position + Rotate({ 0.0f,1.0f }, ang);

		diaglength = sqrt(PARTSIZE * 2.0f * PARTSIZE * 2.0f + PARTSIZE * 2.0f * PARTSIZE * 2.0f);

		shutdownTemp = 15.0f;
		ProcessConnections();
	}
	void ProcessConnections()
	{
		bDataConnections[0].name = "\"Heater\"";
		vDataConnections[0].name = "AllignTo";

		vDataConnections[0].source = false;
		bDataConnections[0].source = false;
	}
	
	void MTProcess(float dt) override
	{
		ProcessConnections();
		ProcessBody(dt);

		glm::vec2 avgvel = (ballVelocity[body[0]] + ballVelocity[body[1]] + ballVelocity[body[2]] + ballVelocity[body[3]]) * 0.25f;
		mid = (ballPosition[body[0]]+ ballPosition[body[1]]+ ballPosition[body[2]]+ ballPosition[body[3]]) * 0.25f;
		float avgtemp = (ballTemp[body[0]]+ ballTemp[body[1]]+ ballTemp[body[2]]+ ballTemp[body[3]])*0.25f;
		if ((bDataConnections[0].connected && bDataConnections[0].data))
			for(int i=0;i<4;i++)
				throtles[i] = 1.0f;
		glm::vec2 glidedir = {0.0f,0.0f};

		if(avgtemp < 2.0f)
			shutdown = false;
		if(avgtemp>shutdownTemp)
			shutdown = true;

		if(vDataConnections[0].connected)
		{
			if(sqrlength(vDataConnections[0].data)>1.1f) //its not normalized
			{
				glidedir = Normalize(vDataConnections[0].data - mid);
			}
			else
				glidedir= vDataConnections[0].data;
		}
		if (!debris && !shutdown && vDataConnections[0].connected && !deactivated && sqrlength(avgvel) > 10.0f*10.0f && sqrlength(glidedir)>0.0f
		)
		{
			ballForce[body[0]] = Force;
			ballForce[body[1]] = Force;
			ballForce[body[2]] = Force;
			ballForce[body[3]] = Force;

			glm::vec2 veldir = Normalize (avgvel);
			glm::vec2 norm = {-veldir.y,veldir.x};
			glm::vec2 trustvec = DOT(norm, -glidedir) * norm;
			
			float forcemult = 1.0f;
			ballForce[body[0]] -= trustvec * Power * 1.0f * forcemult;
			ballForce[body[1]] -= trustvec * Power * 1.0f * forcemult;
			ballForce[body[2]] -= trustvec * Power * 1.0f * forcemult;
			ballForce[body[3]] -= trustvec * Power * 1.0f * forcemult;

			for(int i=0;i<4;i++)
			{
				glm::vec2 dir = Normalize (ballPosition[body[i]] - mid);
				
				throtles[i] =  DOT(dir,trustvec) * 2.0f;
				if(throtles[i] <0.0f)
					throtles[i] = 0.0f;

			}
		}
		else
			for(int i=0;i<4;i++)
				throtles[i] = 0.0f;


		for(int i=0;i<4;i++)
		{			
			ballTemp[body[i]]+= abs(throtles[i]) * HeatPerSecond * dt;
		}
		float change = (ballTemp[body[0]]- ballTemp[body[1]]);

		ballTemp[body[0]]-= change* dt;
		ballTemp[body[1]]+= change* dt;
		change = (ballTemp[body[1]]- ballTemp[body[2]]);
		ballTemp[body[1]]-= change* dt;
		ballTemp[body[2]]+= change* dt;
		change = (ballTemp[body[2]]- ballTemp[body[3]]);
		ballTemp[body[2]]-= change* dt;
		ballTemp[body[3]]+= change* dt;
		change = (ballTemp[body[3]]- ballTemp[body[0]]);
		ballTemp[body[3]]-= change* dt;
		ballTemp[body[0]]+= change* dt;
		

		glm::vec2  velbuf = ballVelocity[body[0]];


		ballPosition[body[0]] += ballVelocity[body[0]] * dt;
		ballPosition[body[1]] += ballVelocity[body[1]] * dt;
		ballPosition[body[2]] += ballVelocity[body[2]] * dt;
		ballPosition[body[3]] += ballVelocity[body[3]] * dt;

		ballVelocity[body[0]] += ballForce[body[0]] * dt;
		ballVelocity[body[1]] += ballForce[body[1]] * dt;
		ballVelocity[body[2]] += ballForce[body[2]] * dt;
		ballVelocity[body[3]] += ballForce[body[3]] * dt;

		ballForce[body[0]] = { 0.0f,0.0f };
		ballForce[body[1]] = { 0.0f,0.0f };
		ballForce[body[2]] = { 0.0f,0.0f };
		ballForce[body[3]] = { 0.0f,0.0f };


		Strut(body[0], body[1], PARTSIZE * 2.0f);
		Strut(body[1], body[2], PARTSIZE * 2.0f);
		Strut(body[2], body[3], PARTSIZE * 2.0f);
		Strut(body[0], body[3], PARTSIZE * 2.0f);

		Strut(body[0], body[2], diaglength);
		Strut(body[3], body[1], diaglength);

	
	}
	void Process(float dt) override
	{
		

		if (!debris )
		{
			throtle = 0.0f;
			for(int i=0;i<4;i++)
			{
				throtle += throtles[i];
			}
			if (abs(throtle) > 0.0f && !shutdown && !deactivated)
			{
				playsound(RocketEngineSound, (ballPosition[body[1]] + ballPosition[body[0]]) * 0.5f, throtle * 0.25f * (freq <= 0.001f ? 0.0f : 1.0f), freq, ballVelocity[body[0]]);


			}
			else
			{
				ballForce[body[1]] = { 0.0f,0.0f };
				ballForce[body[0]] = { 0.0f,0.0f };
			}
		}
		else
		{
			ballForce[body[1]] = { 0.0f,0.0f };
			ballForce[body[0]] = {0.0f,0.0f};

		}
	}

	void Draw() override
	{

		if (drawHealth)
		{
			float stage = Health / maxHealth;
			color = glm::vec4(((1.0f - stage) * 1.1f), stage * 1.1f, 0.0f, 1.0f);
		}
		if (drawHeat)
		{
			for (auto i : body)
			{
				float stage = ballTemp[i] / MaxTemp;
				if (stage > 0)
					DrawCircle(ballPosition[i], PARTSIZE, HeatColor * stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
				else
					DrawCircle(ballPosition[i], PARTSIZE, ColdColor * -stage + glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, BallNormalMapTexture, Z_Index + 10);
			}
		}

		throtle = 0.0f;
		for(int i=0;i<4;i++)
		{
			throtle += throtles[i];
		}
		t -= delta;
		if (t <= 0.0f)
		{
			t = 0.016f;
			for(int i=0;i<4;i++)
				if (abs(throtles[i]) > 0.0f)
				{
					float Particleforce = throtles[i];
					glm::vec2 dir = ballPosition[body[i]] - mid;
					for (int a = 0; a < 5; a++)
						EngineSmoke.Spawn(ballPosition[body[i]],
							dir * 5000.0f * Particleforce * 0.025f, 1,
							EngineSmoke.lifetime * abs(Particleforce) * (rand() % 1000 * 0.0005f + 0.5f));
					
				}
		}
		glm::vec2 lp = (mid);
		DrawLight(glm::vec3(lp.x,lp.y, 0.01f), glm::vec2( 800 * abs(throtle)*0.025f), glm::vec4(4.0f, 0.8f, 0.4f, abs(0.5f + abs(throtle) + (rand() % 100 - 50) * 0.01f)), 0.0f);
		

		glm::vec2 mid2 = (ballPosition[body[1]] + mid) * 0.5f;
		DrawTexturedQuad(mid, glm::vec2(PARTSIZE * 2.0f), GunBaseTexture, get_angle_between_points(ballPosition[body[0]], ballPosition[body[3]]) , color, Z_Index + 1, GunBaseNormalMap,false);

	}

	void DeletePart() override
	{
		Delete = true;
		DeleteBody();
	}

	void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override
	{
	
		
	};
};


void InitParts()
{
	PurchasableParts.clear();
	PurchasableParts.push_back(PART::BALLBODY);
	PurchasableParts.push_back(PART::ROCKETENGINE);
	PurchasableParts.push_back(PART::GUN);
	PurchasableParts.push_back(PART::LASERGUN);
	PurchasableParts.push_back(PART::RADIATOR);
	PurchasableParts.push_back(PART::ROTOR);
	PurchasableParts.push_back(PART::ROCKETLAUNCHER);
	PurchasableParts.push_back(PART::MINIGUN);
	PurchasableParts.push_back(PART::AIAIM);
	PurchasableParts.push_back(PART::GLIDER);

	NodeConstructors.insert({NodeType::LASTNODE -1,[](){return (Node*)new DestructableStaticBall();}});
	NodeConstructorNames.insert({NodeType::LASTNODE -1,"DestructableStaticBall"});
	NodeConstructors.insert({NodeType::LASTNODE - 2,[](){return (Node*)new DestructableStaticCube();}});
	NodeConstructorNames.insert({NodeType::LASTNODE - 2,"DestructableStaticCube"});
	NodeConstructors.insert({NodeType::LASTNODE - 3,[](){return (Node*)new StaticDamageSphere();}});
	NodeConstructorNames.insert({NodeType::LASTNODE - 3,"StaticDamageSphere"});
	NodeConstructors.insert({NodeType::LASTNODE - 4,[](){return (Node*)new Monitor();}});
	NodeConstructorNames.insert({NodeType::LASTNODE - 4,"Monitor"});
	NodeConstructors.insert({NodeType::LASTNODE - 5,[](){return (Node*)new TriggerCube();}});
	NodeConstructorNames.insert({NodeType::LASTNODE - 5,"TriggerCube"});

	NodeConstructors.insert({NodeType::LASTNODE + PART::BALLBODY,[](){return (Node*)new BallBody();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::BALLBODY,"BallBody"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::ROCKETENGINE,[](){return (Node*)new RocketEngine();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::ROCKETENGINE,"RocketEngine"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::GUN,[](){return (Node*)new Gun();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::GUN,"Gun"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::LASERGUN,[](){return (Node*)new LaserGun();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::LASERGUN,"LaserGun"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::RADIATOR,[](){return (Node*)new Radiator();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::RADIATOR,"Radiator"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::ROTOR,[](){return (Node*)new Rotor();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::ROTOR,"Rotor"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::ROCKETLAUNCHER,[](){return (Node*)new RocketLauncher();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::ROCKETLAUNCHER,"RocketLauncher"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::MINIGUN,[](){return (Node*)new MiniGun();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::RAILGUN,"MiniGun"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::CENTRALPART,[](){return (Node*)new CentralPart();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::CENTRALPART,"CentralPart"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::AIAIM,[](){return (Node*)new AIAim();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::AIAIM,"AIAim"});
	NodeConstructors.insert({NodeType::LASTNODE + PART::GLIDER,[](){return (Node*)new Glider();}});
	NodeConstructorNames.insert({NodeType::LASTNODE + PART::GLIDER,"Glider"});
}
BodyComponent* CreatePart(int type, glm::vec2 position, glm::vec2 direction, float size, float mass)
{
	BodyComponent* b= NULL;
	if(type <LASTPART && type != PART::STATICPOINT)
	{
		b = (BodyComponent*)NodeConstructors[NodeType::LASTNODE + type]();
		b->Create(position, direction, size,mass);
	}
	return b;

}
