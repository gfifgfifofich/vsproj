#pragma once





int GetShipCost(CentralPart* Ent)// returns -1 if not an ship, returns cost if succesfull
{
	int cost = -1;
	

	for(int e=0;e<Ent->Parts.size();e++)
		for(int i=0;i<PartInstances.size();i++)
		{
			if(PartInstances[i]->type == Ent->Parts[e]->type)
				{
					cost +=PartInstances[i]->Cost.Matter;
					break;
				}
		}

	return cost;
}

int CheckShipSaveFile(std::string filename)// returns -1 if not an ship, returns cost if succesfull
{
	std::string fn = "./Ships/"+filename + ".sav";
	filename = fn;
	std::ifstream f(filename);
	int cost = -1;
	if (!f.is_open())
	{
		return cost;
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

			int type = 0;

			s >> junk >> type;
			for(int i=0;i<PartInstances.size();i++)
			{
				if(PartInstances[i]->type == NodeType::LASTNODE + type)
					{
						cost +=PartInstances[i]->Cost.Matter;
						break;
					}
			}
		}
	}
	f.close();
	return cost;
}

int _MultithreadEntitiesProcesStep = 0;


float subdt = 0.0017;

void EntityToEntityCollision(CentralPart* e1, CentralPart* e2)
{
	ball e1Collider;
	e1Collider.position = e1->CenterOfMass;
	e1Collider.r = e1->maxR;
	ball e2Collider;
	e2Collider.position = e2->CenterOfMass;
	e2Collider.r = e2->maxR;
	if(!BtBCollisionCheck(e1Collider,e2Collider))
		return;

	float maxdif = 7.0f;
	if(e1->Balls.size()>e2->Balls.size())
		for(auto i : e1->Balls)
			for(auto a : e2->Balls)
				{
					glm::vec2 prevvel  = ballVelocity[i];
					glm::vec2 prevvel2 = ballVelocity[a];
					BtBCollision(i,a);
					
					glm::vec2 dif = prevvel - ballVelocity[i];
					float len = sqrlength(dif);
					if (len > maxdif * maxdif)
						playsound(Hit, ballPosition[i],0.5f,0.1f + (rand() % 100 * 0.00025f), ballVelocity[i],false);
					 dif = prevvel2 - ballVelocity[a];
					len = sqrlength(dif);
					if (len > maxdif * maxdif)
						playsound(Hit, ballPosition[a],0.5f,0.1f + (rand() % 100 * 0.00025f), ballVelocity[a],false);
				}
	else
		for(auto i : e2->Balls)
			for(auto a : e1->Balls)
				{
					glm::vec2 prevvel  = ballVelocity[i];
					glm::vec2 prevvel2 = ballVelocity[a];
					BtBCollision(i,a);
					
					glm::vec2 dif = prevvel - ballVelocity[i];
					float len = sqrlength(dif);
					if (len > maxdif * maxdif)
						playsound(Hit, ballPosition[i],0.5f,0.1f + (rand() % 100 * 0.00025f), ballVelocity[i],false);
					 dif = prevvel2 - ballVelocity[a];
					len = sqrlength(dif);
					if (len > maxdif * maxdif)
						playsound(Hit, ballPosition[a],0.5f,0.1f + (rand() % 100 * 0.00025f), ballVelocity[a],false);
				}

	/*
	for (int x = 1; x < 300; x++)
		for (int y = 1; y < 300; y++)
		{
			if (Grid[x][y].size <= 0)
				continue;
			balllbuffersize = 0;
			for (int i = 0; i < Grid[x][y].size; i++)
			{
				ballbuffer[balllbuffersize] = Grid[x][y].balls[i];
				balllbuffersize++;
			}
			for (int i = 0; i < Grid[x - 1][y].size; i++)
			{
				ballbuffer[balllbuffersize] = Grid[x - 1][y].balls[i];
				balllbuffersize++;
			}
			for (int i = 0; i < Grid[x][y - 1].size; i++)
			{
				ballbuffer[balllbuffersize] = Grid[x][y - 1].balls[i];
				balllbuffersize++;
			}
			for (int i = 0; i < balllbuffersize - 1; i++)
			{
				bool NotSelfCollision = false;

				glm::vec2 pVel = ballbuffer[i]->velocity;
				for (int a = i + 1; a < balllbuffersize; a++)
				{
					if (!NotSelfCollision)
						NotSelfCollision = ballbuffer[i] != ballbuffer[a];
					BtBCollision(ballbuffer[i], ballbuffer[a]);
				}
				glm::vec2 aVel = ballbuffer[i]->velocity;
				if (ballbuffer[i]->soundcd <=0.0f  && ((!snapToGrid || !BuildingMode) && NotSelfCollision))
				{
					float maxdif = 7.0f;

					glm::vec2 dif = pVel - aVel;
					float len = sqrlength(dif);
					if (len > maxdif * maxdif)
					{//heavy collision
						len = sqrt(len);

						glm::vec2 position = ballbuffer[i]->position + ballbuffer[i]->GroundVector * ballbuffer[i]->r;
						int amount = rand() % 10 + 0.001f * len;
						for (int a = 0; a < amount; a++)
							CollisionSparks.Spawn(position,
								-ballbuffer[i]->GroundVector * 150.0f * len * 0.001f + (rand() % 100 - 50.0f) * glm::vec2(-ballbuffer[i]->GroundVector.y, ballbuffer[i]->GroundVector.x) * len * 0.001f +
								Rotate(ballbuffer[i]->GroundVector * DOT(Rotate(ballbuffer[i]->GroundVector, pi * 0.5f), pVel * 0.1f)
									* float(rand() % 100) * 0.1f, pi * 0.5f),
								1);
						CollisionSmoke.Spawn(position,
							15);

						PlayCollisionSound(ballbuffer[i]->position);
						ballbuffer[i]->soundcd = 1.0f; 
					}

				}
			}

			balllbuffersize = 0;
		}
	*/
}

