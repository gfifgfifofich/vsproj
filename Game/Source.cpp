/*
TODO:
{


	more parts
	{
		Shields
		cannon, autocannon
		fix laser
		impulse laser, rapidfire impulse laser
		shileds plates/armour
		bigger rocket engines, multidirectional?, axis alligner/glider
		logic&math modules + input modules2 
	} 

	| Simulations - minigames to unlock new parts? ( race tracks for new engines, wavedeffence for new weapons?)
	| Parts research, like in reassembly is perfect, just another window
	
	

	lots of textures
	AI
	UI

	Instanced lighting 
	Memory Optimizations
	{
		ball[100'000] on stack, parts have int[] ids to the vector; or something //done, works
		array of structs, processor's memory grabber will optimise it //done even better, just arrays, no structs	
	}
}


Mission ideas:
	retrive part/object/debrie ?? 
	kill N crafts ("Pirates") // mostly implemented
	kill N bugs ("Infestation") // make bug parts, modify central part, piratemission with bug crafts, done
	
	defend building/mine for N minutes/waves (or factory/mine, the longer its alive, the more resources player gets)

Mission
{
	Random objects (static asteroids - DONE, planets, fog fields that slow down...)
	Debrie (Random parts for free mb)
	Matter deposits (geodes?)(Extra reward for exploration)


	bool retrival necessary ( retrieve to get resources or not) // probably always necessary

	int dificulty (tier/amount of enemies)  
	int size (amount of enemies)  
	// mb dificulty * size 
	border - killing fog

	enum int type // mining, retrival, defeat pirates, clear bugs etc

}

Stages
{
	Current sate
	{
		Openworld like seamless "MapLoading" - MissionLoader
		Inside/outside, Heat attracts enemies
		1.5 - 2k balls @60fps {balls are in array on stack, parts and other GameObjects are still in chaos in vector<Node*>}
	}
	
	todo {
		Radar {
			Random points - Red, Blue, Yellow and mb green
				Red - something damgerous, will only bleep ocasionally - when guns are fired
				Yellow/Orange - Large heat emmisions
				Blue - mission target and other stuff like that
				green or the color of radar - physical obstractions
			Getting closer to dots on radar will load them (Missionloader in case of physical obstacles & missions)
			Entering new bleep will delete prewious in some cases

			All dots look the same
			when enemy dot gets in radius of heat - it may bleep back and start moving towards player.
			Some dots will bleep on their own, and imitate fight on the radar.

		}

		layers of scene - 
		-	background
		-	Middle - player, objects, game
		-	ForeGround - Fog and other stuff "above" player, will add really great volume felling
		-	interface layer
		-	menulayer

		probably add as quality settings (Lighting, bloom, fog can just have different framebuffer sizes/amount)


		draw foreground fog with half or quatter of resolution (affects shit, bc of bloom, but bloom + particle lighting gives mega depth and volume), fog only = dont care, linear filter will fix low res
		todo: make light buffer & others with less res/ color depth, they dont need full res 32bit float buffers	


		Rewrite
		{
			{
				Fix bleeps
				{
					type
					fraction
					questid
					SaveString
				}
				make quest, vector with quests
				make reputations
				make stations/mission selectors
				a few missions
			}

			remake all multithreading through cpp20 std::for_each(par)

			Solid bleeps. will be one of: enemy(es), asteroids, resources, debrie.
			Gas bleeps - 95% just smoke, some possibility to have hiden solid bleep
			Blue bleeps - Live stations, bleeps only once a few seconds
			Yellow constant bleeps - marks, missions
			
			two enemy bleeps colliding/beng close = high heat for 10-120sec. after - debrie spawn, 50/50 one lives, exits to random ditrection.
			Fast bleep - carrier. can haul materials or parts. fast, hard to hit/catch
			asteroid field bleep
				chance for resources
				chance for enemy
				chance for chill station
			DebrieStation. Just like pirate station, but disabled, full of debrie.
			PirateStation. There is a possibility to use a full scene in theese bleeps, so just make blocks and spawn custom pirate base
			random flying asteroid. big fast bleep crossing map. turns other bleeps into debrie, deletes fog bleeps.

			Station
			Stations will be of one corpo
			maybe a possibility to explore station rooms, fight through to get to reactor, make a little boom.
			station may hint towards other stations/pirates/other bullshit. 
			each corpo - reputation int 
			{
				possible mission - send towards an area to get data. 
				Destroing data - mission failed. 
				Reading data - mission failed. Spawn new marker on map. maybe timer. 
				Possibility to get a message from corpo, mistake on place, mission abort when get the box of data, possible start of story.
			}
			{
				attach large cargo/ multiple cargos to ship.
				deliver to other station.
				loosing cargo - less pay/rep per cargo untill full lose
			}
			{
				Test weapon prototype	
				get weapon
				fight some stuff with it/ go through a cloud of shit/ test it
				turn back
				destroing equipment - mission failed
			}
			{
				Delete station/ pirateStation
			}
			{
				Explore area
				may cause story moment
				may be a suspicious building
				may be nothing, but fog on radar ill sugest were source of signal is now (giant bruse/void/ocasional heat/radio bleep)
			}
			{
				Bounties
				a list of ships that have a bounty
				short description of ship look, vage orientation were it was last spoted. updates every ~5 mins
			}
			{
				Get some weapons/materials
				after enough of supply missions - random event - a fleet of ships will fly towards nearest competitor station
			}
			
			each corpo has their own compaign.
			1 will serch total domination
				end - no other corpos. just destruction. fastest and most aggressive campaign
			other will serach technological ascention
				will mostly ignore all other factions, but sometimes will collide in fights over resources/data/tech
				end - all the parts are at your disposal, other corpos mostly destroyed
			third will try to control balance
				will want some boring goverment stuff from other corpos
				end - other fractions exist, bit only under control of this corpo

			
				ecs memory idea
				{

					Node[]
					Object[]

					object - only object data, id in array gives data of node. 
					No virtual functions, all previous functions accesable through id in array
					if they will be just an array[100k], then they can be identified by id

					+ or -, but +cusstom process() "tree"

					for(nodes)
						if nodetype == NODE
							proces
						else
							continue

					for(objects)
						if nodetype == objects
							process
						else
							continue

					etc..


				}

			
		}			
	}



	Release stage
	{
		at least 1 fully playable mode with features endless sandbox
		full play through 3 times with no major bugs
		No tmp textures & sounds
	}

}

*/

#include <filesystem>


#include "engine/Components/Redactor.h"
#include "engine/Components/Include/sounds.h"
glm::ivec2 StandartResolutions[] =
{
	{800, 600},
	{ 1024,768 },
	{ 1280,720 },
	{ 1280,1024 },
	{ 1366,768 },
	{ 1680,1050 },
	{ 1920,1080 },
	{ 2560,1440 }
};

#include "Parts/smallball.h"

#include "Textures.h"

#include "Helper.h"

#include "ParticleMaterials.h"

#include "DamageSphere.h"
#include "Explodion.h"
#include "SubECS.h"

#include "Parts/Base.h"


#include "Rocket.h"
#include "Bullet.h"
#include "Laser.h"
#include "Parts.h"
#include "Entity.h"


#include "Mission.h"
#include "Radar.h"

bool disablefog = false;

void Delete()
{
}

void ChangeMap(std::string FilePath, glm::vec2 lastoffset, glm::vec2 newoffset)
{
	std::cout<<"changing map to: " <<FilePath<<"\n";
	
	ConsoleTexts.clear();	
	MainMenu = false;

	ActiveRadar.offset = lastoffset;
	glm::vec2 movevec = ActiveRadar.offset;
	movevec -= newoffset;


	CurrnetMission.MissionClear();
	// clear directory, save entities


	for (int i = 0; i < Rockets.size(); i++)
	{
		Rockets[i]->body[0].position += movevec;
		Rockets[i]->body[1].position += movevec;
		Rockets[i]->DS.body.position += movevec;
		Rockets[i]->Explode();
	}
	int EntitiesSize = 0;
	for (int i = 0; i < Entities.size(); i++)
	{
		if (!Entities[i]->missionPawn)
		{
			std::string filename = "tmp/";
			filename += std::to_string(i);
			Entities[i]->SaveTo(filename, true);
			EntitiesSize++;
		}
		for (int a = 0; a < Entities[i]->Parts.size(); a++)
		{
			Entities[i]->Parts[a]->DeletePart();
		}
	}
	//if(Entities.size()>=1)
	Entities.clear();

	for (int i = 0; i < Debris.Parts.size(); i++)
		Debris.Parts[i]->DeletePart();
	Debris.Parts.clear();

	for (int i = 0; i < 100'000; i++)
	{
		IsBall[i] = false;
	}
	lastid = 0; 
	freeBallIDs.clear();
	GameScene->LoadFrom(FilePath);

	


	ActiveRadar.offset = newoffset;
	for (int i = 0; i < emiters.size(); i++)
		for (int a = 0; a < emiters[i]->Particles.size(); a++)
			emiters[i]->Particles[a].position += movevec;

	Rockets.clear();
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].body.body.position += movevec;
		bullets[i].ball.position += movevec;
	}
	//bullets.clear();
	
	for (int i = 0; i < DamageSpheresArray.size(); i++)
	{
		DamageSpheresArray[i].body.position += movevec;
	}
	for (int i = 0; i < Lasers.size(); i++)
	{
		Lasers[i].body.body.position += movevec;
		Lasers[i].RayCast.position += movevec;
	}
	for (int i = 0; i < LightEffects.size(); i++)
	{
		LightEffects[i].position.x += movevec.x;
		LightEffects[i].position.y += movevec.y;
	}
	for (int i = 0; i < ExplodionArray.size(); i++)
	{
		ExplodionArray[i].DS.body.position += movevec;
	}


	// SpawnEntities
	for (int i = 0; i < EntitiesSize; i++)
	{
		std::string filename = "tmp/";
		filename += std::to_string(i);
		SpawnShipAllSave(filename);
	}


	std::cout<<"\nMap Changed to: "<<FilePath;
	SavePlayerData();
}




void SetupInstances()
{
	PartInstances.clear();
	for(auto i : PurchasableParts)
	{
		PartInstances.push_back(CreatePart(i,{0.0f,0.0f},{0.0f,1.0f},1.0f,1.0f));
		GameScene->Nodes.pop_back();// delete autocreated pointer from scene tree
	}
}


