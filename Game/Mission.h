#include <array>
enum MissionType 
{
	mining =1,
	retrival =2,
	pirates =3,
	infestation =4
};
class Mission
{
	public:
	bool exitedmission = false;
	int type = 0;
	int dificulty = 0;
	int size = 0;
	int materialReward = 0;
	std::vector<CentralPart*> AIShips;
	std::vector<unsigned int> AIShipsIds;
	std::vector<float> timers;
	std::map<std::string,bool> flags;
	bool story_mission = false;
	bool exiting = false;
	bool compleated = true;
	std::vector<glm::vec4> TakenAreas;
	std::vector<std::vector<CentralPart*>> Bots;
	std::vector<std::vector<Node*>> NodeHandles;

	glm::vec2 missionpos = { 0.0f,0.0f };

	//Shit for missions
	glm::vec2 planetpos = {0.0f,0.0f};


	
	void Start(glm::vec2 playerpos = {100.0f,-300.0f})
	{
		timers.clear();
		inbase = false;
		TakenAreas.clear();
		TakenAreas.push_back({0,0,50,50});
		AIShips.clear();
		flags.clear();
		Bots.clear();
		glm::vec2 vel = {0.0f,0.0f};
		if(Entities.size()>0)
		{
			vel = Entities[0]->avgvel;
			Entities[0]->SaveTo(EntityBackUpName);
		}
		//ClearMap
		ChangeMap("",false);
		SpawnPlayer(playerpos,EntityBackUpName);
		for(int i=0;i< Entities[0]->Parts.size();i++)
		{
			for(int a=0;a< Entities[0]->Parts[i]->bodysize;a++)
			{
				ballVelocityBuff[Entities[0]->Parts[i]->body[a]]= vel;
				ballVelocity[Entities[0]->Parts[i]->body[a]] = vel;
			}
		}
		Materials -= GetShipCost(Entities[0]);
		if(!story_mission)
		{	for(int i=0;i<size * 5;i++)
			{
				glm::vec2 randpos = glm::vec2(float((rand() % 1000) - 500),float((rand() % 1000) - 500));
				glm::vec2 randsize = glm::vec2(float((rand() % 50) + 50),float((rand() % 50) + 50));
				bool clear = false;
				while(!clear)
				{
					randpos = glm::vec2(float((rand() % 1000) - 500),float((rand() % 1000) - 500));
					randsize = glm::vec2(float((rand() % 50) + 50),float((rand() % 50) + 50));
					clear = true;
					for(auto v : TakenAreas)
					{
						if(randpos.x > v.x-v.z && randpos.x < v.x+v.z && randpos.y > v.y-v.w && randpos.y < v.y+v.w)
							clear = false;
					}
				}
				TakenAreas.push_back(glm::vec4(randpos,randsize));

				int ballscount = rand() %5 + 5;
				for(int a=0;a<ballscount;a++)
				{
					CO_Ball* newball = new CO_Ball();
					newball->position = randpos + glm::vec2(rand() % (int)randsize.x * 0.5f,rand() % (int)randsize.y * 0.5f);
					newball->b.position = newball->position;
					newball->Mater = &MO_Ball;
					newball->b.r = rand() % (int)randsize.x * 0.5f;
					newball->Scale = glm::vec2(newball->b.r, newball->b.r);
					newball->ForceMaterialCheck = false;
					GameScene->Nodes.push_back(newball);
				}
			}

		
			switch (type)
			{
			case MissionType::mining:
			{

			}break;
			case MissionType::retrival:
			{

			}break;
			case MissionType::pirates:
			{
				for(int i=0;i<size;i++)
				{
					glm::vec2 randpos = glm::vec2(float((rand() % 1000) - 500),float((rand() % 1000) - 500));
					bool clear = false;
					while(!clear)
					{
						randpos = glm::vec2(float((rand() % 1000) - 500),float((rand() % 1000) - 500));
						clear = true;
						for(auto v : TakenAreas)
						{
							if(randpos.x > v.x-v.z && randpos.x < v.x+v.z && randpos.y > v.y-v.w && randpos.y < v.y+v.w)
								clear = false;
						}
					}
					int randshipid = rand()% shipNames.size(); 
					AIShips.push_back(SpawnAiShip(randpos,"Save0"));
					AIShipsIds.push_back(AIShips[AIShips.size()-1]->id);
					TakenAreas.push_back(glm::vec4(randpos,AIShips[AIShips.size()-1]->maxR,AIShips[AIShips.size()-1]->maxR));
				}


			}break;
			case MissionType::infestation:
			{

			}
			default:
				break;
			}
		}
		else switch (storyint)
			{
			case 0: // tutorial
			{
				Background.LoadFrom("Scenes/Sun.sav");
				ChangeMap("Scenes/tutorial.sav",false);
				SpawnPlayer(playerpos);
				for(int i=0;i< Entities[0]->Parts.size();i++)
				{
					for (int a = 0; a < Entities[0]->Parts[i]->bodysize; a++)
					{
						ballVelocityBuff[Entities[0]->Parts[i]->body[a]] = vel;
						ballVelocity[Entities[0]->Parts[i]->body[a]] = vel;
					}
				}
				timers.push_back(2.0f);
				timers.push_back(0.0f);
				timers.push_back(0.0f);
				timers.push_back(0.5f);//door timers [3],[4]
				timers.push_back(0.0f);
				flags["Explode"] = false;
				flags["Spawned"] = false;
				for(int i=0;i<8;i++)
					Debris.SpawnPart(PART::ROCKETENGINE,{i + 11.5f,5.0f},PARTSIZE);

				glm::vec2 pos = {30.0f,100.0f};
				Debris.SpawnPart(PART::RADIATOR,pos,PARTSIZE);
				pos = { 30.0f,200.0f};
				Debris.SpawnPart(PART::GUN,pos,PARTSIZE);



			}break;
			case 1:
			{
				ChangeMap("Scenes/Gun.sav",false);
				SpawnPlayer(playerpos);
				for(int i=0;i< Entities[0]->Parts.size();i++)
				{
					for (int a = 0; a < Entities[0]->Parts[i]->bodysize; a++)
					{
						ballVelocityBuff[Entities[0]->Parts[i]->body[a]] = vel;
						ballVelocity[Entities[0]->Parts[i]->body[a]] = vel;
					}
				}
				timers.push_back(5.0f);
				timers.push_back(5.0f);
				timers.push_back(5.0f);
				flags["Ending"] = false;
				flags["Spawned"] = false;
				std::vector<CentralPart*> arrr;
				Bots.push_back(arrr);
				Bots[0].push_back(SpawnAiShip({-150.0f,400.0f},"drone"));
				Bots[0].push_back(SpawnAiShip({-150.0f,380.0f},"drone"));
				Bots[0].push_back(SpawnAiShip({-170.0f,400.0f},"drone"));

				for(auto e : Bots[0])
				{
					e->AIState = 0;
				}

			}break;
			case 2:
			{
				ChangeMap("Scenes/pstation.sav",false);
				SpawnPlayer(playerpos);
				for(int i=0;i< Entities[0]->Parts.size();i++)
				{
					for (int a = 0; a < Entities[0]->Parts[i]->bodysize; a++)
					{
						ballVelocityBuff[Entities[0]->Parts[i]->body[a]] = vel;
						ballVelocity[Entities[0]->Parts[i]->body[a]] = vel;
					}
				}
				timers.push_back(5.0f);
				timers.push_back(5.0f);
				timers.push_back(5.0f);
				flags["Ending"] = false;
				flags["Spawned"] = false;
				std::vector<CentralPart*> arrr;
				Bots.push_back(arrr);
				
				std::vector<Node*> Cooolernv;
				NodeHandles.push_back(Cooolernv);
				std::vector<Node*> nv;
				NodeHandles.push_back(nv);
				for(int i=0;i<GameScene->Nodes.size();i++)
				{
					if(GameScene->Nodes[i]->type == NodeType::NODE)
					{
						while (Bots.size()<=GameScene->Nodes[i]->id)
						{
							std::vector<CentralPart*> v;
							Bots.push_back(v);
						}
						Bots[GameScene->Nodes[i]->id].push_back(SpawnAiShip(GameScene->Nodes[i]->position,GameScene->Nodes[i]->Name));

					}
					if(GameScene->Nodes[i]->Name == "Core")
					{
						NodeHandles[1].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "Cooler")
					{
						NodeHandles[0].push_back(GameScene->Nodes[i]);
					}
				}

			}break;
			case 3:
			{
				AmbientLight=  0.0f;
				ChangeMap("Scenes/mining.sav",false);
				SpawnPlayer(playerpos);
				for(int i=0;i< Entities[0]->Parts.size();i++)
				{
					for (int a = 0; a < Entities[0]->Parts[i]->bodysize; a++)
					{
						ballVelocityBuff[Entities[0]->Parts[i]->body[a]] = vel;
						ballVelocity[Entities[0]->Parts[i]->body[a]] = vel;
					}
				}
				timers.push_back(5.0f);
				timers.push_back(5.0f);
				timers.push_back(5.0f);
				flags["Ending"] = false;
				flags["Spawned"] = false;

				flags["room1"] = false;
				flags["room2"] = false;

				flags["Laser1"] = true;
				flags["Laser2"] = true;
				flags["Laser3"] = true;
				
				
				AqueredCameraScale = {2.0f,2.0f};
				std::vector<CentralPart*> arrr;
				Bots.push_back(arrr);
				
				std::vector<Node*> fuses;
				NodeHandles.push_back(fuses);//0
				std::vector<Node*> Turbine;
				NodeHandles.push_back(Turbine);//1
				std::vector<Node*> Lasers;
				NodeHandles.push_back(Lasers);//2
				std::vector<Node*> LaserVisuals;
				NodeHandles.push_back(LaserVisuals);//3
				std::vector<Node*> LaserParticles;
				NodeHandles.push_back(LaserParticles);//4
				std::vector<Node*> LaserParticleReps;
				NodeHandles.push_back(LaserParticleReps);//5
				std::vector<Node*> LaserLightSpheres;
				NodeHandles.push_back(LaserLightSpheres);//6
				std::vector<Node*> LightSpheres;
				NodeHandles.push_back(LightSpheres);//7
				for(int i=0;i<GameScene->Nodes.size();i++)
				{
					if(GameScene->Nodes[i]->type == NodeType::NODE)
					{
						while (Bots.size()<=GameScene->Nodes[i]->id)
						{
							std::vector<CentralPart*> v;
							Bots.push_back(v);
						}
						Bots[GameScene->Nodes[i]->id].push_back(SpawnAiShip(GameScene->Nodes[i]->position,GameScene->Nodes[i]->Name));

					}
					if(GameScene->Nodes[i]->Name == "L1Fuse"||
					   GameScene->Nodes[i]->Name == "L2Fuse"||
					   GameScene->Nodes[i]->Name == "L3Fuse")
					{
						NodeHandles[0].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "Turbine")
					{
						NodeHandles[1].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "laser1"||
					   GameScene->Nodes[i]->Name == "laser2"||
					   GameScene->Nodes[i]->Name == "laser3")
					{
						NodeHandles[2].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "laservisual1"||
					   GameScene->Nodes[i]->Name == "laservisual2"||
					   GameScene->Nodes[i]->Name == "laservisual3")
					{
						NodeHandles[3].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "laserSparkEmiter1"||
					   GameScene->Nodes[i]->Name == "laserSparkEmiter2"||
					   GameScene->Nodes[i]->Name == "laserSparkEmiter3")
					{
						NodeHandles[4].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "laserSparkRepul1"||
					   GameScene->Nodes[i]->Name == "laserSparkRepul2"||
					   GameScene->Nodes[i]->Name == "laserSparkRepul3")
					{
						NodeHandles[5].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "laserls1"||
					   GameScene->Nodes[i]->Name == "laserls2"||
					   GameScene->Nodes[i]->Name == "laserls3")
					{
						NodeHandles[6].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->type == NodeType::LIGHTSOURCEOBJECT &&
						GameScene->Nodes[i]->id>1)
					{
						NodeHandles[7].push_back(GameScene->Nodes[i]);
					}
				}

			}break;
			case 4:
			{
				ChangeMap("Scenes/lab.sav",false);
				SpawnPlayer(playerpos);
				for(int i=0;i< Entities[0]->Parts.size();i++)
				{
					for (int a = 0; a < Entities[0]->Parts[i]->bodysize; a++)
					{
						ballVelocityBuff[Entities[0]->Parts[i]->body[a]] = vel;
						ballVelocity[Entities[0]->Parts[i]->body[a]] = vel;
					}
				}
				timers.push_back(0.0f);
				flags["LaserShot"] = false;
				
				
				std::vector<CentralPart*> arrr;
				Bots.push_back(arrr);
				
				std::vector<Node*> Turbine;
				NodeHandles.push_back(Turbine);//0
				std::vector<Node*> LaserParts;
				NodeHandles.push_back(LaserParts);//1
				std::vector<Node*> Misc;
				NodeHandles.push_back(Misc);//2
				for(int i=0;i<GameScene->Nodes.size();i++)
				{
					if(GameScene->Nodes[i]->type == NodeType::NODE)
					{
						while (Bots.size()<=GameScene->Nodes[i]->id)
						{
							std::vector<CentralPart*> v;
							Bots.push_back(v);
						}
						Bots[GameScene->Nodes[i]->id].push_back(SpawnAiShip(GameScene->Nodes[i]->position,GameScene->Nodes[i]->Name));

					}
					if(GameScene->Nodes[i]->Name == "turbine")
					{
						NodeHandles[0].push_back(GameScene->Nodes[i]);
					}
					if(GameScene->Nodes[i]->Name == "Laser1"||
					   GameScene->Nodes[i]->Name == "LaserPE"||
					   GameScene->Nodes[i]->Name == "LaserLS"||
					   GameScene->Nodes[i]->Name == "LaserDS")
					{
						NodeHandles[1].push_back(GameScene->Nodes[i]);
					}
					if(	GameScene->Nodes[i]->Name == "turbineMonitor"||
						GameScene->Nodes[i]->Name == "LabMonitor")
					{
						NodeHandles[2].push_back(GameScene->Nodes[i]);
					}
				}

			}break;
			default:
				break;
			}
	}
	void CheckShips(float dt)
	{
		AIShips.clear();
		int AIShipsi =0; 
		if(Entities.size()>0) 
		while(AIShipsi<AIShipsIds.size())
		{
			int i=0;
			for(int i = 1;i <Entities.size();i++)// first is player
				if(Entities[i]->id == AIShipsIds[AIShipsi])
					AIShips.push_back(Entities[i]);
			AIShipsi++;
		}
	}
	void Process(float dt)
	{
		if(exiting)
			exitMission(true);
		
		//for(int i=0;i<Bots.size();i++)
		//{
		//	for(int a=0;a<Bots[i].size();a++)
		//	{
		//		if(Bots[i][a]->Delete)
		//		{
		//			Bots[i][a] = Bots[i][Bots[i].size()]-1;
		//			Bots[i].pop_back();
		//		}
		//	}
		//}
		inside = false;
		for(int i =0;i<TriggerCubes.size();i++)
		{
			if(TriggerCubes[i]->Delete)
			{
				TriggerCubes[i] = TriggerCubes[TriggerCubes.size()]-1;
				TriggerCubes.pop_back();
			}
			if(TriggerCubes[i]->triggered && Bots.size()>TriggerCubes[i]->id)	
			{
				for(int a=0;a<Bots[TriggerCubes[i]->id].size();a++)
				{	
					if(!Bots[TriggerCubes[i]->id][a]->Delete)
					Bots[TriggerCubes[i]->id][a]->AIState = 1;
				}
			}
			if(TriggerCubes[i]->determineInside && TriggerCubes[i]->triggered)
				inside = true;			
		}
		if(inside)
			AmbientLight -= dt;
		else
			AmbientLight += dt;
		
		if(AmbientLight<0.0f)
			AmbientLight = 0.0f;
		if(AmbientLight>0.4f)
			AmbientLight = 0.4f;

		for(int i=0;i<timers.size();i++)
		{
			timers[i]-=dt;
			if(timers[i]<0.0f)
				timers[i]=0.0f;
			
		}

		for(int i=0;i<NodeHandles.size();i++)
		{
			int a =0;
			while(a < NodeHandles[i].size())
			{
				if(NodeHandles[i][a] == nullptr || NodeHandles[i][a]->Delete)
				{
					NodeHandles[i][a] = NodeHandles[i][NodeHandles[i].size()-1];
					NodeHandles[i].pop_back();
				}
				else
					a++;
			}
		}
		if(!story_mission)
			switch (type)
			{
			case MissionType::mining:
			{

			}break;
			case MissionType::retrival:
			{

			}break;
			case MissionType::pirates:
			{
				if(AIShips.size()==0)
					exitMission(true);

			}break;
			case MissionType::infestation:
			{

			}
			default:
				break;
			}
		else
			switch (storyint)
			{
			case 0: // tutorial
			{

				for(auto n : Background.Nodes)
				{
					if(n->Name == "Planet")
					{
						planetpos = n->position;
						if(flags["Explode"])
						{
							glm::vec4 col = glm::vec4(100.0f,2.0f,1.0f,1.0f) * (7.5f - timers[1]) * -2.0f + glm::vec4(0.1f,0.2f,1.0f,1.0f);
							if(col.r<0.1f)
								col.r = 0.1f;
							if(col.g<0.2f)
								col.g = 0.2f;
							if(col.b<1.0f)
								col.b = 1.0f;
							col.a = 1.0f;
							((Object*)n)->Color = col;
							if(timers[1] < 7.5f)
								n->Delete = true;
						}
					}
					if(flags["Explode"])
					{
						if(n->Name == "Planet2" && timers[1] < 7.5f)
							n->Delete = true;
						else if(n->Name == "PlanetP" && timers[1] < 7.5f)
							n->Delete = true;
					}
				}
				ParticleObject* PlanetExplodionPO = nullptr; 
				for(auto n : Background.Nodes)
				{
					if(n->Name == "ExplodionP1" || n->Name == "ExplodionP2" || n->Name == "ExplodionP3")
					{
						n->position = planetpos;
						ParticleObject* po = (ParticleObject*)n; 
						PlanetExplodionPO = po;
						if(!flags["Explode"]  || timers[1] < 7.5f)
						{
							po->amount = 0;
							po->t = 0.0f;
							po->tick = 0.01f;
						}
						else  po->amount = 300;

						
					}
					else if(n->Name == "PLS")
					{
						LightSource* ls = (LightSource*)n;
						if(!flags["Explode"]) 
							ls->color = {10.0f,0.0f,110.0f,10.0f};
						else
							ls->color = glm::vec4(40.0f,5.0f,1.0f,1.5f) * timers[1]*0.125f;
					}
					else if(n->Name == "L1")
					{						
						Object* o = (Object*)n; 

						if(flags["Explode"])
							o->Scale.x = 105.0f * (timers[1] * 0.125f);
						else
							o->Scale.x = 0.0f;
												
						if(o->invertX)
							o->position = planetpos + glm::vec2(-o->Scale.x,900.0f);
						else
							o->position = planetpos + glm::vec2(o->Scale.x,900.0f);
						
					}
					else if(n->Name == "L2")
					{
						Object* o = (Object*)n; 

						if(flags["Explode"])
							o->Scale.x = 95.0f * (timers[1] * 0.125f);
						else
							o->Scale.x = 0.0f;

						if(o->invertX)
							o->position = planetpos + glm::vec2(-o->Scale.x,-700.0f);
						else
							o->position = planetpos + glm::vec2(o->Scale.x,-700.0f);
					}

				}
				// text at start {0.0f,0.0f}
				glm::vec2 pos = {-10.0f,0.0f};
				float stepy = 2.0f;
				float Tscale = 0.04f; 
				if(!BuildingMode)
					DrawText("Press B to enter building mode",pos-=glm::vec2(0.0f,stepy),Tscale);
				else{		
					DrawText("RMB to grab parts",pos-=glm::vec2(0.0f,stepy),Tscale);
					DrawText("LMB to make a stasis field",pos-=glm::vec2(0.0f,stepy),Tscale);
					DrawText("hold alt and click on blue circles to atach parts to your ship",pos-=glm::vec2(0.0f,stepy),Tscale);
				}
				pos = {-10.0f,50.0f};
				DrawText("W S - forward/backwards,",pos-=glm::vec2(0.0f,stepy),Tscale);
				DrawText("Q E - left/right,",pos-=glm::vec2(0.0f,stepy),Tscale);
				DrawText("A D - rotate left/right,",pos-=glm::vec2(0.0f,stepy),Tscale);
				
				pos = {-10.0f,100.0f};
				if(!BuildingMode)
					DrawText("Press B to enter building mode",pos-=glm::vec2(0.0f,stepy),Tscale);
				else{		
					DrawText("Attach radiator to the parts that need cooling",pos-=glm::vec2(0.0f,stepy),Tscale);
					DrawText("Use HeatPipes for better heat transfer (Tab to switch connection type)",pos-=glm::vec2(0.0f,stepy),Tscale);
				}
				pos = {-10.0f,200.0f};
				if(!BuildingMode)
					DrawText("Press B to enter building mode",pos-=glm::vec2(0.0f,stepy),Tscale);
				else{		
					DrawText("Atach weapon",pos-=glm::vec2(0.0f,stepy),Tscale);
					DrawText("In bottom left corner select vector logic",pos-=glm::vec2(0.0f,stepy),Tscale);
					DrawText("Connect weapon(target) to mouse position in central part",pos-=glm::vec2(0.0f,stepy),Tscale);
					DrawText("Select Bools logic, attach gun (Shoot) to any control in central part",pos-=glm::vec2(0.0f,stepy),Tscale);
				}
				if(!flags["Spawned"] || Entities.size()>1)
					timers[3] = 0.5f;
				bool hasgun = false;
				if(Entities.size()>0)
					for(auto p : Entities[0]->Parts)
						if(p->parttype == TYPE::WEAPON)
						{
							hasgun = true;
							break;
						}
				
				if(!flags["Spawned"] && hasgun)
					timers[4] = 0.5f;
				
				for(int i=0;i<GameScene->Nodes.size();i++)
				{
					if(GameScene->Nodes[i]->type == NodeType::LIGHTSOURCEOBJECT)
					{
						LightSourceObject* ls = (LightSourceObject*) GameScene->Nodes[i];
						cube c;
						c.width = ls->Scale.x;
						c.height = ls->Scale.y;
						c.position = ls->position;
						if(Entities.size()>0 && PointToQuadCollisionCheck(c,Entities[0]->mid))
							ls->Color = {1.0f,1.0f,1.0f,1.0f}; 
						else
							ls->Color = {0.0f,0.0f,0.0f,0.0f}; 
							
						if(flags["Explode"])
						{
							ls->Color = glm::vec4(abs(sin(timers[1] * 100.0f))); 
						}
					}

					if(GameScene->Nodes[i]->type == NodeType::CO_CUBE && GameScene->Nodes[i]->Z_Index == 0)
					{
						GameScene->Nodes[i]->position.x =  roundf(GameScene->Nodes[i]->position.x/30.0f) * 30.0f;
						GameScene->Nodes[i]->position.y =  roundf(GameScene->Nodes[i]->position.y/30.0f) * 30.0f;
						((CO_Cube*) GameScene->Nodes[i])->Scale =  {15.0f,15.0f};
						((CO_Cube*) GameScene->Nodes[i])->c.width = 15.0f;
						((CO_Cube*) GameScene->Nodes[i])->c.height = 15.0f;
					}
					else
					{
						CO_Cube* cub= (CO_Cube*) GameScene->Nodes[i];
						//-30 - 60
						// 15 + (-45 * stage * -invertx) 
						//450
						//660
						if(cub->Z_Index == 2)
						{
							if(!cub->invertX)
								cub->position.x = (-30 * timers[4] * 2.0f);
							if(cub->invertX)
								cub->position.x = 30 + (30 * timers[4] * 2.0f);
						}
						if(cub->Z_Index == 3)
						{
							if(!cub->invertX)
								cub->position.x = (30 * timers[3] * 2.0f) - 30;
							if(cub->invertX)
								cub->position.x = 60 + (-30 * timers[3] * 2.0f);

						}						
					}

					if(GameScene->Nodes[i]->id == 7)// race track
					{
						// text 2
						glm::vec2 pos = GameScene->Nodes[i]->position;
						DrawText("Press 'V' to switch controll scheme (relative to mouse + rotate towards mouse / absolute)",pos-=glm::vec2(0.0f,stepy),Tscale);
					}
					if(GameScene->Nodes[i]->id == 6)// enemy
					{
						// text 3
						glm::vec2 pos = GameScene->Nodes[i]->position;
					
						DrawText("Destroy drone",pos-=glm::vec2(0.0f,stepy),Tscale);	
						DrawText("Destroing central part will kill drone instantly",pos-=glm::vec2(0.0f,stepy),Tscale);	
						if(Entities.size()>0)
							{
								if(Entities[0]->mid.x<-100.0f && Entities.size() == 1 && flags["Spawned"] && !flags["Explode"])
								{
									flags["Explode"] = true;
									timers[0] = 2.0f;
									timers[1] = 8.0f;
									timers[2] = 15.0f;
									AqueredCameraScale = {6.0f,6.0f};
									Entities[0]->player = true;
									Entities[0]->autocontrol = true;
									Entities[0]->deactivated = true;
								}
								if(!flags["Spawned"] && sqrlength(Entities[0]->mid - pos) < 10000)
								{
									flags["Spawned"] = true;
									SpawnAiShip(pos,"bot");
								}
							}
						}
				}
				

				if(flags["Explode"])
				{	
					DrawLight(glm::vec3(CameraPosition - glm::vec2(100.0f,50.0f),-0.05f), {1000.0f,1000.0f,5.0f},glm::vec4(10.0f * timers[1]*0.5f,2.0f * timers[1]*0.5f,1.0f * timers[1]*0.5f,2.0f  * timers[1]*0.5f));
					
					ConsoleTexts.clear();	
					if(timers[2]<10.0f)
					{
					GetWindow(BackgroundWindowID)->w_DirectionalLight = timers[2]/10.0f;
					ConsoleTexts.push_back("Major threat detected");
					ConsoleTexts.push_back("No operators available");
					ConsoleTexts.push_back("attempt at connection to ground facilities...");
					if(timers[2]<9.0f)
					{
					ConsoleTexts.push_back("failed");
					ConsoleTexts.push_back("attempt at connection to ground facilities...");
					if(timers[2]<8.0f)
					{
					ConsoleTexts.push_back("failed");
					ConsoleTexts.push_back("attempt at connection to ground facilities...");
					if(timers[2]<7.0f)
					{
					ConsoleTexts.push_back("failed");
					ConsoleTexts.push_back("No ground facility found");
					ConsoleTexts.push_back("objective priority critical, autonomus controll available. Switch to fully autonomous controlls: y/n?");
					if(timers[2]<6.5f)
					{
					ConsoleTexts.push_back("transfering to fully autonomus controll...");
					if(timers[2]<5.0f)
					{
					ConsoleTexts.push_back("analyzing data from last operator training session...");
					if(timers[2]<3.0f)
					{
					ConsoleTexts.push_back("syntezising of new control unit...");
					if(timers[2]<1.0f)
					{
					ConsoleTexts.push_back("connecting AI controller to the unit...");
					}}}}}}}}

					
					if(timers[0]<=0.0f)
					{
						float stage = (timers[1])*0.1666f;
						float antstage = (6.0f - timers[1])*0.1666f;
						float val = 6.0f * stage;
						if(val<0.7f)
							val = 0.7f;
						AqueredCameraScale = {val,val};
						camerapos -= glm::vec2(300.0f,300.0f) * dt * antstage;
						if(timers[1]>0.1f)
						{
							ScreenShake = 0.01f + antstage * 0.02f;
							ChromaticAbberation = 10.00f + antstage * 10.00f;
						}
						if(Entities.size()>0  && !Entities[0]->dead && !Entities[0]->destroyed)
						{
							Entities[0]->SaveTo("PreMissionBackup");
							Entities[0]->SaveTo(EntityBackUpName);
							Entities[0]->Destroy();
						}

					}
					if (timers[2]<0.1f)
					{
						ScreenShake = 0.0f;
						storyint =1;
						Materials = 500;

						GetWindow(BackgroundWindowID)->w_DirectionalLight = 1.0f;
						exitMission(false);
					}
					
				}
			}break;
			case 1:
			{
				if(flags["Ending"])
				{	
					ConsoleTexts.clear();
					
					if(timers[2]<14.0f)
						ConsoleTexts.push_back("Analizyng blackboxes...");
					if(timers[2]<12.3f)
					{
						ConsoleTexts.push_back("    \"...So, they asked you to just stay around a station that just deleted a planet,");
						ConsoleTexts.push_back("when its cloacking failed right in the middle of the shot, offered you an insane pay check");
						ConsoleTexts.push_back("and you aggreed? Its a suicide mission. \"");
						ConsoleTexts.push_back("    \" Listen, no one with a clear mind will get even close to a thing that kiled a planet");
						ConsoleTexts.push_back("and we only need to be here for a few more hours. And for a day i've been here i only");
						ConsoleTexts.push_back("saw a few madmans on a small ships.\"");
						ConsoleTexts.push_back("    \" Oh, look, here is another one\"");
					}
					if(timers[2]<8.0f)
						ConsoleTexts.push_back("Saving the \"Employer\" location as a \"Target\"...");
					if(timers[2]<7.3f)
						ConsoleTexts.push_back("Continuing analizyng blackboxes...");
					if(timers[2]<5.0f)
						ConsoleTexts.push_back("Last ship got here only becouse of high heat radiation after hull collaps");
					if(timers[2]<4.5f)
						ConsoleTexts.push_back("Saving note: High heat radiatoin will attract more pirates");
					if(timers[2]<1.5f)
						ConsoleTexts.push_back("Initiaing extraction");
						

					if(timers[2]<=0.0f)
					{

						storyint =2;
						exitMission(true);
					}
					return;
				}
				glm::vec2 Breachpos = {0.0f,0.0f};
				glm::vec2 Corepos = {135.0f,150.0f};
				for(int i=0;i<GameScene->Nodes.size();i++)
				{
					if(GameScene->Nodes[i]->Name == "wCO_Cube")
					{
						CO_Cube* cc = (CO_Cube*)GameScene->Nodes[i];
						if(flags["Explode"])
						{
							glm::vec2 dif = Corepos - cc->position;
							if(sqrlength(dif)>100.0f)
							{
								cc->position += Normalize(dif) * (10.0f - timers[0])*0.2f;
							}
						}
					}
					if(GameScene->Nodes[i]->Name == "CO_Cube")
					{
						CO_Cube* cc = (CO_Cube*)GameScene->Nodes[i];
						if(flags["Explode"] && timers[0]<6.0f)
						{
							glm::vec2 dif = Corepos - cc->position;
							if(sqrlength(dif)>100.0f)
							{
								cc->position += Normalize(dif) * (8.0f - timers[0])*0.2f;
							}
						}
					}
					if(GameScene->Nodes[i]->Name == "CO_Ball")
					{
						CO_Ball* cc = (CO_Ball*)GameScene->Nodes[i];
						if(flags["Explode"] && timers[0]<6.0f)
						{
							cc->b.r = 40 + 40.0f * (6.0f - timers[0])/6.0f;
							cc->Scale = {40 + 40.0f * (6.0f - timers[0])/6.0f,40 + 40.0f * (6.0f - timers[0])/6.0f};
						}
					}
					if(GameScene->Nodes[i]->Name == "Object")
					{
						if(flags["Explode"])
						{
							GameScene->Nodes[i]->Delete = true;
						}
					}
					if(GameScene->Nodes[i]->Name == "rodlight")
					{
						LightSourceObject* ls = (LightSourceObject*)GameScene->Nodes[i];
						if(flags["Explode"]&& timers[0]<6.5f)
						{
							ls->Color.a=0.0f;
						}
					}
					if(GameScene->Nodes[i]->Name == "fe1")
					{
						ParticleObject* cc = (ParticleObject*)GameScene->Nodes[i];
						if(flags["Explode"] && timers[0]<6.0f)
						{
							cc->r = 40 + 40.0f * (6.0f - timers[0])/6.0f;
						}
					}
					if(GameScene->Nodes[i]->Name == "fa1")
					{
						ParticleObject* cc = (ParticleObject*)GameScene->Nodes[i];
						if(flags["Explode"] && timers[0]<6.0f)
						{
							cc->r = 40 + 40.0f * (6.0f - timers[0])/6.0f;
						}
					}
					if(GameScene->Nodes[i]->Name == "CrackLight")
					{
						Breachpos = GameScene->Nodes[i]->position;
						LightSourceObject* ls = (LightSourceObject*)GameScene->Nodes[i];
						if(flags["Explode"])
						{
							ls->Color.a = (timers[0] - 9.0f);
							if(ls->Color.a<0.0f)
								ls->Color.a = 0.0f;
						}
					}
					if(GameScene->Nodes[i]->Name == "CoreLight")
					{
						LightSourceObject* ls = (LightSourceObject*)GameScene->Nodes[i];
						if(flags["Explode"])
						{
							ls->Color.a = (1.0f - (timers[0] - 9.0f));
							if(ls->Color.a>1.0f)
								ls->Color.a = 1.0f;
						}
						if(flags["Explode"] && timers[0]<6.0f)
						{
							ls->Scale = {270.0f + 270.0f * (6.0f - timers[0])/2.0f,270.0f + 270.0f * (6.0f - timers[0])/2.0f};
						}
					}

				}



				if(timers[0] <7.0f && flags["Explode"]&& ConsoleTexts.size()==1)
				{
					ConsoleTexts.push_back("Incoming transmition, source unknown");
				
				}
				if(timers[0] <6.5f && flags["Explode"]&& ConsoleTexts.size()==2)
				{
					ConsoleTexts.push_back("Warning, Hull integrity critical, Station collaps imminent");

					ScreenShake = 1.0f;
				}
				if(timers[0] <2.0f && flags["Explode"]&& ConsoleTexts.size()>0)
				{
					ConsoleTexts.clear();
					for(int i=0;i<GameScene->Nodes.size();i++)
					{
						if(GameScene->Nodes[i]->Name == "CO_Ball")
						{
							CO_Ball* cc = (CO_Ball*)GameScene->Nodes[i];
							if(flags["Explode"] && timers[0]<6.0f)
							{
								cc->Color = {10.0f,2.0f,2.0f,1.0f};
							}
						}
						if(GameScene->Nodes[i]->Name == "fe1")
						{
							ParticleObject* cc = (ParticleObject*)GameScene->Nodes[i];
							if(flags["Explode"] && timers[0]<6.0f)
							{
								cc->amount = 0;
							}
						}
					}
				}
				if(timers[0] <=0.0f && flags["Explode"] && !flags["Spawned"])
				{
					CentralPart* s =  SpawnAiShip({-300.0f,700.0f},"Bigboy");	
					s->AIState = 1;			
					flags["Spawned"] = true;
				}
				if(!flags["Ending"] && flags["Spawned"] && Entities.size()==1)
				{
					flags["Ending"] = true;
					timers[2] = 15.0f;
				}
			

				if(Entities.size()>0)
				{
					if(sqrlength(Entities[0]->mid - Breachpos) < 400 * 400)
						for(auto e : Bots[0])
						{
							e->AIState = 1;
						}

					if(sqrlength(Entities[0]->mid - Breachpos) < 100 * 100 && !flags["Explode"] && Entities.size() == 1)
					{
						flags["Explode"] = true;
						timers[0] = 10.0f;
						ConsoleTexts.push_back("Hull breach succesfull");
						ScreenShake = 0.2f;
						for(int i=0;i<GameScene->Nodes.size();i++)
						{
							if(GameScene->Nodes[i]->Name == "wCO_Cube")
							{
								CO_Cube* cc = (CO_Cube*)GameScene->Nodes[i];

								for(auto a : AvailableParticleAssets)
									if(a->Name == "sparks")a->pe.SpawnInCircle(cc->position,20.0f,10,{-10.0f,10.0f});
							}
						}
					}
				}
			}break;
			case 2:
			{
				DestructableStaticBall* Core = nullptr;
				if(NodeHandles.size()>=2 && NodeHandles[1].size()>0)
					Core = (DestructableStaticBall*)NodeHandles[1][0];		
				if(Core!=nullptr)
				{
					Core->passiveCooling = 10.0f;
					if(NodeHandles.size()>0 && NodeHandles[0].size()>0)
					{
						for(int a=0;a<NodeHandles[0].size();a++)
						{
							Core->passiveCooling += 10.0f;
						}
					}
				}
				for(int i=0;i<Bots.size();i++)
				{
					// central
					if(i==1 && Entities[0]->mid.y>-200.0f)
					{
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
					// left
					if(i==2 && Entities[0]->mid.x<-885.0f && Entities[0]->mid.x>-1365.0f && Entities[0]->mid.y>60.0f && Entities[0]->mid.y<570.0f)
					{
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
					// right
					if(i==3 && Entities[0]->mid.x>825.0f && Entities[0]->mid.x<1305.0f && Entities[0]->mid.y>60.0f && Entities[0]->mid.y<570.0f)
					{
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
				}

				if(!flags["Exploded"] && (Core == nullptr || Core->Delete))
				{
					timers[0] = 5.0f;
					flags["Exploded"] = true;
				}
				if(flags["Exploded"] && timers[0] <=0.0f)
				{
					storyint+=1;
					exitMission(true);
				}
			}break;
			case 3:
			{
				for(int i=0;i<Bots.size();i++)
				{
					if(i==2 && Entities[0]->mid.y<-390.0f)
					{
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
					if(i==1 && Entities[0]->mid.x<1005.0f && Entities[0]->mid.x>525.0f && Entities[0]->mid.y>-1650.0f && Entities[0]->mid.y<-870.0f)
					{
						flags["room1"] = true;
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
					if(i==3 && Entities[0]->mid.x>-1065.0f && Entities[0]->mid.x<-315.0f && Entities[0]->mid.y>-1740.0f && Entities[0]->mid.y<-1320.0f)
					{
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
					if(i==4 && Entities[0]->mid.x>780.0f && Entities[0]->mid.x<1260.0f && Entities[0]->mid.y>-2790.0f && Entities[0]->mid.y<-1860.0f)
					{
						flags["room2"] = true;
						for(int a =0; a<Bots[i].size();a++)
							Bots[i][a]->AIState = 1;
					}
				}

				if(NodeHandles.size()<8)
					return;
				std::vector<Node*> fuses = NodeHandles[0];
				std::vector<Node*> Turbine = NodeHandles[1];
				std::vector<Node*> Lasers = NodeHandles[2];
				std::vector<Node*> LaserVisuals = NodeHandles[3];
				std::vector<Node*> LaserParticles = NodeHandles[4];
				std::vector<Node*> LaserParticleReps = NodeHandles[5];
				std::vector<Node*> LaserLightSpheres = NodeHandles[6];
				std::vector<Node*> LightSpheres = NodeHandles[7];
				DestructableStaticBall* turb = nullptr;
				if(Turbine.size()>0)
					turb = (DestructableStaticBall*)Turbine[0];
				float energy = 0.0f;
				if(turb !=nullptr)
					energy = 1.0f + ((turb->temperature - turb->trgTemperature)/(turb->maxTemperature - turb->trgTemperature));
				for(int i=0;i<LightSpheres.size();i++)
				{
					((LightSourceObject*)LightSpheres[i])->Color.a=energy;
					if(LightSpheres[i]->id == 3) ((LightSourceObject*)LightSpheres[i])->Color.a = energy * flags["room1"];
					if(LightSpheres[i]->id == 4) ((LightSourceObject*)LightSpheres[i])->Color.a = energy * flags["room2"];
				}
				flags["Laser1"] = false;
				flags["Laser2"] = false;
				flags["Laser3"] = false;
				for(int i=0;i<fuses.size();i++)
				{
					if(fuses[i]->Name == "L1Fuse")
						flags["Laser1"] = true;
					if(fuses[i]->Name == "L2Fuse")
						flags["Laser2"] = true;
					if(fuses[i]->Name == "L3Fuse")
						flags["Laser3"] = true;
				}
				for(int i=0;i<Lasers.size();i++)
				{
					DestructableStaticBall* lsr = (DestructableStaticBall*)Lasers[i];
					if(lsr->temperature>lsr->maxTemperature * 0.8f)
					{
						if(fuses[i]->Name == "laser1")
							flags["Laser1"] = true;
						if(fuses[i]->Name == "laser2")
							flags["Laser2"] = true;
						if(fuses[i]->Name == "laser3")
							flags["Laser3"] = true;
					}
				}

				if(Turbine.size()<=0)
				{
					flags["Laser1"] = false;
					flags["Laser2"] = false;
					flags["Laser3"] = false;
				}
				
				for(int i=0;i<LaserVisuals.size();i++) 
				{
					if (LaserVisuals[i]->Name == "laservisual1") ((Object*)LaserVisuals[i])->Color.a = 1.0f * flags["Laser1"];
					if (LaserVisuals[i]->Name == "laservisual2") ((Object*)LaserVisuals[i])->Color.a = 1.0f * flags["Laser2"];
					if (LaserVisuals[i]->Name == "laservisual3") ((Object*)LaserVisuals[i])->Color.a = 1.0f * flags["Laser3"];
				}
				
				for(int i=0;i<LaserParticles.size();i++) 
				{
					if (LaserParticles[i]->Name == "laserSparkEmiter1") ((ParticleObject*)LaserParticles[i])->amount = 5 * flags["Laser1"];
					if (LaserParticles[i]->Name == "laserSparkEmiter2") ((ParticleObject*)LaserParticles[i])->amount = 5 * flags["Laser2"];
					if (LaserParticles[i]->Name == "laserSparkEmiter3") ((ParticleObject*)LaserParticles[i])->amount = 5 * flags["Laser3"];
				}
				
				for(int i=0;i<LaserLightSpheres.size();i++) 
				{
					if (LaserLightSpheres[i]->Name == "laserls1") ((LightSourceObject*)LaserLightSpheres[i])->Color.a = energy * flags["Laser1"];
					if (LaserLightSpheres[i]->Name == "laserls2") ((LightSourceObject*)LaserLightSpheres[i])->Color.a = energy * flags["Laser2"];
					if (LaserLightSpheres[i]->Name == "laserls3") ((LightSourceObject*)LaserLightSpheres[i])->Color.a = energy * flags["Laser3"];
				}
				
				
				if(Entities.size()==1)
				{
					compleated = true;
				}



				

			}break;
			case 4:
			{
				std::vector<Node*> Turbines = NodeHandles[0];
				std::vector<Node*> LaserParts = NodeHandles[1];
				std::vector<Node*> Misc = NodeHandles[2];
				Node* monitor = nullptr;
				std::array<int,6> tstatuses = {-1,-1,-1,-1,-1,-1};
				bool TurbinesOk = true;
				for(auto n : Misc)
				{
					if(n->Name == "turbineMonitor")
						monitor = n;
				}	
				for(auto n : Misc)
				{
					if(n->Name == "turbineMonitor")
						monitor = n;
					if(n->Name == "LabMonitor" && flags["LaserShot"])
					{
						((Monitor*)n)->text1 = "";
						((Monitor*)n)->text2 = "";
						((Monitor*)n)->text3 = "";
						((Monitor*)n)->text4 = "";
						((Monitor*)n)->text5 = "";
						((Monitor*)n)->text6 = "";
						((Monitor*)n)->text7 = "";
						((Monitor*)n)->text8 = "";
						((Monitor*)n)->text9 = "";
						((Monitor*)n)->text10 = "No input";
						((Monitor*)n)->textScale = 0.8f;
						((Monitor*)n)->textOffset.y = 5.0f;
					}
				}
				int turbiter = 0;
				for(int i = 0; i < Turbines.size();i++)
				{
					
					DestructableStaticBall* turb = (DestructableStaticBall*)Turbines[i];
					if(turb->temperature>30.0f)
					{
						turb->trgTemperature = 50.0f;
						tstatuses[turbiter] = 1;
					}
					else
						tstatuses[turbiter] = 0;
					
					turbiter++;
					if(turbiter > 5)
						turbiter = 0;
				}

				if(monitor!=nullptr)
				{
					Monitor* m = (Monitor*)monitor;
					if(tstatuses[0] == -1) 	{m->text2 = "";}
					if(tstatuses[0] == 0)	{m->text2 = "t1 - frozen";	m->color2 = {4.5f,0.1f,0.1f,1.0f};}
					if(tstatuses[0] == 1)	{m->text2 = "t1 - OK"; 		m->color2 = {0.1f,4.5f,0.1f,1.0f};}

					if(tstatuses[1] == -1) 	{m->text3 = "";}
					if(tstatuses[1] == 0)	{m->text3 = "t2 - frozen";	m->color3 = {4.5f,0.1f,0.1f,1.0f};}
					if(tstatuses[1] == 1)	{m->text3 = "t2 - OK";	 	m->color3 = {0.1f,4.5f,0.1f,1.0f};}

					if(tstatuses[2] == -1)	{m->text4 = "";}
					if(tstatuses[2] == 0) 	{m->text4 = "t3 - frozen";	m->color4 = {4.5f,0.1f,0.1f,1.0f};}
					if(tstatuses[2] == 1) 	{m->text4 = "t3 - OK"; 		m->color4 = {0.1f,4.5f,0.1f,1.0f};}

					if(tstatuses[3] == -1)	{m->text5 = "";}
					if(tstatuses[3] == 0) 	{m->text5 = "t4 - frozen";	m->color5 = {4.5f,0.1f,0.1f,1.0f};}
					if(tstatuses[3] == 1) 	{m->text5 = "t4 - OK"; 		m->color5 = {0.1f,4.5f,0.1f,1.0f};}

					if(tstatuses[4] == -1)	{m->text6 = "";}
					if(tstatuses[4] == 0) 	{m->text6 = "t5 - frozen";	m->color6 = {4.5f,0.1f,0.1f,1.0f};}
					if(tstatuses[4] == 1) 	{m->text6 = "t5 - OK"; 		m->color6 = {0.1f,4.5f,0.1f,1.0f};}

					if(tstatuses[5] == -1)	{m->text7 = "";}
					if(tstatuses[5] == 0) 	{m->text7 = "t6 - frozen";	m->color7 = {4.5f,0.1f,0.1f,1.0f};}
					if(tstatuses[5] == 1) 	{m->text7 = "t6 - OK"; 		m->color7 = {0.1f,4.5f,0.1f,1.0f};}

					if(tstatuses[0] == 0 || tstatuses[1] == 0 || tstatuses[2] == 0 || tstatuses[3] == 0 || tstatuses[4] == 0 || tstatuses[5] == 0)
						TurbinesOk = false;
					
					if(TurbinesOk)
					{
						m->text8 = "All turbines operating"; 				m->color8 =  {0.1f,4.5f,0.1f,1.0f};
						m->text9 = "Experimantal equipment operational"; 	m->color9 =  {0.1f,4.5f,0.1f,1.0f};
						m->text10 = "Power supply stable"; 					m->color10 = {0.1f,4.5f,0.1f,1.0f};
						m->bleepLastLine = false;
						if(!flags["LaserShot"])
						{
							flags["LaserShot"] = true;
							timers[0] = 5.0f;	
						}
					}
					else
					{
						m->text8 = "1 or more turbines mulfunction detected"; 		m->color8 =  {4.5f,0.1f,0.1f,1.0f};
						m->text9 = "Experimantal and production equipment offline"; m->color9 =  {4.5f,0.1f,0.1f,1.0f};
						m->text10 = "Emergency power supply"; 						m->color10 = {4.5f,0.1f,0.1f,1.0f};
						m->bleepLastLine = true;
					
					}
				}
				if(timers[0]>0.0f && flags["LaserShot"])
				{
					ScreenShake = timers[0] * 0.2f;
					ChromaticAbberation = timers[0] * 0.7f;
					for(auto n : LaserParts)
					{
						if(n->Name == "LaserPE")
							((ParticleObject*)n)->amount = 1;
						if(n->Name == "LaserDS")
						{
							((StaticDamageSphere*)n)->damage = 5.0f;
							((StaticDamageSphere*)n)->heat = 1.0f;
						}
						if(n->Name == "LaserLS")
						{
							((LightSourceObject*)n)->Color.a = timers[0]*1.0f;
						}
						if(n->Name == "Laser1")
						{
							((Object*)n)->Color.a = timers[0]*0.2f;
						}

					}
				}
				else
				{
					for(auto n : LaserParts)
					{
						if(n->Name == "LaserDS")
						{
							((StaticDamageSphere*)n)->damage = 0.0f;
							((StaticDamageSphere*)n)->heat = 0.0f;
						}
						if(n->Name == "LaserLS")
						{
							((LightSourceObject*)n)->Color.a = 0.0f;
						}
						if(n->Name == "Laser1")
						{
							((Object*)n)->Color.a = timers[0]*0.2f;
						}

					}
				
				}
			}break;
			default:
				break;
			}
		
	}
	void exitMission(bool extracted = false)
	{
		exiting = true;
		if(Exposure >0.8f)
		{
			Exposure = -0.85f;
		}
		if (Exposure > 0.0f)
		{
			if(extracted)
			{
				Materials += materialReward;
				if(Entities.size()>0 && !Entities[0]->dead && !Entities[0]->destroyed)
				{
					Materials += GetShipCost(Entities[0]);
					Entities[0]->SaveTo(EntityBackUpName);

				}
			}
			exitedmission = true;
			inbase = true;
			exiting = false;
			GetWindow(BackgroundWindowID)->w_DirectionalLight = 1.0f;
			ChangeMap("Scenes/base.sav", false);
			SpawnPlayer(EntityBackUpName);
		}
	}
};