void ProcessEntities(float dt,int s)
{
	ProcessDamageSpheres(dt);
	ProcessLasers(dt, false);
	ProcessBullets(dt, true);
	ProcessRockets(dt);
	ProcessLasers(dt, false,true);
	ProcessExplodions(dt);
	// refactor required//?

	//for (int x = 0; x < 300; x++)
	//	for (int y = 0; y < 300; y++)
	//		Grid[x][y].size = 0;
	


	for (int i = 0; i < Entities.size(); i++)
		if (Entities[i]->Alive)
		{
			Entities[i]->freq = Speed;


			if (Entities[i]->autocontrol && i != 0)
			{
				Entities[i]->Playerpos = Entities[0]->mid;

			}
		}

	subdt = dt ;
	Debris.Process(subdt, s, s == substeps - 1);
	balltaken = false;


	//for (int i = 0; i < Debris.Parts.size(); i++)
	//{
	//	balls.reserve(Debris.Parts[i]->BodyIdsWithCollision.size());
	//	for (auto pid : Debris.Parts[i]->BodyIdsWithCollision)
	//		balls.push_back(&Debris.Parts[i]->body[pid]);
	//}
	//for (int e = 0; e < Entities.size(); e++)
	//{
	//	balls.reserve(Entities[e]->Balls.size());
	//	for (int i = 0; i < Entities[e]->Balls.size(); i++)
	//		balls.push_back(Entities[e]->Balls[i]);
	//}

	for (int i = 0; i < lastid; i++)
	{
		if (!IsBall[i])
			continue;
		// friction
		glm::vec2 aVel = ballVelocity[i];
		glm::vec2 pVel = ballVelocityBuff[i];
		ballVelocity[i] -= ballVelocity[i] * sqrlength(ballVelocity[i]) * subdt * 0.000002f;
		glm::vec2 offset = Entities[0]->mid + glm::vec2(-150, -150);
		//int x = roundf(balls[i]->position.x - offset.x);
		//int y = roundf(balls[i]->position.y - offset.y);
		////std::cout << x << "			" << y << "\n";
		//if (y >= 0 && y < 300 &&
		//	x >= 0 && x < 300)
		//	Grid[x][y].add(balls[i]);

		ballSoundcd[i] -= subdt;
		if(ballSoundcd[i] < -1.0f)
			ballSoundcd[i] = -1.0f;


		if (ballSoundcd[i] <=0.0f && (!snapToGrid || !BuildingMode))
		{
			float maxdif = 7.0f;

			glm::vec2 dif = pVel - aVel;
			float len = sqrlength(dif);
			if (len > maxdif * maxdif)
			{//heavy collision
				len = sqrt(len);

				glm::vec2 position = ballPosition[i];
				int amount = rand() % 10 + 0.001f * len;
				for (int a = 0; a < amount; a++)
					CollisionSparks.Spawn(position,
						dif * 150.0f * 0.0001f * (rand() % 100 + 50.0f),
						1);
				CollisionSmoke.Spawn(position,
					15);
				
				playsound(Hit,ballPosition[i],0.5f,0.1f + (rand() % 100 * 0.00025f), ballVelocity[i],false);
				ballSoundcd[i] = 1.0f;
				
			}

		}

		


		if (BuildingMode)
		{
			if (in_UI <= 0)
				if (JustPressedbutton[GLFW_MOUSE_BUTTON_2])
					if (sqrlength(ballPosition[i] - MousePosition)<PARTSIZE*PARTSIZE && !balltaken)
					{
						balltaken = true;
						GrabbedBall = i;
					}

			if (JustReleasedbutton[GLFW_MOUSE_BUTTON_2])
				GrabbedBall = NULL;


			if (keys[GLFW_KEY_LEFT_ALT])
				DrawCircle(ballPosition[i], PARTSIZE, glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), false, BallNormalMapTexture, 10);

			else if (keys[GLFW_KEY_LEFT_SHIFT])
				DrawCircle(ballPosition[i], PARTSIZE, glm::vec4(1.1f, 0.1f, 0.1f, 1.0f), false, BallNormalMapTexture, 10);

			if (snapToGrid)
			{
				ballVelocity[i] -= ballVelocity[i] * length(ballVelocity[i]) * subdt * 2.0f;

				/*glm::vec2 snapvector = glm::vec2(
					((int)(balls[i]->position.x + 40 - 3000) % 1),
					((int)(balls[i]->position.y - 40 - 3000) % 1)
				);*/
				glm::vec2 snapvector = glm::vec2(
					roundf(ballPosition[i].x),
					roundf(ballPosition[i].y)
				);

				DrawCircle(snapvector, 0.25f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, BallNormalMapTexture, 1000);

				ballForce[i] += (snapvector - ballPosition[i]) * 10.0f;
			}
		}
		else
		{
			GrabbedBall = NULL;
		}

	}

	/*
	for (int x = 1; x < 300; x++)
		for (int y = 1; y < 300; y++)
		{
			if (Grid[x][y].size <= 0)
				continue;
			balllbuffersize = 0;
			for (int i = 0; i < Grid[x][y].size; i++)
			{
				ballbuffer[balllbuffersize] = Grid[x][y].balls[i];
				balllbuffersize++;
			}
			for (int i = 0; i < Grid[x - 1][y].size; i++)
			{
				ballbuffer[balllbuffersize] = Grid[x - 1][y].balls[i];
				balllbuffersize++;
			}
			for (int i = 0; i < Grid[x][y - 1].size; i++)
			{
				ballbuffer[balllbuffersize] = Grid[x][y - 1].balls[i];
				balllbuffersize++;
			}
			for (int i = 0; i < balllbuffersize - 1; i++)
			{
				bool NotSelfCollision = false;

				glm::vec2 pVel = ballbuffer[i]->velocity;
				for (int a = i + 1; a < balllbuffersize; a++)
				{
					if (!NotSelfCollision)
						NotSelfCollision = ballbuffer[i] != ballbuffer[a];
					BtBCollision(ballbuffer[i], ballbuffer[a]);
				}
				glm::vec2 aVel = ballbuffer[i]->velocity;
				if (ballbuffer[i]->soundcd <=0.0f  && ((!snapToGrid || !BuildingMode) && NotSelfCollision))
				{
					float maxdif = 7.0f;

					glm::vec2 dif = pVel - aVel;
					float len = sqrlength(dif);
					if (len > maxdif * maxdif)
					{//heavy collision
						len = sqrt(len);

						glm::vec2 position = ballbuffer[i]->position + ballbuffer[i]->GroundVector * ballbuffer[i]->r;
						int amount = rand() % 10 + 0.001f * len;
						for (int a = 0; a < amount; a++)
							CollisionSparks.Spawn(position,
								-ballbuffer[i]->GroundVector * 150.0f * len * 0.001f + (rand() % 100 - 50.0f) * glm::vec2(-ballbuffer[i]->GroundVector.y, ballbuffer[i]->GroundVector.x) * len * 0.001f +
								Rotate(ballbuffer[i]->GroundVector * DOT(Rotate(ballbuffer[i]->GroundVector, pi * 0.5f), pVel * 0.1f)
									* float(rand() % 100) * 0.1f, pi * 0.5f),
								1);
						CollisionSmoke.Spawn(position,
							15);

						PlayCollisionSound(ballbuffer[i]->position);
						ballbuffer[i]->soundcd = 1.0f; 
					}

				}
			}

			balllbuffersize = 0;
		}
	*/

	int i = 0;
	while (i < Entities.size())
	{
		if (Entities[i]->Alive && !Entities[i]->destroyed && Entities[i]->Health>0.0f && !Entities[i]->Delete)
		{
			for(int a = i+1; a< Entities.size();a++)
			{
				if (Entities[a]->Alive && Entities[a]->Health>0.0f && !Entities[a]->Delete && !Entities[a]->destroyed)
					EntityToEntityCollision(Entities[i],Entities[a]);
				
			}
		}
		i++;
	}
	
	

}