void ProcessPlayerControls()
{

	glm::vec4 UI_ColorON = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec4 UI_ColorOFF = glm::vec4(0.8f, 0.8f, 0.8f, 0.8f);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* sw = GetWindow(ForeWindowID);
	Window* iw = GetWindow(InterfaceWindowID);
	Window* mw = GetWindow(MenuWindowID);
	Window* pw = GetWindow(PreviewWindowID);
	Window* jw = GetWindow(JournalWindowID); 

	AqueredCameraScale *= 1.0f + scrollmovement * 0.1f;

	if (AqueredCameraScale.x > 80.0f)
		AqueredCameraScale = { 80.0f, 80.0f };
	//if (AqueredCameraScale.x < 1.0f)
	//	AqueredCameraScale = { 1.0f ,1.0f };

	CameraScale += (AqueredCameraScale - CameraScale) * 0.25f * (delta > 0.017f ? 0.017f : delta) * 60.0f;

	if (JustPressedkey[GLFW_KEY_V])
		absoluteControl = !absoluteControl;

	if (!absoluteControl)
	{
		glm::vec2 LookAtVec = Normalize(MousePosition - Entities[0]->CenterOfMass);

		if (LookAtVec.x != 0.0f || LookAtVec.y != 0.0f)LookAtVec = Normalize(LookAtVec);
		if (sqrlength(MousePosition - CameraPosition) > 10.0f)
			Entities[0]->LookAt = LookAtVec;

		glm::vec2 velvec = glm::vec2(0.0f);

		if (keys[GLFW_KEY_W]) velvec += LookAtVec;
		if (keys[GLFW_KEY_S]) velvec -= LookAtVec;
		if (keys[GLFW_KEY_D]) velvec += glm::vec2(LookAtVec.y, -LookAtVec.x);
		if (keys[GLFW_KEY_A]) velvec -= glm::vec2(LookAtVec.y, -LookAtVec.x);


		if (velvec.x != 0.0f || velvec.y != 0.0f) velvec = Normalize(velvec);
		Entities[0]->ThrustDirection = velvec;

	}
	else if (Entities.size()>0)
	{
		glm::vec2 LookAtVec = Entities[0]->direction;

		if (LookAtVec.x != 0.0f || LookAtVec.y != 0.0f)
			LookAtVec = Normalize(LookAtVec);
		else
			LookAtVec = glm::vec2(0.0f, 1.0f);


		if (keys[GLFW_KEY_A]) LookAtVec = Rotate(LookAtVec, pi * 0.5);
		else if (keys[GLFW_KEY_D]) LookAtVec = Rotate(LookAtVec, pi * -0.5);
		else LookAtVec = Entities[0]->direction;

		Entities[0]->LookAt = LookAtVec;

		glm::vec2 velvec = glm::vec2(0.0f);

		if (keys[GLFW_KEY_W]) velvec += Entities[0]->direction;
		if (keys[GLFW_KEY_S]) velvec -= Entities[0]->direction;
		if (keys[GLFW_KEY_E]) velvec += glm::vec2(Entities[0]->direction.y, -Entities[0]->direction.x);
		if (keys[GLFW_KEY_Q]) velvec -= glm::vec2(Entities[0]->direction.y, -Entities[0]->direction.x);

		if (velvec.x != 0.0f || velvec.y != 0.0f) velvec = Normalize(velvec);
		Entities[0]->ThrustDirection = velvec;
	}

	if (Entities.size() > 0 && !Entities[0]->dead && !Entities[0]->destroyed)
	{
		camerapos = Entities[0]->mid;
		Entities[0]->player = true;
		Entities[0]->autocontrol = false;
		listenerVel = { Entities[0]->midvel.x ,Entities[0]->midvel.y ,1.0f };

		if (BuildingMode || bLogicMode || fLogicMode || vLogicMode)
		{
			BackgroundColor = glm::vec4(0.0f, 0.0f, 0.01f, 1.0f);
			//grid
			glm::vec2 gridpos = glm::vec2(0.0f, 0.0f);
			Entities[0]->FireGuns = false;

			// grid size = 1
			glm::vec2 roundCP = glm::vec2(roundf(camerapos.x), roundf(camerapos.y));
			float aVal = (CameraScale.x - 10.0f) / 70.0f;
			if (CameraScale.x > 10.0f)
				for (int i = 0; i < 200; i++)
				{
					DrawLine(glm::vec2(50 * (i * 0.02f - 2.0f), 50 * -2.0f) + gridpos + roundCP,
						glm::vec2(50 * (i * 0.02f - 2.0f), 50 * 2.0f) + gridpos + roundCP, 1.75f * 0.025f,
						glm::vec4(0.75f, 0.75f, 0.75f, aVal), false, CubeNormalMapTexture, -10);

					DrawLine(glm::vec2(50 * -2.0f, 50 * (i * 0.02f - 2.0f)) + gridpos + roundCP,
						glm::vec2(50 * 2.0f, 50 * (i * 0.02f - 2.0f)) + gridpos + roundCP, 1.75f * 0.025f,
						glm::vec4(0.75f, 0.75f, 0.75f, aVal), false, CubeNormalMapTexture, -10);
				}

			if (JustPressedLMB && in_UI <= 0)
			{
				DamageSphere DS;
				DS.body.r = 5.0f;
				DS.body.position = MousePosition;
				DS.Damage = false;
				DS.timeLeft = 1.0f;
				DS.Heat = 0.0f;
				DS.recoil = 0.0f;
				DS.neutral = true;
				DS.friction = 0.9f;
				DS.singleHit = false;
				DamageSpheresArray.push_back(DS);
			}
		}
		else
		{
			DataconnectionData[0] = 0;

			Entities[0]->GunsTargetrotPoint = MousePosition;
			BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			/*if (buttons[GLFW_MOUSE_BUTTON_2])
			{

				Entities.push_back(new Entity);
				Entities[Entities.size() - 1]->Ready(MousePosition, { 0.0f,1.0f }, PARTSIZE);
				Entities[Entities.size() - 1]->LoadFrom("Cube.sav");
				Entities[Entities.size() - 1]->autocontrol = true;
				Entities[Entities.size() - 1]->trgPos = MousePosition;
				Entities[Entities.size() - 1]->enemy = true;
				Entities[Entities.size() - 1]->CP.Health=-10;


			}*/
		}
		if (bLogicMode || fLogicMode || vLogicMode)
		{
			if (JustPressedbutton[GLFW_MOUSE_BUTTON_2])
				DataconnectionData[0] = 0;


			for (int i = 0; i < Entities[0]->Parts.size(); i++)
			{
				Entities[0]->Parts[i]->DrawDataConnections(i, bLogicMode, fLogicMode, vLogicMode, 1000);

			}
			if (DataconnectionData[0] == 1)
			{
				if (DataconnectionData[1] == 0)
					DrawLine(MousePosition, Entities[0]->Parts[DataconnectionData[2]]->bDataConnections[DataconnectionData[3]].lastpos, 0.125f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0, 1000);
				else if (DataconnectionData[1] == 1)
					DrawLine(MousePosition, Entities[0]->Parts[DataconnectionData[2]]->fDataConnections[DataconnectionData[3]].lastpos, 0.125f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0, 0, 1000);
				else if (DataconnectionData[1] == 2)
					DrawLine(MousePosition, Entities[0]->Parts[DataconnectionData[2]]->vDataConnections[DataconnectionData[3]].lastpos, 0.125f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0, 1000);
			}
			if (DataconnectionData[0] == 2)
			{
				Entities[0]->AddDataConnection(DataconnectionData[1], DataconnectionData[2], DataconnectionData[3], DataconnectionData[4], DataconnectionData[5]);
				DataconnectionData[0] = 0;
			}
		}
		else
			DataconnectionData[0] = 0;

		if (BuildingMode)
		{


			if (JustPressedkey[GLFW_KEY_TAB])
			{
				NewConType++;
				if (NewConType > 3) NewConType = 0;
			}

			if (JustPressedkey[GLFW_KEY_SPACE])
				align = !align;
			if (align)
			{
				//Entities[0].Parts[0] -> central part
				glm::vec2 mid = (ballPosition[Entities[0]->Parts[0]->body[0]] + ballPosition[Entities[0]->Parts[0]->body[1]]) * 0.5f;

				ballVelocity[Entities[0]->Parts[0]->body[0]] = ((mid + glm::vec2(0.0f, 1.0f) * PARTSIZE) - ballPosition[Entities[0]->Parts[0]->body[0]]) * 15.0f;
				ballVelocity[Entities[0]->Parts[0]->body[1]] = ((mid - glm::vec2(0.0f, 1.0f) * PARTSIZE) - ballPosition[Entities[0]->Parts[0]->body[1]]) * 15.0f;

			}



			if (keys[GLFW_KEY_LEFT_ALT])
			{
				if (ConCreationStage == 1)
				{
					DrawLine(ballPosition[NewConBall1], MousePosition, 0.125f);
				}
				if (JustPressedLMB)
				{
					int prevcon = ConCreationStage;
					for (int i = 0; i < Entities[0]->Parts.size(); i++)
					{
						for (auto a : Entities[0]->Parts[i]->BodyIdsWithCollision)
						{

							glm::vec2 dif = ballPosition[Entities[0]->Parts[i]->body[a]] - MousePosition;
							if (sqrlength(dif) < PARTSIZE * PARTSIZE)
							{
								if (ConCreationStage == 0)
								{
									NewConBall1 = Entities[0]->Parts[i]->body[a];
									NewConIndex1 = a;
									NewConPart1 = i;
									ConCreationStage++;
								}
								else if (ConCreationStage == 1)
								{
									NewConBall2 = Entities[0]->Parts[i]->body[a];
									NewConIndex2 = a;
									NewConPart2 = i;
									ConCreationStage++;
								}
							}
						}

					}



					if (prevcon == ConCreationStage && !NewConDebrie1)
					{
						for (int i = 0; i < Debris.Parts.size(); i++)
						{
							for (auto a : Debris.Parts[i]->BodyIdsWithCollision)
							{
								glm::vec2 dif = ballPosition[Debris.Parts[i]->body[a]] - MousePosition;
								if (sqrlength(dif) < PARTSIZE * PARTSIZE)
								{
									if (ConCreationStage == 0)
									{
										NewConBall1 = Debris.Parts[i]->body[a];
										NewConIndex1 = a;
										NewConPart1 = i;
										ConCreationStage++;
										NewConDebrie1 = true;
									}
									else if (ConCreationStage == 1)
									{
										NewConBall2 = Debris.Parts[i]->body[a];
										NewConIndex2 = a;
										NewConPart2 = i;
										ConCreationStage++;
										NewConDebrie2 = true;
									}
								}
							}

						}


					}
					if (ConCreationStage == 1 && prevcon != ConCreationStage)
					{
						Sparks.Spawn(ballPosition[NewConBall1], rand() % 5 + 5);
						playsound(Clang, MousePosition, 0.3f, 2.5f);
					}
					if (ConCreationStage >= 2)
					{
						playsound(Clang, MousePosition, 0.3f, 2.5f);
						Sparks.Spawn(ballPosition[NewConBall2], rand() % 5 + 5);
						ConCreationStage = 0;
						if (NewConDebrie1)
							NewConPart1 = Entities[0]->ClaimPart(NewConPart1);
						if (NewConDebrie2)
							NewConPart2 = Entities[0]->ClaimPart(NewConPart2);
						NewConDebrie1 = false;
						NewConDebrie2 = false;

						if (NewConPart1 != NewConPart2)
							Entities[0]->AddConnection(NewConType, -1, 0.125f, 1500, 2, NewConType == HEATPIPE ? 0.4f : 0.1f, NewConPart1, NewConIndex1, NewConPart2, NewConIndex2);
					}
				}
			}
			else
			{
				ConCreationStage = 0;
				NewConDebrie1 = false;
				NewConDebrie2 = false;
			}
			if (keys[GLFW_KEY_LEFT_SHIFT] && !keys[GLFW_KEY_LEFT_ALT])
			{
				if (JustPressedLMB && in_UI <= 0)
				{
					bool detached = false;
					for (int i = 1; i < Entities[0]->Parts.size(); i++)
					{

						for (int a = 0; a < Entities[0]->Parts[i]->body.size(); a++)
						{

							glm::vec2 dif = ballPosition[Entities[0]->Parts[i]->body[a]] - MousePosition;

							if (i < Entities[0]->Parts.size() && sqrlength(dif) < PARTSIZE * PARTSIZE)
							{
								playsound(Detach, MousePosition, 0.3f, 3.5f);
								Entities[0]->DetachPart(i);
								detached = true;
								break;
							}
						}
						if (detached)
							break;
					}
				}
			}

			if (JustPressedbutton[GLFW_MOUSE_BUTTON_3])
			{
				bool found = false;
				for (int i = 0; i < Debris.Parts.size(); i++)
				{
					if (Debris.Parts[i]->bodysize > 1)
					{
						for (int a = 0; a < Debris.Parts[i]->bodysize; a++)
						{
							glm::vec2 dif = ballPosition[Debris.Parts[i]->body[a]] - MousePosition;
							if (sqrlength(dif) < PARTSIZE * PARTSIZE)
							{

								found = true;
								SelectedPart = i;

								break;
							}
						}
					}

					if (found)
						break;
				}
			}
			if (JustReleasedbutton[GLFW_MOUSE_BUTTON_3])
				SelectedPart = -1;
			if (SelectedPart > 0)
			{
				glm::vec2 mid = { 0.0f,0.0f };
				for (int i = 0; i < Debris.Parts[SelectedPart]->bodysize; i++)
					mid += ballPosition[Debris.Parts[SelectedPart]->body[i]];
				mid /= Debris.Parts[SelectedPart]->bodysize;

				glm::vec2 dir = Normalize(MousePosition - mid);

				float rot = 0.0f;
				float rotStep = pi * 2.0f / Debris.Parts[SelectedPart]->bodysize;
				for (int i = 0; i < Debris.Parts[SelectedPart]->bodysize; i++)
				{
					ballPosition[Debris.Parts[SelectedPart]->body[i]] = mid + Rotate(dir, rot) * PARTSIZE;
					rot += rotStep;
				}

			}
			EndOfWindow();
			iw->Use();
			glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
			MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
			MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
			ScreenMousePosition = WindowMousePosition;
			LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
			foregroundMousePosition = MousePosition;

			bool b = (NewConType == STRUT);
			UI_buttonOnlyON(&b, "Strut", { -0.47f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
			if (b && b != (NewConType == STRUT))
				NewConType = STRUT;

			b = (NewConType == SPRING);
			UI_buttonOnlyON(&b, "Spring", { -0.47f * WIDTH ,-0.40f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
			if (b && b != (NewConType == SPRING))
				NewConType = SPRING;

			b = (NewConType == HEATPIPE);
			UI_buttonOnlyON(&b, "Heat pipe", { -0.47f * WIDTH ,-0.34f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
			if (b && b != (NewConType == HEATPIPE))
				NewConType = HEATPIPE;

			b = (NewConType == ROPE);
			UI_buttonOnlyON(&b, "Rope", { -0.47f * WIDTH ,-0.28f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
			if (b && b != (NewConType == ROPE))
				NewConType = ROPE;





			UI_CheckBox(&align, "Align rotation", { -0.35f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);

			UI_CheckBox(&snapToGrid, "Snap to grid", { -0.20f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);

			UI_TextBox(&saveFileName, { -0.46f * WIDTH ,0.46f * HEIGHT }, 32, { 15 * UISize,UISize * 1.0f }, TextSize, { 0.4f,0.4f,0.4f,0.4f }, { 1.0f,1.0f,1.0f,1.0f }, 1200);

			bool save = false;
			UI_buttonOnlyON(&save, "Save", { -0.46f * WIDTH ,0.40f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
			if (save)
				Entities[0]->SaveTo(saveFileName);

			bool load = false;
			UI_buttonOnlyON(&load, "Load", { -0.46f * WIDTH ,0.34f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
			if (load)
				Entities[0]->LoadFrom(saveFileName);

				
			iw->End();
			sw->Use();
			WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
			MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
			MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
			ScreenMousePosition = WindowMousePosition;
			LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
			foregroundMousePosition = MousePosition;

		}
	}

	/*if (!CurrnetMission.compleated || MissionSelectMenu.missionSelected)
	{
		if (MissionSelectMenu.missionSelected)
			DrawCircle(Entities[0]->mid + Normalize(MissionSelectMenu.missionPosition - Entities[0]->mid) * 10.0f, 5.0f, { 5.0f,1.0f,1.0f,1.0f }, 0, 0, 10000);
		else
			DrawCircle(Entities[0]->mid + Normalize(glm::vec2(0.0f, 0.0f) - Entities[0]->mid) * 10.0f, 5.0f, { 5.0f,1.0f,1.0f,1.0f }, 0, 0, 10000);
	}
	else
	{
		if (!MissionSelectMenu.Hub)
			DrawCircle(Entities[0]->mid + Normalize(-MissionSelectMenu.missionPosition - Entities[0]->mid) * 10.0f, 5.0f, { 1.0f,1.0f,5.0f,1.0f }, 0, 0, 10000);
		else
			DrawCircle(Entities[0]->mid + Normalize(glm::vec2(0.0f, 0.0f) - Entities[0]->mid) * 10.0f, 5.0f, { 1.0f,1.0f,5.0f,1.0f }, 0, 0, 10000);
	}*/

	//if (MissionSelectMenu.missionSelected && Entities.size() > 0 && sqrlength(Entities[0]->mid - MissionSelectMenu.missionPosition) < (4000 * 4000))
	//{
	//	CurrnetMission.story_mission = MissionSelectMenu.missionStory;
	//	CurrnetMission.size = MissionSelectMenu.missionSize;
	//	CurrnetMission.dificulty = MissionSelectMenu.missionDificulty;
	//	CurrnetMission.type = MissionSelectMenu.missionType;
	//	CurrnetMission.missionpos = MissionSelectMenu.missionPosition;
	//	MissionSelectMenu.missionSelected = false;
	//	MissionSelectMenu.Hub = false;
	//	glm::vec2 plPos = (Entities[0]->mid - MissionSelectMenu.missionPosition);
	//	CurrnetMission.Start();
	//	inbase = false;
	//	switchScene = false;
	//	OpenMenu = false;
	//	Entities[0]->mid = plPos;
	//	loadedThisFrame = true;
	//}
	//if (!MissionSelectMenu.Hub && Entities.size() > 0 && sqrlength(-MissionSelectMenu.missionPosition - Entities[0]->mid) < (2000 * 2000))
	//{
	//	MissionSelectMenu.missionSelected = false;
	//	MissionSelectMenu.Hub = true;
	//	glm::vec2 plPos = (Entities[0]->mid + MissionSelectMenu.missionPosition);
	//	Materials += GetShipCost(Entities[0]);
	//	Entities[0]->SaveTo(EntityBackUpName);
	//	glm::vec2 vel = { 0.0f,0.0f };
	//	if (Entities.size() > 0)
	//	{
	//		vel = Entities[0]->avgvel;
	//		Entities[0]->SaveTo(EntityBackUpName);
	//	}
	//	inbase = true;
	//	GetWindow(BackgroundWindowID)->w_DirectionalLight = 1.0f;
	//	ChangeMap("Scenes/base.sav", MissionSelectMenu.missionPosition, {0.0f,0.0f});
	//	Entities[0]->mid = plPos;
	//	loadedThisFrame = true;
	//}

	EndOfWindow();
	iw->Use();
	glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
	MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
	MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
	ScreenMousePosition = WindowMousePosition;
	LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
	foregroundMousePosition = MousePosition;

	//UI_DrawLine({ 0.34f * WIDTH ,0.46f * HEIGHT },{ 0.34f * WIDTH + 0.16f * WIDTH,0.46f * HEIGHT }, 10.0f,glm::vec4(0.1f,0.1f,0.1f,1.0f),0,0,10000);
	//UI_DrawLine({ 0.35f * WIDTH ,0.46f * HEIGHT },{ 0.35f * WIDTH + AccumulatedHeat/400.0f * 0.15f * WIDTH,0.46f * HEIGHT }, 8.0f,glm::vec4(AccumulatedHeat / 40.0f,AccumulatedHeat / 200.0f,AccumulatedHeat / 300.0f,1.0f),0,0,10000);

	bool blm = bLogicMode;
	bool flm = fLogicMode;
	bool vlm = vLogicMode;
	bool bm = BuildingMode;

	if (JustPressedkey[GLFW_KEY_B])
	{
		BuildingMode = !BuildingMode;
		BuildingSwitched = true;
	}
	else BuildingSwitched = false;

	if (Entities.size() > 0)
		ActiveRadar.playerHeat = Entities[0]->avgheat;

	foregroundFogParticleAmount = 0.0f;
	bool forcenofog = false;
	for (int i = 0; i < ActiveRadar.bleeps.size(); i++)
	{
		if (Entities.size() > 0)
		{
			if (ActiveRadar.bleeps[i].state == 1 &&
				1000.0f > length(ActiveRadar.bleeps[i].position - (Entities[0]->mid + ActiveRadar.offset)))
			{
				ActiveRadar.bleeps[i].t = -10.0f;
				ActiveRadar.bleeps[i].infinite = false;
				CentralPart* cp = SpawnAiShip(ActiveRadar.bleeps[i].position - ActiveRadar.offset, ActiveRadar.bleeps[i].SaveString,true);
				cp->questid = ActiveRadar.bleeps[i].questid;
				if(cp->questid <0)cp->AIState = 1;
			}
			if (ActiveRadar.bleeps[i].state == 0 &&
				1000.0f + ActiveRadar.bleeps[i].r * 100.0f > length(ActiveRadar.bleeps[i].position - (Entities[0]->mid + ActiveRadar.offset)))
			{
				if (foregroundFogParticleAmount < ActiveRadar.bleeps[i].fogamount)
					foregroundFogParticleAmount = ActiveRadar.bleeps[i].fogamount;
			}
			if (ActiveRadar.bleeps[i].state == -1 &&
				5000.0f > length(ActiveRadar.bleeps[i].position - (Entities[0]->mid + ActiveRadar.offset)))
			{

				if (ActiveRadar.enteredBleep != i)
				{
					if (ActiveRadar.enteredBleep >= 0 && ActiveRadar.enteredBleep < ActiveRadar.bleeps.size())
					{
						std::string savestr = "Saves/" + GameSaveName + "/bleeps/" + std::to_string(ActiveRadar.BleepsCounter);
						CurrnetMission.Save(savestr);
						ActiveRadar.bleeps[ActiveRadar.enteredBleep].SaveString = savestr;
						ActiveRadar.BleepsCounter++;
					}
					ActiveRadar.enteredBleep = i;
					MissionSelectMenu.missionSelected = false;
					CurrnetMission.story_mission = false;
					CurrnetMission.size = 0.0f;
					CurrnetMission.dificulty = 0.0f;
					CurrnetMission.type = -1;
					MissionSelectMenu.Hub = true;
					CurrnetMission.missionpos = ActiveRadar.bleeps[i].position;
					inbase = true;
					switchScene = false;
					OpenMenu = false;
					loadedThisFrame = true;
					if(ActiveRadar.bleeps[i].SaveString == "")
						CurrnetMission.Start();
					else
					{
						CurrnetMission.Load(ActiveRadar.bleeps[i].SaveString);
					}
				}
			}
			if (ActiveRadar.bleeps[i].state == 2 &&
				5000.0f > length(ActiveRadar.bleeps[i].position - (Entities[0]->mid + ActiveRadar.offset)))
			{

				forcenofog = true;
				if (ActiveRadar.enteredBleep!=i)
				{
					if (ActiveRadar.enteredBleep >= 0 && ActiveRadar.enteredBleep < ActiveRadar.bleeps.size())
					{
						std::string savestr = "Saves/" + GameSaveName + "/bleeps/" + std::to_string(ActiveRadar.BleepsCounter);
						CurrnetMission.Save(savestr);
						ActiveRadar.bleeps[ActiveRadar.enteredBleep].SaveString = savestr;
						ActiveRadar.BleepsCounter++;
					}
					ActiveRadar.enteredBleep = i;
					CurrnetMission.story_mission = true;
					CurrnetMission.size = ActiveRadar.bleeps[i].size;
					CurrnetMission.dificulty = ActiveRadar.bleeps[i].dificulty;
					CurrnetMission.type = ActiveRadar.bleeps[i].state - 3;
					CurrnetMission.questid = ActiveRadar.bleeps[i].questid;
					CurrnetMission.materialReward = ActiveRadar.bleeps[i].materialReward;
					CurrnetMission.missionpos = ActiveRadar.bleeps[i].position;
					MissionSelectMenu.missionPosition = ActiveRadar.bleeps[i].position;
					MissionSelectMenu.Hub = false;
					glm::vec2 plPos = (Entities[0]->mid - MissionSelectMenu.missionPosition);
					CurrnetMission.outside = false;
					inbase = false;
					switchScene = false;
					OpenMenu = false;
					loadedThisFrame = true;
					if (ActiveRadar.bleeps[i].SaveString == "")
						CurrnetMission.Start();
					else
					{
						CurrnetMission.Load(ActiveRadar.bleeps[i].SaveString);
					}
				}
			}
			// side missions
			if (ActiveRadar.bleeps[i].state > 2 &&
				3000.0f > length(ActiveRadar.bleeps[i].position - (Entities[0]->mid + ActiveRadar.offset)))
			{

				if (ActiveRadar.enteredBleep != i)
				{
					if (ActiveRadar.enteredBleep  >= 0 && ActiveRadar.enteredBleep < ActiveRadar.bleeps.size())
					{
						std::string savestr = "Saves/" + GameSaveName + "/bleeps/" + std::to_string(ActiveRadar.BleepsCounter);
						CurrnetMission.Save(savestr);
						ActiveRadar.bleeps[ActiveRadar.enteredBleep].SaveString = savestr;
						ActiveRadar.BleepsCounter++;
					}

					ActiveRadar.enteredBleep = i;
					CurrnetMission.story_mission = false;
					
					
					
					

					CurrnetMission.size = ActiveRadar.bleeps[i].size;
					CurrnetMission.dificulty = ActiveRadar.bleeps[i].dificulty;
					CurrnetMission.type = ActiveRadar.bleeps[i].state - 3;
					CurrnetMission.questid = ActiveRadar.bleeps[i].questid;
					CurrnetMission.materialReward = ActiveRadar.bleeps[i].materialReward;
					CurrnetMission.missionpos = ActiveRadar.bleeps[i].position;
					MissionSelectMenu.missionPosition = ActiveRadar.bleeps[i].position;
					MissionSelectMenu.Hub = false;
					glm::vec2 plPos = (Entities[0]->mid - MissionSelectMenu.missionPosition);
					CurrnetMission.outside = false;
					inbase = false;
					switchScene = false;
					OpenMenu = false;
					loadedThisFrame = true;
					if (ActiveRadar.bleeps[i].SaveString == "")
						CurrnetMission.Start();
					else
					{
						CurrnetMission.Load(ActiveRadar.bleeps[i].SaveString);
					}
				}
			}
		}
	}
	if (inbase && Entities.size()>0 && sqrlength(Entities[0]->mid) < 5000 * 5000)
		forcenofog = true;
	if (forcenofog)
	{
		foregroundFogParticleAmount = 0.0f;
	}
	ActiveRadar.offset = CurrnetMission.missionpos;
	ConsoleTexts.clear();
	for (int i = 1; i < Entities.size(); i++)
	{
		if (sqrlength(Entities[i]->mid - (Entities[0]->mid)) > 2500 * 2500 && Entities[i]->Parts.size()>1 && !Entities[i]->Delete && Entities[i]->saveid<0)
		{
			Radar::Bleep bl;
			bl.state = 1;
			bl.position = ActiveRadar.offset + Entities[i]->mid;
			bl.velocity = Entities[i]->avgvel;
			bl.infinite = true;
			bl.r = 5.0f;
			bl.fogamount = 0.0f;
			bl.questid = Entities[i]->questid;
			std::string savestr = "Saves/" + GameSaveName + "/bleeps/" + std::to_string(ActiveRadar.BleepsCounter) + "/ship.sav";
			std::filesystem::create_directories("Saves/" + GameSaveName + "/bleeps/" + std::to_string(ActiveRadar.BleepsCounter));
			ActiveRadar.BleepsCounter++;
			Entities[i]->SaveTo(savestr,false,true);

			Entities[i]->Clear();
			Entities[i]->Destroy();
			bl.SaveString = savestr;
			ActiveRadar.bleeps.push_back(bl);
		}
	}

	if (!CurrnetMission.outside && sqrlength(Entities[0]->mid) > 7000.0f * 7000.0f)
	{
		if (ActiveRadar.enteredBleep >= 0 && ActiveRadar.enteredBleep < ActiveRadar.bleeps.size())
		{
			std::string savestr = "Saves/" + GameSaveName + "/bleeps/" + std::to_string(ActiveRadar.BleepsCounter);
			CurrnetMission.Save(savestr);
			ActiveRadar.bleeps[ActiveRadar.enteredBleep].SaveString = savestr;
			ActiveRadar.BleepsCounter++;

		}

		ActiveRadar.enteredBleep = -1;
		CurrnetMission.story_mission = false;
		CurrnetMission.size = 0;
		CurrnetMission.dificulty = 0;
		CurrnetMission.type = -1;
		CurrnetMission.questid = -1;
		CurrnetMission.missionpos = (Entities[0]->mid + ActiveRadar.offset);
		MissionSelectMenu.missionPosition = (Entities[0]->mid + ActiveRadar.offset);
		MissionSelectMenu.Hub = false;
		CurrnetMission.outside = true;
		inbase = false;
		switchScene = false;
		OpenMenu = false;
		loadedThisFrame = true;
		CurrnetMission.MissionClear();
		ChangeMap("0", ActiveRadar.offset, CurrnetMission.missionpos);
	}

	ActiveRadar.Process(delta);
	//ActiveRadar.Draw({ 0.5f * WIDTH - 120.0f,0.5f * HEIGHT - 120.0f });
	//JournalWindowPos = SmootherStep(jw->ViewportSize.x * 1.5f
	ActiveRadar.Draw({ 0.5f * WIDTH - 120.0f + JournalWindowPos  - jw->ViewportSize.x * 1.1f,0.5f * HEIGHT - 150.0f });

	ConsoleTexts.push_back("Materials: " + std::to_string(Materials));
	for (auto q : quests)
	{
		quests[q.first].Process(delta);
	}
	for (auto q : quests)
	{
		if (q.second.flags["Failed"])
			quests.erase(q.first);
		else if (q.second.flags["Done"])
		{
			Materials += q.second.materialreward;
			quests.erase(q.first);
		}
	}
	
	UI_CheckBox(&bLogicMode, "Bools Logic", { 0.40f * WIDTH ,-0.46f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
	UI_CheckBox(&fLogicMode, "Number Logic", { 0.40f * WIDTH ,-0.40f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
	UI_CheckBox(&vLogicMode, "Vector Logic", { 0.40f * WIDTH ,-0.34f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);
	UI_CheckBox(&BuildingMode, "Building Mode", { 0.40f * WIDTH ,-0.28f * HEIGHT }, UISize, TextSize, UI_ColorON, UI_ColorOFF, 1200);

	iw->End();
	sw->Use();
	WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
	MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
	MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
	ScreenMousePosition = WindowMousePosition;
	LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
	foregroundMousePosition = MousePosition;

	if (loadedThisFrame)
	{
		camerapos = (Entities[0]->mid + MissionSelectMenu.missionPosition);
		CameraPosition = (Entities[0]->mid + MissionSelectMenu.missionPosition);
	}
	if (bLogicMode && blm != bLogicMode)
	{
		bLogicMode = true;
		fLogicMode = false;
		vLogicMode = false;
		BuildingMode = false;
		DataconnectionData[0] = 0;
	}
	else if (fLogicMode && flm != fLogicMode)
	{
		bLogicMode = false;
		fLogicMode = true;
		vLogicMode = false;
		BuildingMode = false;
		DataconnectionData[0] = 0;
	}
	else if (vLogicMode && vlm != vLogicMode)
	{
		bLogicMode = false;
		fLogicMode = false;
		vLogicMode = true;
		BuildingMode = false;
		DataconnectionData[0] = 0;
	}
	else if (BuildingMode && bm != BuildingMode)
	{
		bLogicMode = false;
		fLogicMode = false;
		vLogicMode = false;
		BuildingMode = true;
		DataconnectionData[0] = 0;
	}

	if (GrabbedBall != NULL)
	{
		point p;
		p.position = MousePosition;
		ball tmpb;
		tmpb.position = ballPosition[GrabbedBall];
		tmpb.velocity = ballVelocity[GrabbedBall];
		tmpb.Force = ballForce[GrabbedBall];
		SpringBallToPoint(&tmpb, &p, 0.0f, 3000.0f, 1.0f);
		ballPosition[GrabbedBall] = tmpb.position;
		ballVelocity[GrabbedBall] = tmpb.velocity;
		ballForce[GrabbedBall] = tmpb.Force;
		DrawLine(ballPosition[GrabbedBall], MousePosition, 0.025f, { 1.0f,1.0f,0.0f,1.0f });

		DamageSphere DS;
		DS.body.r = 0.25f;
		DS.body.position = MousePosition;
		DS.Damage = false;
		DS.timeLeft = 0.044f;
		DS.Heat = 0.0f;
		DS.recoil = -1.0f;
		DS.neutral = true;
		DS.friction = 0.99f;
		DS.singleHit = false;
		DamageSpheresArray.push_back(DS);

	}

	if(CurrnetMission.exitedmission)
	{
		MissionSelectMenu.GenerateNewMissions();
		CurrnetMission.exitedmission = false;
	}
	
	CurrnetMission.Process(delta);
	

}

void ProcessMainMenu()
{

	// -900;280


	Window* sw = GetWindow(ForeWindowID);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* mw = GetWindow(MenuWindowID);
	
	mw->Use();
	
	glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
	MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
	MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
	LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
	ScreenMousePosition = WindowMousePosition;

	glm::vec2 Corner = { WIDTH * -0.45f,HEIGHT *0.35f };
	float step = 10.0f;
	glm::vec4 textColor = { 2.0f,2.0f,2.0f,1.0f };
	glm::vec4 textColorOff = { 1.0f,1.0f,1.0f,1.0f };
	int textZ = 2000;
	if (OpenMenu || SettingsWindow)
	{
		UI_DrawCube({ 0.0,0.0f }, { WIDTH * 1.0f,HEIGHT * 1.0f }, 0.0f, { 0.0f, 0.0f, 0.0f, 0.8f }, false, NULL, textZ - 100);
	}

	bool b = false;

	Corner.y += UI_DrawText("HEAT", Corner, TextSize*2.0f, textColor, textZ).y * -1.0f -step;
	
	Corner.y += UI_CheckBox(&b,"Start", Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
	if (b)
	{
		Exposure = -0.85f;
		switchScene = true;
	}
	if (Exposure > 0.0f && switchScene)
	{
		mw->End();
		sw->Use();

		if(storyint >0)
		{
			ChangeMap("Scenes/basev2.sav", { 0.0f,0.0f }, { 0.0f,0.0f });

			SpawnPlayer();
			Background.LoadFrom("Scenes/Sun.sav");

			CurrnetMission.story_mission = false;
			inbase = true;
			switchScene = false;
			OpenMenu = false;
		}
		else
		{
			CurrnetMission.story_mission = true;
			CurrnetMission.Start();	
			inbase = false;
			switchScene = false;
			OpenMenu = false;
		}
		SaveSettings();
		sw->End();
		mw->Use();
		
		glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		ScreenMousePosition = WindowMousePosition;
	}

	Corner.y += UI_CheckBox(&Settings, "Settings", Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
	
	if (Settings)
	{
		glm::vec2 s_Corner = { WIDTH * -0.115f,HEIGHT * 0.35f };
		bool b = false;

		s_Corner.y += UI_DrawText("Settings", s_Corner, TextSize * 2.0, textColor, textZ).y * -1.0f - step;

		s_Corner.y += UI_CheckBox(&bloom, "Bloom", s_Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		s_Corner.y += UI_CheckBox(&disablefog, "disable fog", s_Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		s_Corner.y += UI_Slider(&brightness, "Brightness", s_Corner, 0.1f,1.5f, { 10 * UISize,UISize * 0.35f }, TextSize, textColor, textColor, textZ).y * -1.0f - step;

		s_Corner.y += UI_CheckBox(&b, "Reset to default", s_Corner , UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		if (b)
			ResetSettings();

		s_Corner.y += UI_CheckBox(&s_Fullscreen, "Full screen", s_Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		b = false;
		float stepX = UI_CheckBox(&b, "-", s_Corner , UISize, TextSize, textColor, textColorOff, textZ).x * 0.5f + step;
		if (b)
		{
			SRiter--;
			if (SRiter < 0)
				SRiter = 7;
			s_Resolution = StandartResolutions[SRiter];

			RX = std::to_string(s_Resolution.x);
			RY = std::to_string(s_Resolution.y);
			RX.resize(4);
			if (RX[3] == '.')
				RX.pop_back();

			RY.resize(4);
			if (RY[3] == '.')
				RY.pop_back();
		}


		std::string BRX = RX;
		std::string BRY = RY;
		stepX += UI_TextBox(&RX, s_Corner + glm::vec2(stepX, 0.0f), -1, { 5 * UISize,UISize * 1.0f }, TextSize, textColorOff, textColor , textZ).x * 0.5f + step;
		
		stepX += UI_TextBox(&RY, s_Corner + glm::vec2(stepX, 0.0f), -1, { 5 * UISize,UISize * 1.0f }, TextSize, textColorOff, textColor, textZ).x * 0.5f + step;
	

		if (BRX != RX || BRY != RY)
			SRiter = 0;
		b = false;
		s_Corner.y += UI_CheckBox(&b,"+", s_Corner + glm::vec2(stepX, 0.0f), UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
		if(b)
		{
			SRiter++;
			if (SRiter > 7)
				SRiter = 0;
			s_Resolution = StandartResolutions[SRiter];

			RX = std::to_string(s_Resolution.x);
			RY = std::to_string(s_Resolution.y);
			RX.resize(4);
			if (RX[3] == '.')
				RX.pop_back();

			RY.resize(4);
			if (RY[3] == '.')
				RY.pop_back();
		}
	

		s_Corner.y += -20 - step;
		s_Corner.y += UI_DrawText("Resolution or Full screen changes require a reload", s_Corner, TextSize, textColor, textZ).y * -1.0f - step;


		s_Corner.y += UI_Slider(&s_ChromaticAbberation, "Chromatic abberation", s_Corner, 0.0f, 1.0f, { 10 * UISize,UISize * 0.35f }, TextSize, textColor, textColor, textZ).y * -1.0f - step;

		s_Corner.y += UI_Slider(&s_ScreenShake, "Screen shake", s_Corner, 0.0f, 1.0f, { 10* UISize,UISize * 0.35f }, TextSize, textColor, textColor, textZ).y * -1.0f - step;
		/*b = false;
		s_Corner.y += UI_CheckBox(&b, "Save", s_Corner, 25.0f, 1.0f).y * -1.0f - step;
		if (b)
			SaveSettings();*/



	}
	
	
	
	
	b = false;
	Corner.y += UI_CheckBox(&b, "Exit", Corner, UISize, TextSize, textColor, textColorOff, textZ).y * -1.0f - step;
	if (b)
	{
		SaveSettings();
		glfwSetWindowShouldClose(window, true);
	}
	mw->End();
}


void Ready()
{
	
	std::cout<<"ready begins\n";
	
	ScreenShake = 0.0f;
	ChromaticAbberation = 0.0f;
	ScreenShakeFallOff = 1.0f;
	ChromaticAbberationFallOff = 1.0f;
	LoadTextures();

	

	noize = NULL;
	GenNoizeTexture(&noize, 100, 2, 1.0f, 2);

	GenPrimitiveTexture(&PrimitiveQuad.Texture,5,QUAD);
	//LoadFont("engine/fonts/LiberationSansR.ttf", 128);

	
	GenPrimitiveTexture(&MO_Ball.mater.Texture,1000,CIRCLE,true);
	GenPrimitiveTexture(&MO_Cube.mater.Texture,5,QUAD);
	MO_Ball.mater.NormalMap = BallNormalMapTexture;
	MO_Cube.mater.NormalMap = CubeNormalMapTexture;

	float maxsize = fmaxf(WIDTH, HEIGHT);
	TextSize = maxsize * 0.0005f * 0.35f;
	UISize = 25 * maxsize * 0.0005f;

	RX = std::to_string(s_Resolution.x);
	RY = std::to_string(s_Resolution.y);
	RX.resize(4);
	if (RX[3] == '.')
		RX.pop_back();

	RY.resize(4);
	if (RY[3] == '.')
		RY.pop_back();

	soundscale = { 40.0f,40.0f ,1.0f };

	std::cout<<"loading sounds ready\n";
	shopmenu.window = CreateWindow();
	SaveScreenmenu.window = CreateWindow();
	MissionSelectMenu.window = CreateWindow();

	Window* shw = GetWindow(shopmenu.window);
	shw->Init({570,600});
	shw->backgroundColor = {0.0f,0.0f,0.0f,1.0f};
	shw->Position = {-100,50};
	
	Window* sshw = GetWindow(SaveScreenmenu.window);
	sshw->Init({570,600});
	sshw->backgroundColor = {0.0f,0.0f,0.0f,1.0f};
	sshw->Position = {-100,250};

	
	Window* mshw = GetWindow(MissionSelectMenu.window);
	mshw->Init({570,600});
	mshw->backgroundColor = {0.0f,0.0f,0.0f,1.0f};
	mshw->Position = {-100,250};

	RocketEngineSound = LoadSound("Sounds/noize_L.wav");
	SHHSound = LoadSound("Sounds/wind.wav");
	GunSound = LoadSound("Sounds/HeavyHit.wav");
	HeavyHit = LoadSound("Sounds/BigShot.wav");
	LaserGunSound = LoadSound("Sounds/Laser.wav");
	Clang = LoadSound("Sounds/metalhit1.wav");
	Detach = LoadSound("Sounds/detach.wav");
	Scratch = LoadSound("Sounds/Scratch.wav");
	PartDestrSOund = LoadSound("Sounds/Explodion.wav");
	Hit = LoadSound("Sounds/hit.wav");
	BulletHit = LoadSound("Sounds/CleanExplodion.wav");
	ExplodionSound = LoadSound("Sounds/VineBoom.wav");
	MiniGunSound = LoadSound("Sounds/Minigun.wav");
	
	SetupPEs();
	shipNames.clear();
	shipNames.push_back("Bigboy");
	shipNames.push_back("Cube");
	shipNames.push_back("Rotator");
	shipNames.push_back("Save0");
	shipNames.push_back("Save1");
	shipNames.push_back("Spinner");


	std::cout<<"loaded sounds ready\n";
	//SpawnPlayer();

	substeps = 20;
	listenerPos.z = 2.0f;

	

	glfwSwapInterval(0);
	VSync = 0;


	ForeWindowID = CreateWindow();
	BackgroundWindowID = CreateWindow();
	MenuWindowID = CreateWindow();
	InterfaceWindowID = CreateWindow();
	ForeGroundWindowID = CreateWindow();
	PreviewWindowID = CreateWindow();
	JournalWindowID = CreateWindow();

	Window* bw = GetWindow(BackgroundWindowID);
	Window* sw = GetWindow(ForeWindowID);
	Window* fw = GetWindow(ForeGroundWindowID);
	Window* cw = GetWindow(InterfaceWindowID);
	Window* mw = GetWindow(MenuWindowID);
	Window* pw = GetWindow(PreviewWindowID);
	Window* jw = GetWindow(JournalWindowID);

	sw->AutoActive = false;
	bw->AutoActive = false;
	fw->AutoActive = false;
	cw->AutoActive = false;
	mw->AutoActive = false;
	pw->AutoActive = false;
	jw->AutoActive = true;

	sw->hdr = true;
	bw->hdr = true;
	fw->hdr = false;
	jw->hdr = false;

	sw->Init(GetWindow(SceneWindowID)->ViewportSize);
	bw->Init(GetWindow(SceneWindowID)->ViewportSize);
	fw->Init(GetWindow(SceneWindowID)->ViewportSize);
	cw->Init(GetWindow(SceneWindowID)->ViewportSize);
	mw->Init(GetWindow(SceneWindowID)->ViewportSize);
	pw->Init(glm::vec2(GetWindow(SceneWindowID)->ViewportSize.x * 0.2f));
	jw->Init(glm::vec2(GetWindow(SceneWindowID)->ViewportSize.x * 0.3f, GetWindow(SceneWindowID)->ViewportSize.y * 0.6f));


	sw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	bw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	fw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	cw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	mw->backgroundColor = { 0.0f,0.0f,0.0f,0.0f };
	pw->backgroundColor = { 0.0f,0.0f,0.0f,1.0f };
	jw->backgroundColor = { 0.0f,0.0f,0.0f,1.0f };

	EditorColor = {0.0f,0.0f,0.0f,1.0f};
	sw->w_AmbientLight = 0.4f;
	bw->w_AmbientLight = 0.1f;
	fw->w_AmbientLight = 0.1f;
	cw->w_AmbientLight = 1.4f;
	sw->w_DirectionalLight = 2.0f;
	pw->w_AmbientLight = 1.0f;
	jw->w_AmbientLight = 1.0f;

	


	for (int i = 0; i < StarsAmount; i++)
	{
		Stars[i].x = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.x;
		Stars[i].y = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.y;
		Stars[i].z = rand() % 1000 * 0.0001f * StarsDepth;
		Stars[i].w = rand() % 1000 * 0.001f * pi * 0.5f;


		BackgroundStars[i].x = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.x;
		BackgroundStars[i].y = rand() % 1000 * 0.001f * StarsSpread - StarsSpread * 0.5f + camerapos.y;
		BackgroundStars[i].z = rand() % 1000 * 0.0001f * StarsDepth;
		BackgroundStars[i].w = rand() % 1000 * 0.001f * pi * 0.5f;
	}
	
	std::cout<<"loading pData ready\n";
	LoadPlayerData();
	ChangeMap(MapFileName, { 0.0f,0.0f }, { 0.0f,0.0f });
	MainMenu = true;

	addsound(SHHSound, true, 10);
	addsound(RocketEngineSound, true, 4);
	addsound(MiniGunSound, true, 10);
	addsound(ExplodionSound, false, 10);
	addsound(BulletHit, false, 10);
	addsound(Hit, false, 4);
	addsound(PartDestrSOund, false, 10);
	addsound(Scratch, true, 10);
	addsound(Detach, false, 10);
	addsound(Clang, false, 10);
	addsound(LaserGunSound, true, 10);
	addsound(HeavyHit, false, 10);
	addsound(GunSound, false, 10);
	std::cout<<"added all sounds ready\n";
	//150 sources used

	MissionSelectMenu.GenerateNewMissions();
	SetupInstances();
	VSync = 1;


	GrabbedBall = -1;
	SelectedPart = -1;
	
	BuildingMode = false;
	bLogicMode = false;
	fLogicMode = false;
	vLogicMode = false;
	DataconnectionData[6] = {0};
	Speed = 1.0f;
	ActiveRadar.Init();
	std::cout<<"ready\n";

}
void SubSteppedProcess(float dt, int SubStep)
{
	Window* sw = GetWindow(ForeWindowID);
	sw->Use();
	
	ProcessEntities(dt, SubStep);

	sw->End();
	UseWindow(SceneWindowID);

	for (int i = 0; i < Sounds.size(); i++)
		Sounds[i].Update();
}

void Process(float dt)
{
	if (clock() < 100)
		Speed = 0.0f;
	else if (clock() >= 100 && clock() < 1200)
		Speed = 1.0f;

	if (dt > 0.017f)
		dt = 0.017f;

	dt *= Speed;
	in_UI--;

	if (in_UI < 0)
		in_UI = 0;

	Window* sw = GetWindow(ForeWindowID);
	Window* fw = GetWindow(ForeGroundWindowID);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* cw = GetWindow(InterfaceWindowID);
	Window* mw = GetWindow(MenuWindowID);
	Window* pw = GetWindow(PreviewWindowID);
	Window* jw = GetWindow(JournalWindowID);

	sw->active = true;

	sw->Use();
	glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
	MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
	MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
	ScreenMousePosition = WindowMousePosition;
	LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
	foregroundMousePosition =MousePosition; 
	

	if (JustReleasedkey[GLFW_KEY_ESCAPE])
		OpenMenu = !OpenMenu;

	if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
		LastJustPressedLMBScrMousePos = { 0.0f,0.0f };

	ImGui::Begin("Settings");
	ImGui::SliderFloat("speed", &Speed, 0.0f, 10.0f);
	ImGui::Text("ballssize = %.i", lastid);
	ImGui::Text("%.1ffps (%.3fms)", 1.0f / delta, delta);
	ImGui::Text("%.3fms DrawTime", DrawTime);
	ImGui::Text("%.3fms UpdateTime", UpdateTime);
	ImGui::Text("%.i channels count", channelcount);


	


	ImGui::Text("sources size (%.i)", sources.size());




	
	reloadSources = false;
	bool res = false;
	for (int i = 0; i < sources.size(); i++)
	{
		if (*sources[i] < 0)
			res = true;

	}

	ImGui::Text("parts size (%.i)", Debris.Parts.size());

	ImGui::Text("Camera position { %.2f ; %.2f }", CameraPosition.x, CameraPosition.y);
	ImGui::Text("Mission position { %.2f ; %.2f }", CurrnetMission.missionpos.x, CurrnetMission.missionpos.y);
	ImGui::Text("Exposure { %.2f }", Exposure );
	
	if (ImGui::Button("inside"))
	{
		inside = !inside;
	}
	ImGui::SliderFloat("AmbientLight", &AmbientLight, 0.0f, 1.0f);
	ImGui::SliderFloat("Camera", &AqueredCameraScale.x, 1.0f, 80.0f);
	AqueredCameraScale.y = AqueredCameraScale.x;


	if (ImGui::Button("VSync"))
	{
		if (VSync)
		{
			glfwSwapInterval(0);
			VSync = 0;
		}
		else
		{
			glfwSwapInterval(1);
			VSync = 1;
		}
	}
	//float a[2] = { Entities[0].direction.x,Entities[0].direction.y };
	//ImGui::DragFloat2("Direction", a);
	ImGui::Text("zoom = %.2f", CameraScale.x);
	ImGui::End();


	int currentObject = 1;
	
	//balls.clear();


	
	
	if (!Settings) {
		if (keys[GLFW_KEY_1])
		{

			DamageSphere DS;
			DS.body.r = 10.25f;
			DS.body.position = MousePosition;
			DS.Damage = false;
			DS.timeLeft = 0.044f;
			DS.Heat = 0.0f;
			DS.recoil = -1.0f;
			DS.neutral = true;
			//DS.friction = 0.00f;
			DS.singleHit = false;
			DamageSpheresArray.push_back(DS);
		}
		if (JustPressedkey[GLFW_KEY_2])
		{
			CentralPart* ent = SpawnAiShip(MousePosition,"Save0.sav");
			ent ->trgPos = MousePosition;
			ent ->AIState = 1;
			//Entities[Entities.size() - 1]->CP.Health = -10;

		}
		if (JustPressedkey[GLFW_KEY_3])
		{
			CentralPart* ent = SpawnAiShip(MousePosition, "Bigboy");
			ent->trgPos = MousePosition;
			ent->AIState = 1;
			//Entities[Entities.size() - 1]->CP.Health = -10;

		}
		if (JustPressedkey[GLFW_KEY_4])
		{
			SpawnExplodion(MousePosition, 10.0f, 0.5f, 0.4f);
		}
	}
	

	ProcessPE(dt);

	ProcessLightEffects(dt);
	DrawExplodions(dt);
	loadedThisFrame = false;
	if (!OpenMenu && !MainMenu)
		ProcessPlayerControls();


	if (Entities.size() > 0 && !Entities[0]->dead && !Entities[0]->destroyed)
		camerapos = Entities[0]->mid;
	

	if (Entities.size() > 0)
	{
		listenerVel.x = Entities[0]->avgvel.x;
		listenerVel.y = Entities[0]->avgvel.y;
		listenerPos.x = Entities[0]->mid.x;
		listenerPos.y = Entities[0]->mid.y;
		ListenerPos = { listenerPos.x, listenerPos.y };
		UpdateListenerPosition();
	}
	else
	{

		listenerVel.x = 0.0f;
		listenerVel.y = 0.0f;
		ListenerPos = { listenerPos.x, listenerPos.y };
		UpdateListenerPosition();
	}
	
	sw->End();
	ProcessCamera(dt);
	sw->Use();
	// stars
	if (clock() > 250)
	{
		for (int i = 0; i < StarsAmount; i++)
		{
			if (Stars[i].x > CameraPosition.x + StarsSpread * 0.5f)
			{
				Stars[i].x = CameraPosition.x - StarsSpread * 0.5f;
				Stars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (Stars[i].x < CameraPosition.x - StarsSpread * 0.5f)
			{
				Stars[i].x = CameraPosition.x + StarsSpread * 0.5f;
				Stars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (Stars[i].y > CameraPosition.y + StarsSpread * 0.5f)
			{
				Stars[i].y = CameraPosition.y - StarsSpread * 0.5f;
				Stars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (Stars[i].y < CameraPosition.y - StarsSpread * 0.5f)
			{
				Stars[i].y = CameraPosition.y + StarsSpread * 0.5f;
				Stars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}

			DrawCube(glm::vec2(Stars[i]), glm::vec2(Stars[i].z * 0.25f), Stars[i].w, glm::vec4(1.0f, 1.0f, 1.0f, Stars[i].z * 10.0f / StarsDepth), false, 0, -1000);

		}
		sw->End();
		bw->Use();
		for (int i = 0; i < StarsAmount * 0.2; i++)
		{
			if (BackgroundStars[i].x > CameraPosition.x + StarsSpread * 0.5f)
			{
				BackgroundStars[i].x = CameraPosition.x - StarsSpread * 0.5f;
				BackgroundStars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (BackgroundStars[i].x < CameraPosition.x - StarsSpread * 0.5f)
			{
				BackgroundStars[i].x = CameraPosition.x + StarsSpread * 0.5f;
				BackgroundStars[i].y = CameraPosition.y + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (BackgroundStars[i].y > CameraPosition.y + StarsSpread * 0.5f)
			{
				BackgroundStars[i].y = CameraPosition.y - StarsSpread * 0.5f;
				BackgroundStars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}
			if (BackgroundStars[i].y < CameraPosition.y - StarsSpread * 0.5f)
			{
				BackgroundStars[i].y = CameraPosition.y + StarsSpread * 0.5f;
				BackgroundStars[i].x = CameraPosition.x + rand() % 10000 * 0.0001f * StarsSpread - StarsSpread * 0.5f;
			}

			DrawCube(glm::vec2(BackgroundStars[i] * 2.0f), glm::vec2(BackgroundStars[i].z * 1.25f), BackgroundStars[i].w, glm::vec4(1.0f, 1.0f, 1.0f, BackgroundStars[i].z * 10.0f / StarsDepth), false, 0, -1000);
		}
		bw->End();
		sw->Use();
		glm::vec2 WindowMousePosition = (GetWindow(SceneWindowID)->WindowMousePosition);
		MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
		MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
		ScreenMousePosition = WindowMousePosition;
		LastJustPressedLMBScrMousePos = GetWindow(SceneWindowID)->w_LastJustPressedLMBScrMousePos;
		foregroundMousePosition = MousePosition;
	}

	if(MissionSelectMenu.Hub && Entities.size()>0)
	{
		if(sqrlength(Entities[0]->mid)<600*600)
			inside = true;
		else
			inside = false;
	}

	/*if(!inside && Entities.size()>0)
	{
		AccumulatedHeat += (Entities[0]->sumheat - 5.0f) * dt * 0.1f;
		if(AccumulatedHeat<0.0f)
			AccumulatedHeat = 0.0f;
		
		if(AccumulatedHeat>400.0f)
		{
			AccumulatedHeat-=400.0f;
			SpawnAiShip(Entities[0]->mid + glm::vec2(0.0f,1000.0f),"Bigboy")->AIState = 1;
		}
	}
	else
	{
		AccumulatedHeat -= 10.0f * dt;
		if(AccumulatedHeat<0.0f)
			AccumulatedHeat = 0.0f;
	}*/
	
	GameScene->Draw(dt);
	int ent=0;
	
	while (ent < Entities.size())
	{
		if (!Entities[ent]->Alive && !Entities[ent]->destroyed || Entities[ent]->Health<=0.0f || Entities[ent]->Delete)
		{
			
			LightEffect le;
			le.volume = 0.005f;
			le.S_Color = { 1.0f,20.0f,200.0f,1.0f };
			le.S_Scale = 250.0f;
			le.time = 3.0f;
			le.maxT = 3.0f;
			le.position = glm::vec3(Entities[ent]->mid,0.0f);
			LightEffects.push_back(le);
			Entities[ent]->Destroy();
			Entities[ent]->Delete=true;
			Entities[ent] = Entities[Entities.size() - 1];
			Entities.pop_back();
		}
		else
			ent++;
	}
	CurrnetMission.CheckShips(dt);

	int lastball = lastid;
	for (int i =0; i < lastid; i++)
	{
		if (IsBall[i])
			lastball = i ;
	}

	sw->End();

	if (OpenMenu || MainMenu)
		ProcessMainMenu();


	bw->w_CameraPosition = (sw->w_CameraPosition + ActiveRadar.offset)*0.02f;

	bw->Use();
	Background.DrawCollisions = false;
	ProcessScene(&Background, true, false);
	Background.Draw(delta);

	bw->End();

	
	fw->w_CameraPosition = (sw->w_CameraPosition + ActiveRadar.offset) * 1.0f;
	fw->w_CameraScale = (sw->w_CameraScale * 0.75f);

	fw->Use();

	glm::vec2 fogbox = glm::vec2(WIDTH * 1.2f, HEIGHT * 1.2f);
	foregroundFogParticleT -= dt;
	if (foregroundFogParticleT <= 0.0f)
	{
		foregroundFog.SpawnInCube(fw->w_CameraPosition, fogbox, foregroundFogParticleAmount * 100);
		foregroundFogParticleT = 1.00f;
	}
	for (int i = 0; i < foregroundFog.Particles.size(); i++)
	{
		if (foregroundFog.Particles[i].position.x - CameraPosition.x < -fogbox.x)
		{
			foregroundFog.Particles[i].position.x = CameraPosition.x + fogbox.x ;
			foregroundFog.Particles[i].position.y = CameraPosition.y + fogbox.y  * (rand() % 1000 * 0.002f - 1.0f);
			foregroundFog.Particles[i].velocity = { 0.0f, 0.0f };
		}
		if (foregroundFog.Particles[i].position.x - CameraPosition.x > fogbox.x )
		{
			foregroundFog.Particles[i].position.x = CameraPosition.x + -fogbox.x ;
			foregroundFog.Particles[i].position.y = CameraPosition.y + fogbox.y  * (rand() % 1000 * 0.002f - 1.0f);
			foregroundFog.Particles[i].velocity = { 0.0f, 0.0f };
		}
		if (foregroundFog.Particles[i].position.y - CameraPosition.y < -fogbox.y )
		{
			foregroundFog.Particles[i].position.y = CameraPosition.y + fogbox.y ;
			foregroundFog.Particles[i].position.x = CameraPosition.x + fogbox.x  * (rand() % 1000 * 0.002f - 1.0f);
			foregroundFog.Particles[i].velocity = { 0.0f, 0.0f };
		}
		if (foregroundFog.Particles[i].position.y - CameraPosition.y > fogbox.y )
		{
			foregroundFog.Particles[i].position.y = CameraPosition.y + -fogbox.y ;
			foregroundFog.Particles[i].position.x = CameraPosition.x + fogbox.x  * (rand() % 1000 * 0.002f - 1.0f);
			foregroundFog.Particles[i].velocity = { 0.0f, 0.0f };
		}
	}

	foregroundFog.ShowWindow = false;
	foregroundFog.Process(dt);
	foregroundFog.SpheresOfInfluence.clear();
	foregroundFog.LightSpheres.clear();

	fw->End();

	bool b = false;
	cw->Use();
	ConsoleShow;
	ConsolePos;
	ConsolePosTimer;


	if (ConsoleShow)
	{
		ConsolePosTimer += dt * 1.5f;
		if (ConsolePosTimer > 1.0f)
			ConsolePosTimer = 1.0f;
	}
	else
	{
		ConsolePosTimer -= dt * 1.5f;
		if (ConsolePosTimer < 0.0f)
			ConsolePosTimer = 0.0f;
	}

	ConsolePos = SmootherStep(cw->ViewportSize.x*-0.3f, 0.0f, ConsolePosTimer);



	float ConsoleYStep = 15.0f;
	float ConsoleTextSize = 0.4f;
	glm::vec4 consoleTextColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);

	float posy = HEIGHT * 0.5f - ConsoleYStep * 4.0f;
	float posx = WIDTH * -0.5f + ConsoleYStep * 2.0f + ConsolePos;
	for(int i=0;i<ConsoleTexts.size();i++)
	{
		DrawText(ConsoleTexts[i],{posx,posy},ConsoleTextSize,consoleTextColor);
		posy-=ConsoleYStep;
	}
	

	cw->End();

	pw->active = true;
	pw->AutoDraw = PreviewShowWindow;

	if (PreviewShowWindow)
	{
		PreviewPosTimer += dt * 1.5f;
		if (PreviewPosTimer > 1.0f)
			PreviewPosTimer = 1.0f;
	}
	else
	{
		PreviewPosTimer -= dt * 1.5f;
		if (PreviewPosTimer < 0.0f)
			PreviewPosTimer = 0.0f;
	}
	
	PreviewPos = SmootherStep(-pw->ViewportSize.y * 1.0f, 0.0f, PreviewPosTimer);

	pw->Use();

	if (Entities.size() > 0 && PreviewShowWindow)
	{
		UI_CheckBox(&Previewheat, "Heat/Health", glm::vec2(-pw->ViewportSize.x * 0.5f + 15.0f, pw->ViewportSize.y * 0.5f - 25.0f));
		CameraPosition = Entities[0]->mid;
		CameraScale = glm::vec2(float(pw->ViewportSize.x * 0.4f) / Entities[0]->maxR);
		if (!Previewheat)
		{
			drawHealth = true;
			DirectionalLight = 0.0f;
		}
		else
		{
			drawHeat = true;
			DirectionalLight = 0.0f;
		}
		Entities[0]->Draw();
		for (auto p : Entities[0]->Parts)
			p->Draw();
	}

	drawHealth = false;
	drawHeat = false;
	pw->End();
	pw->Position = glm::vec2(pw->ViewportSize.x * 0.5f - sw->ViewportSize.x * 0.5f + 5.0f, pw->ViewportSize.y * 0.5f - sw->ViewportSize.y * 0.5f + 5.0f + PreviewPos);


	jw->AutoDraw = JournalWindowShow;

	if (JournalWindowShow)
	{
		JournalWindowPosTimer += dt * 1.5f;
		if (JournalWindowPosTimer > 1.0f)
			JournalWindowPosTimer = 1.0f;
	}
	else
	{
		JournalWindowPosTimer -= dt * 1.5f;
		if (JournalWindowPosTimer < 0.0f)
			JournalWindowPosTimer = 0.0f;
	}

	JournalWindowPos = SmootherStep(jw->ViewportSize.x * 1.1f, 0.0f, JournalWindowPosTimer);

	jw->Use();

	glm::vec2 Corner = { -jw->ViewportSize.x * 0.5f + 15.0f, jw->ViewportSize.y * 0.5 - 25.0f};
	glm::vec2 CornerAdd = { 0.0f, -JournalWindowScroll };
	if (JournalWindowShow)
	{
		//b = false;
		//UI_CheckBox(&b, "Journal", glm::vec2(-jw->ViewportSize.x * 0.5f + 15.0f, jw->ViewportSize.y * 0.5f - 25.0f));
		//if (b)
		//{
		//
		//}
		Corner.y += UI_DrawText("Quests:", Corner + CornerAdd, 0.45f).y * -1.0f;
	

		for (auto q : quests)
		{
			b = false;
			UI_DrawText(q.second.name.c_str(), Corner + CornerAdd - glm::vec2(0.0f, 5.0f), 0.35f);
			UI_DrawCube(Corner + CornerAdd + glm::vec2(250 * 0.5f, 0.0f), glm::vec2(250.0f, 20.0f) * 0.5f, 0.0f, glm::vec4(0.07f));
			Corner.y += UI_button(&b, "", Corner + CornerAdd, { 250,20 }, 0.35f, glm::vec4(0.0f), glm::vec4(0.5f), glm::vec4(0.0f)).y * -1.0f - 0;
			if (b)
			{
				JournalSelectedQuest = q.second.id;
			}
		}
	}

	JournalWindowScroll += scrollmovement * 50.0f;


	if (JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE])
		JournalPrevMousePos = MousePosition;
	if (buttons[GLFW_MOUSE_BUTTON_MIDDLE] && !JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE])
	{
		JournalWindowScroll -= Journaldif.y;
		Journaldif = JournalPrevMousePos - MousePosition;
	}
	else
		Journaldif = { 0.0f ,0.0f};
	JournalWindowScroll += Journaldif.y;

	if (-jw->ViewportSize.y * 0.5f > Corner.y)
	{
		if (JournalWindowScroll > 0.0f)
			JournalWindowScroll = 0.0f;
		else if (JournalWindowScroll < (Corner.y))
			JournalWindowScroll = (Corner.y);
	}
	else
		JournalWindowScroll = 0.0f;
	
	jw->End();
	jw->Position = glm::vec2(jw->ViewportSize.x * -0.5f + sw->ViewportSize.x * 0.5f - 5.0f + JournalWindowPos, jw->ViewportSize.y * -0.5f + sw->ViewportSize.y * 0.5f - 5.0f);


	UseWindow(SceneWindowID);
	AmbientLight = 1.0f;

	fw->w_AmbientLight = 0.4f;
	fw->w_DirectionalLight = 1.0f;
	cw->w_AmbientLight = 1.0f;
	cw->w_DirectionalLight = 1.0f;

	bw->Draw(0);
	
	if (!disablefog)
	{
		fw->Scale = { 1.0f,1.0f };

		fw->Draw(1);
	}
	sw->Draw(2);

	if (!disablefog)
	{
		fw->Scale = { 1.6f,1.6f };

		fw->Draw(3);
	}
	cw->Draw(4);
	b = false;
	UI_button(&b, "Console", glm::vec2(-cw->ViewportSize.x * 0.5f + 15.0f, cw->ViewportSize.y * 0.5f - 20.0f), { 150,35 }, 0.35f, glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(0.5f), 10);
	if (b)
		ConsoleShow = !ConsoleShow;

	jw->Draw(4);
	b = false;
	UI_button(&b, "Journal", glm::vec2(cw->ViewportSize.x * 0.5f - 165.0f, cw->ViewportSize.y * 0.5f - 20.0f), { 150,35 }, 0.35f, glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(0.5f), 10);
	if (b)
		JournalWindowShow= !JournalWindowShow;

	pw->Draw(4);
	b = false;
	UI_button(&b, "Ship preview", pw->Position + glm::vec2(pw->ViewportSize.x * -0.5f + 15.0f, pw->ViewportSize.y * 0.5f + 20.0f), {150,35}, 0.35f, glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(0.5f), 10);
	if (b)
		PreviewShowWindow = !PreviewShowWindow;

	mw->Draw(5);


	if (JustPressedkey[GLFW_KEY_F9])
		QuickLoad("TheQuickSave");
	else if (JustPressedkey[GLFW_KEY_F5])
		QuickSave("TheQuickSave");
}

void PreReady()
{
	drawHeat = false;
	drawHealth = true;

	Speed = 1.0f;

	NewConType = 0;
	ConCreationStage = 0;
	
	NewConDebrie1 = false;
	NewConDebrie2 = false;

	newPartType = -1;//-1 = not selected
	newPartRotation = 0.0f;

	balltaken = false;
	saveFileName = "Save0.sav";
	absoluteControl = true;
	BuildingMode = false;

	bLogicMode = false;
	fLogicMode = false;
	vLogicMode = false;
	
	in_UI = 0;// <=0 - not, else - browsing ui // compleat bullshit

	createdFreezeDS = false; // why is that a thing?

	align = false;
	snapToGrid = false;

	NewConBall1 = -1;
	NewConBall2 = -1;
	
	StarsSpread = 2520.0f;
	StarsDepth = 10.0f;
	
	lastquestid = 0;
	MainMenu = false;
	switchScene = false;
	switchToMission = false;
	OpenMenu = false;
	Settings = false;
	lastsound = 0;
	lastsound2 = 0;
	lastplayedsound = 1;

	Exposure = 0.0f;


	BackgroundWindowID = -1;// background cosmetic window
	ForeWindowID = -1;// "game" window
	ForeGroundWindowID = -1;// foreground cosmetic window
	InterfaceWindowID = -1;// player interface window
	MenuWindowID = -1;
	foregroundMousePosition = { 0.0f,0.0f };
	lastEntityID = 0;
	TextSize = 1.0f;
	UISize = 18.0f;

	camerapos = { 0.0f,0.0f };// cameraposition that sets cameraposition in ProcessCamera() + screenshake
	GridCellSize = PARTSIZE * 2.0f;// gird that doesnt exist anymore
	reloadSources = false; // reload sound system
	brightness = 1.0f;// exposure modifier
	SRiter = 0; // screen res iter


	Materials = 0;
	storyint = 0;
	lastEntityName = "Save0.sav";
	EntityBackUpName = "PreMissionBackup";
	GameSaveName = "Save0";
	inside = false;
	inbase = true;

	RX = std::to_string(s_Resolution.x);
	RY = std::to_string(s_Resolution.y);

	lastid = 0;
	lastStaticBallid = 0;
	lastStaticCubeid = 0;
	foregroundFogParticleAmount = 1.0f;
	foregroundFogParticleT = 1.0f;
	for (int i = 0; i < 100'000; i++)
	{
		ballPosition[i] = { 0.0f,0.0f };
		ballVelocity[i] = { 0.0f,0.0f };
		ballForce[i] = { 0.0f,0.0f };
		ballSoundcd[i] = 0.0f;
		ballTemp[i] = 0.0f;
		ballVelocityBuff[i] = { 0.0f,0.0f };
		IsBall[i] = false;
	}
	// Remake with DataStorage
	std::cout<<"Pre ready\n";
	std::ifstream f("Settings.sav");
	if (f.is_open())
		while (!f.eof())
		{

			char junk;
			char line[256];
			f.getline(line, 256);
			std::strstream s;
			s << line;



			if (line[0] == 'R' && line[1] == 'e' && line[2] == 's')
				s >> junk >> junk >> junk >> s_Resolution.x >> s_Resolution.y;

			else if (line[0] == 'B' && line[1] == 'r' && line[2] == ' ')
				s >> junk >> junk >> brightness;

			else if (line[0] == 'B' && line[1] == 'l' && line[2] == ' ')
				s >> junk >> junk >> bloom;

			else if (line[0] == 'F' && line[1] == 'S' && line[2] == ' ')
				s >> junk >> junk >> s_Fullscreen;
			else if (line[0] == 'S' && line[1] == 'h' && line[2] == ' ')
				s >> junk >> junk >> s_ScreenShake;
			else if (line[0] == 'C' && line[1] == 'a' && line[2] == ' ')
				s >> junk >> junk >> s_ChromaticAbberation;

		}

	f.close();

	std::cout<<"settings ready\n";
	PartsData.Load("PartsProperties.ds");
	InitParts();
	PartsData.Save("PartsProperties.ds");
	
	std::cout<<"part props ready\n";


	
}

void SceneEnd()
{

	for(int i=0;i<Entities.size();i++)
		Entities[i]->Destroy();
	Entities.clear();

	for (int i = 0; i < Debris.Parts.size(); i++)
		Debris.Parts[i]->DeletePart();
	Debris.Parts.clear();

	DamageSpheres.clear();
	DamageSpheresArray.clear();
	bullets.clear();
	Rockets.clear();
	Lasers.clear();
	LightEffects.clear();
	clearParticleMaterials();
	CurrnetMission.TakenAreas.clear();
	CurrnetMission.TakenAreas.push_back({0,0,50,50});
	BuildingMode = false;
	vLogicMode = false;
	bLogicMode = false;
	fLogicMode = false;
	MainMenu = true;
}
void Rescale(int newWindth,int newHeight)
{

	Window* sw = GetWindow(ForeWindowID);
	Window* bw = GetWindow(BackgroundWindowID);
	Window* iw = GetWindow(InterfaceWindowID);
	Window* fw = GetWindow(ForeGroundWindowID);
	Window* mw = GetWindow(MenuWindowID);
	Window* pw = GetWindow(PreviewWindowID);
	Window* jw = GetWindow(JournalWindowID);

	sw->ViewportSize = { newWindth ,newHeight };
	sw->RecalculateSize();

	bw->ViewportSize = { newWindth ,newHeight };
	bw->RecalculateSize();

	mw->ViewportSize = { newWindth ,newHeight };
	mw->RecalculateSize();

	iw->ViewportSize = { newWindth ,newHeight };
	iw->RecalculateSize();

	fw->ViewportSize = { newWindth ,newHeight };
	fw->RecalculateSize();

	pw->ViewportSize = (glm::vec2(newWindth * 0.2f));
	pw->RecalculateSize();
	jw->ViewportSize = (glm::vec2(newWindth * 0.3f, newHeight * 0.6f));
	jw->RecalculateSize();

	float maxsize = fmaxf(sw->ViewportSize.x, sw->ViewportSize.y);
	TextSize = maxsize * 0.0005f * 0.35f;
	UISize = 25 * maxsize * 0.0005f;

}

void Destroy()
{	
	SavePlayerData();
	CurrnetMission.TakenAreas.clear();
	CurrnetMission.TakenAreas.push_back({0,0,50,50});
	SaveSettings();
}