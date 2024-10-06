#include "../../Include/Helper.h"
#include "../../Include/Drawing.h"
#include "../../Include/Objects/Particle.h"
#include <thread>
#include <condition_variable>
#include <mutex>




bool _pthreads_stop;

std::thread* _pthreads;
std::condition_variable* _pConVars;
std::mutex* _pMutexes;
static inline std::atomic<bool>* _pthreadsStates;// 1 waiting | 0 working | -1 done
   

ParticleEmiter* _ptheadstarget;
void _pthead_Process(int thr);
void _Startpthread(int t);
void _Startpthreads();
void _Deletepthreads();

void _pthead_Process(int thr)
{
	std::unique_lock<std::mutex> lm(_pMutexes[thr]);
	_pConVars[thr].wait(lm);
	if(_ptheadstarget!=NULL)
		_ptheadstarget->_Process(thr);
	_pthreadsStates[thr].store(1);
}

void _Startpthread(int t)
{
	while(!_pthreads_stop)
	{
		_pthead_Process(t);
	}
}

void _Startpthreads()
{
	_ptheadstarget = NULL;
	_pthreads_stop = false;
	_pMutexes = new std::mutex[threadcount];
	_pConVars = new std::condition_variable[threadcount];
	_pthreads = new std::thread[threadcount];
	_pthreadsStates = new std::atomic<bool>[threadcount];

	for(int i=0;i<threadcount;i++)
	{
		_pthreads[i] = std::thread(_Startpthread,i);
		_pthreadsStates[i].store(true);
	}
}



void _Deletepthreads()
{
	_pthreads_stop = true;
	
	_ptheadstarget->Particles.clear();
	for(int thr = 0;thr<threadcount;thr++)
	{
		std::unique_lock<std::mutex> lm(_pMutexes[thr]);
		_pConVars[thr].notify_one();
	}
	int sum = 0;
	while(sum!=-threadcount)
	{
		_pthreads_stop = true;//just in case

		sum=0;
		for(int thr = 0;thr<threadcount;thr++)
			sum += _pthreadsStates[thr];
		
	}

	for(int thr = 0;thr<threadcount;thr++)
		_pthreads[thr].join();
}

int ParticleEmiter::AddSpheresOfInfluence(glm::vec2 position, float r, glm::vec2 velocity, bool Attractive, float AttractionStrength )//returns index of created sphere
{
	influenceSphere s;
	s.position = position;
	s.r = r;
	s.velocity = velocity;
	s.attractive = Attractive;
	s.attractionStrength = AttractionStrength;
	SpheresOfInfluence.push_back(s);
	return SpheresOfInfluence.size() - 1;
}

int ParticleEmiter::AddCubeOfInfluence(glm::vec2 position, glm::vec2 scale, glm::vec2 velocity, bool Attractive, float AttractionStrength)//returns index of created Cube
{
	influenceCube a;

	a.position = position;
	a.scale = scale;
	a.velocity = velocity;
	a.attractive = Attractive;
	a.attractionStrength = AttractionStrength;
	CubesOfInfluence.push_back(a);
	return CubesOfInfluence.size() - 1;
}
int ParticleEmiter::AddCubeEmiter(glm::vec2 position, glm::vec2 scale, glm::vec2 velocity, float amount, float tick)//returns index of created Cube
{
	EmitionCube a;

	a.position = position;
	a.scale = scale;
	a.velocity = velocity;
	a.amount = amount;
	a.tick = tick;
	EmitionCubes.push_back(a);
	return EmitionCubes.size() - 1;
}
int ParticleEmiter::AddCircleEmiter(glm::vec2 position, glm::vec2 velocity, float r, float amount, float tick)//returns index of created Circle
{
		EmitionCircle a;

		a.position = position;
		a.velocity = velocity;
		a.r = r;
		a.amount = amount;
		a.tick = tick;
		EmitionCircles.push_back(a);
		return EmitionCircles.size() - 1;
	}
int ParticleEmiter::AddPointEmiter(glm::vec2 position, glm::vec2 velocity, float amount, float tick)//returns index of created point
	{
		EmitionPoint a;

		a.position = position;
		a.velocity = velocity;
		a.amount = amount;
		a.tick = tick;
		EmitionPoints.push_back(a);
		return EmitionPoints.size() - 1;
	}

int ParticleEmiter::AddLightSphere(glm::vec2 position, float r, glm::vec4 Color)//returns index of created point
	{
		LightSphere a;
		a.position = position;
		a.r = r;
		a.Color = Color;
		LightSpheres.push_back(a);
		return LightSpheres.size() - 1;
	}
int ParticleEmiter::AddLightCube(glm::vec2 position, glm::vec2 scale, glm::vec4 Color)//returns index of created point
	{
		LightCube a;
		a.position = position;
		a.scale = scale;
		a.Color = Color;
		LightCubes.push_back(a);
		return LightCubes.size() - 1;
	}

void ParticleEmiter::Process(float dt)
{
	Partdelta = dt;


	if (ShowWindow)
	{

		ImGui::Begin("Particles");
		float vec[2];
		vec[0] = InitialVelocity.x;
		vec[1] = InitialVelocity.y;
		ImGui::DragFloat2("Initial velocity", vec);
		InitialVelocity.x = vec[0];
		InitialVelocity.y = vec[1];

		vec[0] = acceleration.x;
		vec[1] = acceleration.y;
		ImGui::DragFloat2("acceleration", vec);
		acceleration.x = vec[0];
		acceleration.y = vec[1];


		vec[0] = VelocityRandomness.x;
		vec[1] = VelocityRandomness.y;
		ImGui::DragFloat2("VelocityRandomness X", vec);
		VelocityRandomness.x = vec[0];
		VelocityRandomness.y = vec[1];

		vec[0] = VelocityRandomness.z;
		vec[1] = VelocityRandomness.w;
		ImGui::DragFloat2("VelocityRandomness Y", vec);
		VelocityRandomness.z = vec[0];
		VelocityRandomness.w = vec[1];

		ImGui::SliderFloat("VelocityDamper", &VelocityDamper, 0, 1.0f);

		vec[0] = StartSize.x;
		vec[1] = StartSize.y;
		ImGui::DragFloat2("StartSize", vec, 0.1f);
		StartSize.x = vec[0];
		StartSize.y = vec[1];

		vec[0] = EndSize.x;
		vec[1] = EndSize.y;
		ImGui::DragFloat2("EndSize", vec, 0.1f);
		EndSize.x = vec[0];
		EndSize.y = vec[1];

		float color[4];
		color[0] = StartColor.x;
		color[1] = StartColor.y;
		color[2] = StartColor.z;
		color[3] = StartColor.w;
		ImGui::ColorEdit4("StartColor", color);
		StartColor.x = color[0];
		StartColor.y = color[1];
		StartColor.z = color[2];
		StartColor.w = color[3];

		color[0] = EndColor.x;
		color[1] = EndColor.y;
		color[2] = EndColor.z;
		color[3] = EndColor.w;
		ImGui::ColorEdit4("EndColor", color);
		EndColor.x = color[0];
		EndColor.y = color[1];
		EndColor.z = color[2];
		EndColor.w = color[3];

		ImGui::DragFloat("InitialRotation", &InitialRotation, 0.01f);
		ImGui::DragFloat("RotationRandomness", &RotationRandomness, 0.01f);
		ImGui::DragFloat("RotationVelocity", &RotationVelocity, 0.01f);
		ImGui::DragFloat("RotationAcceleration", &RotationAcceleration, 0.01f);
		ImGui::DragFloat("RotationDamper", &RotationDamper, 0.01f);

		ImGui::DragFloat("InitialOrbitalVelocity", &InitialOrbitalVelocity, 0.01f);
		ImGui::DragFloat("OrbitalVelocityRandomness", &OrbitalVelocityRandomness, 0.01f);

		ImGui::DragFloat("lifetime", &lifetime, 0.1f);
		ImGui::SliderFloat("lifetimeRandomness", &lifetimeRandomness, 0, 1);
		ImGui::Checkbox("Influenced", &influenced);
		ImGui::Checkbox("Lighted", &lighted);
		ImGui::Checkbox("DrawToNormalMap", &DrawToNormalMap);
		ImGui::Checkbox("Additive", &Additive);


		ImGui::End();
	}
	for (int i = 0; i < EmitionPoints.size(); i++)
	{
		EmitionPoints[i].t += dt;
		if (EmitionPoints[i].t > EmitionPoints[i].tick)
		{
			EmitionPoints[i].t = 0.0f;
			Spawn(EmitionPoints[i].position, EmitionPoints[i].velocity, EmitionPoints[i].amount);
		}
	}
	for (int i = 0; i < EmitionCubes.size(); i++)
	{
		EmitionCubes[i].t += dt;
		if (EmitionCubes[i].t > EmitionCubes[i].tick)
		{
			EmitionCubes[i].t = 0.0f;
			for (int a = 0; a < EmitionCubes[i].amount; a++)
			{
				glm::vec2 pos = EmitionCubes[i].position;
				pos.x += rand() % ((int)EmitionCubes[i].scale.x * 100) * 0.02f - EmitionCubes[i].scale.x;
				pos.y += rand() % ((int)EmitionCubes[i].scale.y * 100) * 0.02f - EmitionCubes[i].scale.y;
				Spawn(pos, EmitionCubes[i].velocity);
			}
		}
	}
	for (int i = 0; i < EmitionCircles.size(); i++)
	{
		EmitionCircles[i].t += dt;
		if (EmitionCircles[i].t > EmitionCircles[i].tick)
		{
			EmitionCircles[i].t = 0.0f;
			for (int a = 0; a < EmitionCircles[i].amount; a++)
			{
				glm::vec2 pos = EmitionCircles[i].position;
				float Radius = 1.0f;
				if((int)EmitionCircles[i].r * 100 !=0)
					Radius = ((rand() * 100) % ((int)EmitionCircles[i].r * 100))*0.01f * 0.7f;
				
				float angle = rand()*0.001f;
				pos += glm::vec2((cos(angle) - sin(angle)) * Radius, (cos(angle) + sin(angle)) * Radius);


				Spawn(pos, EmitionCircles[i].velocity);
			}
		}
	}

	std::vector <int> iter;
	iter.resize(threadcount);

	for (int i = 0; i < threadcount; i++)
		iter[i] = i;



	int SLI = FindSceneLayer(Z_Index, Additive);
	SceneLayerIndex = SLI;

	//std::cout << "Particle SLI = " << SLI << " Z_Index = " << SceneLayers[SLI].Z_Index<<" Name = "<<Name<<"\n";

	unsigned int NormalMap = material.NormalMap;

	if (Type == "CIRCLE")
	{
		start = SceneLayers[SceneLayerIndex].CirclePosScale.size();
		SceneLayers[SceneLayerIndex].CirclePosScale.resize(SceneLayers[SceneLayerIndex].CirclePosScale.size() + Particles.size());
		SceneLayers[SceneLayerIndex].Circlecolors.resize(SceneLayers[SceneLayerIndex].Circlecolors.size() + Particles.size());
		SceneLayers[SceneLayerIndex].CircleRotations.resize(SceneLayers[SceneLayerIndex].CircleRotations.size() + Particles.size());

		if (DrawToNormalMap)
		{
			TQA = -1;

			for (int i = 0; i < SceneLayers[SceneLayerIndex].NormalMaps.size(); i++)
				if (SceneLayers[SceneLayerIndex].NormalMaps[i].material.Texture == NormalMap)
					TQA = i;
			if (TQA == -1)
			{
				TexturedQuadArray NewTQA;
				NewTQA.material.Texture = NormalMap;
				SceneLayers[SceneLayerIndex].NormalMaps.push_back(NewTQA);
				for (int i = 0; i < SceneLayers[SceneLayerIndex].NormalMaps.size(); i++)
					if (SceneLayers[SceneLayerIndex].NormalMaps[i].material.Texture == NormalMap)
						TQA = i;
			}
			NormalMapIndex = TQA;

			Normastart = SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size();
			SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.resize(SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size() + Particles.size());
			SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadRotations.resize(SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size() + Particles.size());
			SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadDepth.resize(SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size() + Particles.size());
			

		}
	}
	else if (Type == "QUAD" || Type == "LINE")
	{
		start = SceneLayers[SceneLayerIndex].Quadcolors.size();
		SceneLayers[SceneLayerIndex].Quadcolors.resize(SceneLayers[SceneLayerIndex].Quadcolors.size() + Particles.size());
		SceneLayers[SceneLayerIndex].QuadPosScale.resize(SceneLayers[SceneLayerIndex].QuadPosScale.size() + Particles.size());
		SceneLayers[SceneLayerIndex].QuadRotations.resize(SceneLayers[SceneLayerIndex].QuadRotations.size() + Particles.size());
		if (DrawToNormalMap)
		{
				TQA = -1;

				for (int i = 0; i < SceneLayers[SceneLayerIndex].NormalMaps.size(); i++)
					if (SceneLayers[SceneLayerIndex].NormalMaps[i].material.Texture == NormalMap)
						TQA = i;
				if (TQA == -1)
				{
					TexturedQuadArray NewTQA;
					NewTQA.material.Texture = NormalMap;
					SceneLayers[SceneLayerIndex].NormalMaps.push_back(NewTQA);
					for (int i = 0; i < SceneLayers[SceneLayerIndex].NormalMaps.size(); i++)
						if (SceneLayers[SceneLayerIndex].NormalMaps[i].material.Texture == NormalMap)
							TQA = i;
				}
				NormalMapIndex = TQA;

				Normastart = SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size();
				SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.resize(SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size() + Particles.size());
				SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadRotations.resize(SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size() + Particles.size());
				SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadDepth.resize(SceneLayers[SceneLayerIndex].NormalMaps[TQA].QuadPosScale.size() + Particles.size());
			

		}
	}
	else if ((Type == "TEXTURED" || Type == "TEXTUREDLINE") && material.Texture > 0)
	{

		TQA = -1;


		for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
			if (SceneLayers[SLI].TexturedQuads[i].material == material)
				TQA = i;
		if (TQA == -1)
		{
			TexturedQuadArray NewTQA;
			NewTQA.material = material;
			SceneLayers[SLI].TexturedQuads.push_back(NewTQA);
			for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
				if (SceneLayers[SLI].TexturedQuads[i].material == material)
					TQA = i;
		}
		start = SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors.size();

		SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors.resize(SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors.size() + Particles.size());
		SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadPosScale.resize(SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadPosScale.size() + Particles.size());
		SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations.resize(SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations.size() + Particles.size());
		SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadDepth.resize(SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations.size() + Particles.size());


	}

	_ptheadstarget = this;
	if(ParticleMultithreading) // Multithreaded drawing is not working stable, only particle processing is MT, Pushing to draw arrays is singlethread
	{
		if (Particles.size() > 1000)
		{
			for(auto thr : iter)
			{
				_pthreadsStates[thr].store(0);
				std::unique_lock<std::mutex> lm(_pMutexes[thr]);
				_pConVars[thr].notify_one();
			}

			bool wait = true;
			float startWaittime = glfwGetTime(); 
			while(wait)
			{
				wait = false;
				for(int thr = 0;thr<threadcount;thr++)
				{
					if(!_pthreadsStates[thr].load())
						wait = true;
				}
				if(glfwGetTime() - startWaittime > Partdelta*10.0f) // something happend with threads
				{
					std::cout<<"\nsomething happend with threads in particles";
					for(int thr = 0;thr<threadcount;thr++)
					{						
						_pConVars[thr].notify_one();
						_pthreadsStates[thr].store(1);
					}
					break;
				}	
			}
		}
		else
		{
			int buf = threadcount;
			threadcount=1;
			_Process(0);
			threadcount = buf;

		}
	}
	else
	{
		if (Particles.size() > 1000)
			for(int thr = 0;thr<threadcount;thr++)
				_Process(thr);
		else
		{
			int buf = threadcount;
			threadcount=1;
			_Process(0);
			threadcount = buf;

		}
	}
	
	for(int i=0;i<Particles.size();i++)
	{
		if (Type == "CIRCLE") {

			float aspx = ScreenDivisorX * CameraScale.x;
			float aspy = ScreenDivisorY * CameraScale.y;

			glm::vec2 position = Particles[i].position - CameraPosition;
			position *= glm::vec2(aspx, aspy);


			SceneLayers[SceneLayerIndex].CirclePosScale[start + i] = glm::vec4(position, Particles[i].Size * glm::vec2(aspx, aspy));
			SceneLayers[SceneLayerIndex].Circlecolors[start + i] = Particles[i].color;
			if (DrawToNormalMap)
			{
			
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadPosScale[Normastart + i] = glm::vec4(position, Particles[i].Size * glm::vec2(aspx, aspy));
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadRotations[Normastart + i] = Particles[i].Rotation;
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadDepth[Normastart + i] = 0.0f;
				
			}
		}
		else if (Type == "QUAD")
		{
			float aspx = ScreenDivisorX * CameraScale.x;
			float aspy = ScreenDivisorY * CameraScale.y;

			glm::vec2 position = Particles[i].position - CameraPosition;
			position *= glm::vec2(aspx, aspy);



			SceneLayers[SceneLayerIndex].QuadPosScale[start + i] = glm::vec4(position, Particles[i].Size * glm::vec2(aspx, aspy));
			SceneLayers[SceneLayerIndex].QuadRotations[start + i] = Particles[i].Rotation;
			SceneLayers[SceneLayerIndex].Quadcolors[start + i] = Particles[i].color;
			if (DrawToNormalMap)
			{
				
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadPosScale[Normastart + i] = glm::vec4(position, Particles[i].Size * glm::vec2(aspx, aspy));
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadRotations[Normastart + i] = Particles[i].Rotation;
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadDepth[Normastart + i] = 0.0f;
				
			}

		}
		else if (Type == "LINE")
		{

			glm::vec2 p1 = Particles[i].position;
			glm::vec2 p2 = Particles[i].position + Particles[i].velocity * Particles[i].Size.y * 0.017f;
			glm::vec2 midpos = (p2 + p1) / 2.0f;
			float rotation = get_angle_between_points(p1, p2);
			glm::vec2 dif = p1 - p2;
			float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;

			float aspx = ScreenDivisorX * CameraScale.x;
			float aspy = ScreenDivisorY * CameraScale.y;

			glm::vec2 position = midpos - CameraPosition;
			position *= glm::vec2(aspx, aspy);


			//DrawCube(midpos, glm::vec2(length * 5.125f * 25, width), glm::vec3(0.0f, 0.0f, rotation), color);

			SceneLayers[SceneLayerIndex].QuadPosScale[start + i] = glm::vec4(position, glm::vec2(Particles[i].Size.x, Particles[i].Size.y * length) * glm::vec2(aspx, aspy));
			SceneLayers[SceneLayerIndex].QuadRotations[start + i] = rotation;
			SceneLayers[SceneLayerIndex].Quadcolors[start + i] = Particles[i].color;

			if (DrawToNormalMap)
			{
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadPosScale[Normastart + i] = glm::vec4(position, glm::vec2(Particles[i].Size.x, Particles[i].Size.y * length) * glm::vec2(aspx, aspy));
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadRotations[Normastart + i] = rotation;
				SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadDepth[Normastart + i] = 0.0f;
				
			}
		}
		else if (Type == "TEXTURED" && material.Texture > 0)
		{
			float aspx = ScreenDivisorX * CameraScale.x;
			float aspy = ScreenDivisorY * CameraScale.y;

			glm::vec2 position = Particles[i].position - CameraPosition;
			position *= glm::vec2(aspx, aspy);

			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors[start + i] = Particles[i].color;
			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadPosScale[start + i] = glm::vec4(position, Particles[i].Size * glm::vec2(aspx, aspy));
			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations[start + i] = Particles[i].Rotation;
			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadDepth[start + i] = 0.0f;

		}
		else if (Type == "TEXTUREDLINE" && material.Texture > 0)
		{

			glm::vec2 p1 = Particles[i].position;
			glm::vec2 p2 = Particles[i].position + Particles[i].velocity * Particles[i].Size.y * 0.017f;
			glm::vec2 midpos = (p2 + p1) / 2.0f;
			float rotation = get_angle_between_points(p1, p2);
			glm::vec2 dif = p1 - p2;
			float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;


			float aspx = ScreenDivisorX * CameraScale.x;
			float aspy = ScreenDivisorY * CameraScale.y;
			glm::vec2 position = midpos - CameraPosition;
			position *= glm::vec2(aspx, aspy);
			//DrawCube(midpos, glm::vec2(length * 5.125f * 25, width), glm::vec3(0.0f, 0.0f, rotation), color);


			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors[start + i] = Particles[i].color;
			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadPosScale[start + i] = glm::vec4(position, glm::vec2(Particles[i].Size.x, Particles[i].Size.y * length) * glm::vec2(aspx, aspy));
			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations[start + i] = rotation;
			SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadDepth[start + i] = 0.0f;


		}
	}
	
	int p = 0;
	while (p<Particles.size())
	{
		if (Particles[p].time <= 0.0f)
		{
			Particles[p] = Particles[Particles.size() - 1];
			Particles.pop_back();
		}
		else
			p++;
	}
}


void ParticleEmiter::_Process(int thr)
{
	
	int step = Particles.size() / threadcount;
	int begin = thr * step;
	int end = (thr + 1) * step;
	if (thr == 0)
		begin =0;

	if (thr == threadcount -1 || threadcount == 1)
		end =Particles.size() ;

	for (int i = begin; i < end; i++)
		if (i < Particles.size())
		{
			Particles[i].time -= Partdelta;
			if (Particles[i].time > 0.0f)
			{
				float stage = Particles[i].time / lifetime;

				glm::vec2 SizeDif = StartSize - EndSize;
				glm::vec4 ColorDif = StartColor - EndColor;

				glm::vec2 Size = EndSize + (SizeDif * stage);
				glm::vec4 color = EndColor + (ColorDif * stage);

				Particles[i].position += Particles[i].velocity * Partdelta;
				Particles[i].velocity += acceleration * Partdelta;
				Particles[i].velocity += glm::vec2(-Particles[i].velocity.y, Particles[i].velocity.x) * Particles[i].OrbitalVelocity * Partdelta;
				Particles[i].velocity -= Particles[i].velocity * VelocityDamper * Partdelta;

				Particles[i].Rotation += Particles[i].RotationVelocity * Partdelta;
				Particles[i].RotationVelocity += RotationAcceleration * Partdelta;
				Particles[i].RotationVelocity -= Particles[i].RotationVelocity * RotationDamper * Partdelta;
				if (influenced)
				{
					for (int s = 0; s < CaptureSpheres.size(); s++)
					{
						float dist = sqrlength(Particles[i].position - CaptureSpheres[s].position);
						if (dist < CaptureSpheres[s].r * CaptureSpheres[s].r)
						{
							Particles[i].velocity += CaptureSpheres[s].velocity * Partdelta;
							if (CaptureSpheres[s].attractive)
							{
								glm::vec2 dir = (CaptureSpheres[s].position - Particles[i].position);
								if (sqrlength(dir) <= 1.0f)
									Particles[i].velocity += (dir)*CaptureSpheres[s].attractionStrength * abs(CaptureSpheres[s].attractionStrength) * Partdelta;
								else
								{
									Particles[i].velocity += (dir) * CaptureSpheres[s].attractionStrength * abs(CaptureSpheres[s].attractionStrength) * Partdelta;
								}
							}
						}
					}
					for (int s = 0; s < SpheresOfInfluence.size(); s++)
					{
						float dist = sqrlength(Particles[i].position - SpheresOfInfluence[s].position);
						if (dist < SpheresOfInfluence[s].r * SpheresOfInfluence[s].r)
						{
							Particles[i].velocity += SpheresOfInfluence[s].velocity * Partdelta;
							if (SpheresOfInfluence[s].attractive)
							{
								glm::vec2 dir = (SpheresOfInfluence[s].position - Particles[i].position);
								if (sqrlength(dir) <= 1.0f)
									Particles[i].velocity += (dir)*SpheresOfInfluence[s].attractionStrength * abs(SpheresOfInfluence[s].attractionStrength) * Partdelta;
								else
								{
									float sdist = sqrt(dist);
									dir /= sdist;
									Particles[i].velocity += (dir / (sdist)) * SpheresOfInfluence[s].attractionStrength * abs(SpheresOfInfluence[s].attractionStrength) * Partdelta;
								}
							}
						}
					}
					for (int s = 0; s < CubesOfInfluence.size(); s++)
					{
						if (CubesOfInfluence[s].position.x + CubesOfInfluence[s].scale.x >= Particles[i].position.x &&
							CubesOfInfluence[s].position.x - CubesOfInfluence[s].scale.x <= Particles[i].position.x &&
							CubesOfInfluence[s].position.y + CubesOfInfluence[s].scale.y >= Particles[i].position.y &&
							CubesOfInfluence[s].position.y - CubesOfInfluence[s].scale.y <= Particles[i].position.y)
						{
							float dist = sqrlength(Particles[i].position - CubesOfInfluence[s].position);

							Particles[i].velocity += CubesOfInfluence[s].velocity * Partdelta;
							if (CubesOfInfluence[s].attractive)
							{
								glm::vec2 dir = (CubesOfInfluence[s].position - Particles[i].position);
								if (sqrlength(dir) <= 1.0f)
									Particles[i].velocity += (dir)*CubesOfInfluence[s].attractionStrength * abs(CubesOfInfluence[s].attractionStrength) * Partdelta;
								else
								{
									float sdist = sqrt(dist);
									dir /= sdist;
									Particles[i].velocity += (dir / (sdist)) * CubesOfInfluence[s].attractionStrength * abs(CubesOfInfluence[s].attractionStrength) * Partdelta;
								}
							}
						}
					}
				}

				if (lighted)
				{
					for (int s = 0; s < LightSpheres.size(); s++)
					{
						float dist = sqrlength(Particles[i].position - LightSpheres[s].position);
						float rr = LightSpheres[s].r * LightSpheres[s].r;
						if (dist < rr)
						{
							dist /= rr;
							color.r += LightSpheres[s].Color.r * LightSpheres[s].Color.a * (1.0f - dist);
							color.g += LightSpheres[s].Color.g * LightSpheres[s].Color.a * (1.0f - dist);
							color.b += LightSpheres[s].Color.b * LightSpheres[s].Color.a * (1.0f - dist);
						}
					}
					for (int s = 0; s < LightCubes.size(); s++)
					{
						if (Particles[i].position.x < LightCubes[s].position.x + LightCubes[s].scale.x &&
							Particles[i].position.x > LightCubes[s].position.x - LightCubes[s].scale.x &&
							Particles[i].position.y < LightCubes[s].position.y + LightCubes[s].scale.y &&
							Particles[i].position.y > LightCubes[s].position.y - LightCubes[s].scale.y)
						{

							float stage = (abs(Particles[i].position.x - LightCubes[s].position.x) + abs(Particles[i].position.y - LightCubes[s].position.y)) / (LightCubes[s].scale.x * 0.5f + LightCubes[s].scale.y * 0.5f);
							if (stage > 1.0f)
								stage = 1.0f;
							color.r += LightCubes[s].Color.r * LightCubes[s].Color.a * (1.0f - stage);
							color.g += LightCubes[s].Color.g * LightCubes[s].Color.a * (1.0f - stage);
							color.b += LightCubes[s].Color.b * LightCubes[s].Color.a * (1.0f - stage);
						}
					}
				}
				Particles[i].color = color;
				Particles[i].Size = Size;

			/*
				if (Type == "CIRCLE") {

					float aspx = ScreenDivisorX * CameraScale.x;
					float aspy = ScreenDivisorY * CameraScale.y;

					glm::vec2 position = Particles[i].position - CameraPosition;
					position *= glm::vec2(aspx, aspy);


					SceneLayers[SceneLayerIndex].CirclePosScale[start + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
					SceneLayers[SceneLayerIndex].Circlecolors[start + i] = color;
					if (DrawToNormalMap)
					{
						if (material.NormalMap != CubeNormalMapTexture && material.NormalMap != BallNormalMapTexture)
						{
							SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadPosScale[Normastart + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadRotations[Normastart + i] = Particles[i].Rotation;
						}
						else if (material.NormalMap == CubeNormalMapTexture)
						{
							SceneLayers[SceneLayerIndex].NormalMapCubePosScale[Normastart + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMapCubeRotations[Normastart + i] = Particles[i].Rotation;
						}
						else
						{
							SceneLayers[SceneLayerIndex].NormalMapCirclePosScale[Normastart + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMapCircleRotations[Normastart + i] = Particles[i].Rotation;
						}
					}
				}
				else if (Type == "QUAD")
				{
					float aspx = ScreenDivisorX * CameraScale.x;
					float aspy = ScreenDivisorY * CameraScale.y;

					glm::vec2 position = Particles[i].position - CameraPosition;
					position *= glm::vec2(aspx, aspy);



					SceneLayers[SceneLayerIndex].QuadPosScale[start + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
					SceneLayers[SceneLayerIndex].QuadRotations[start + i] = Particles[i].Rotation;
					SceneLayers[SceneLayerIndex].Quadcolors[start + i] = color;
					if (DrawToNormalMap)
					{
						if (material.NormalMap != CubeNormalMapTexture && material.NormalMap != BallNormalMapTexture)
						{
							SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadPosScale[Normastart + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadRotations[Normastart + i] = Particles[i].Rotation;
						}
						else if (material.NormalMap == CubeNormalMapTexture)
						{
							SceneLayers[SceneLayerIndex].NormalMapCubePosScale[Normastart + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMapCubeRotations[Normastart + i] = Particles[i].Rotation;
						}
						else
						{
							SceneLayers[SceneLayerIndex].NormalMapCirclePosScale[Normastart + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMapCircleRotations[Normastart + i] = Particles[i].Rotation;
						}
					}

				}
				else if (Type == "LINE")
				{

					glm::vec2 p1 = Particles[i].position;
					glm::vec2 p2 = Particles[i].position + Particles[i].velocity * Size.y * 0.017f;
					glm::vec2 midpos = (p2 + p1) / 2.0f;
					float rotation = get_angle_between_points(p1, p2);
					glm::vec2 dif = p1 - p2;
					float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;

					float aspx = ScreenDivisorX * CameraScale.x;
					float aspy = ScreenDivisorY * CameraScale.y;

					glm::vec2 position = midpos - CameraPosition;
					position *= glm::vec2(aspx, aspy);


					//DrawCube(midpos, glm::vec2(length * 5.125f * 25, width), glm::vec3(0.0f, 0.0f, rotation), color);

					SceneLayers[SceneLayerIndex].QuadPosScale[start + i] = glm::vec4(position, glm::vec2(Size.x, Size.y * length) * glm::vec2(aspx, aspy));
					SceneLayers[SceneLayerIndex].QuadRotations[start + i] = rotation;
					SceneLayers[SceneLayerIndex].Quadcolors[start + i] = color;
				
					if (DrawToNormalMap)
					{
						if (material.NormalMap != CubeNormalMapTexture && material.NormalMap != BallNormalMapTexture)
						{
							SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadPosScale[Normastart + i] = glm::vec4(position, glm::vec2(Size.x, Size.y * length) * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMaps[NormalMapIndex].QuadRotations[Normastart + i] = rotation;
						}
						else if (material.NormalMap == CubeNormalMapTexture)
						{
							SceneLayers[SceneLayerIndex].NormalMapCubePosScale[Normastart + i] = glm::vec4(position, glm::vec2(Size.x, Size.y * length) * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMapCubeRotations[Normastart + i] = rotation;
						}
						else
						{
							SceneLayers[SceneLayerIndex].NormalMapCirclePosScale[Normastart + i] = glm::vec4(position, glm::vec2(Size.x, Size.y * length) * glm::vec2(aspx, aspy));
							SceneLayers[SceneLayerIndex].NormalMapCircleRotations[Normastart + i] = rotation;
						}
					}
				}
				else if (Type == "TEXTURED" && material.Texture > 0)
				{
					float aspx = ScreenDivisorX * CameraScale.x;
					float aspy = ScreenDivisorY * CameraScale.y;

					glm::vec2 position = Particles[i].position - CameraPosition;
					position *= glm::vec2(aspx, aspy);

					SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors[start + i] = color;
					SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadPosScale[start + i] = glm::vec4(position, Size * glm::vec2(aspx, aspy));
					SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations[start + i] = Particles[i].Rotation;

				}
				else if (Type == "TEXTUREDLINE" && material.Texture > 0)
				{

					glm::vec2 p1 = Particles[i].position;
					glm::vec2 p2 = Particles[i].position + Particles[i].velocity * Size.y * 0.017f;
					glm::vec2 midpos = (p2 + p1) / 2.0f;
					float rotation = get_angle_between_points(p1, p2);
					glm::vec2 dif = p1 - p2;
					float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;


					float aspx = ScreenDivisorX * CameraScale.x;
					float aspy = ScreenDivisorY * CameraScale.y;
					glm::vec2 position = midpos - CameraPosition;
					position *= glm::vec2(aspx, aspy);
					//DrawCube(midpos, glm::vec2(length * 5.125f * 25, width), glm::vec3(0.0f, 0.0f, rotation), color);


					SceneLayers[SceneLayerIndex].TexturedQuads[TQA].Quadcolors[start + i] = color;
					SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadPosScale[start + i] = glm::vec4(position, glm::vec2(Size.x, Size.y * length) * glm::vec2(aspx, aspy));
					SceneLayers[SceneLayerIndex].TexturedQuads[TQA].QuadRotations[start + i] = rotation;


				}
			*/
			
			}
			
		}

}


void ParticleEmiter::Spawn(glm::vec2 position, int amount)
	{
		Particle p;
		for (int i = 0; i < amount; i++)
		{

			p.position = position;
			p.Rotation = InitialRotation;
			p.RotationVelocity = RotationVelocity;
			p.velocity = InitialVelocity;
			p.time = lifetime;
			p.OrbitalVelocity = InitialOrbitalVelocity;

			p.time += (lifetime * (rand() % int(1000) / 1000.0f) - lifetime * 1.0f) * lifetimeRandomness;
			if (int(RotationRandomness) != 0) p.Rotation += rand() % int(RotationRandomness) - RotationRandomness * 0.5f;
			if (int(OrbitalVelocityRandomness * 1000) != 0) p.OrbitalVelocity += rand() % int(OrbitalVelocityRandomness * 1000) / 1000.0f - OrbitalVelocityRandomness * 0.5f;
			if (int(VelocityRandomness.y) != 0)p.velocity.x += rand() %  int(VelocityRandomness.y) - VelocityRandomness.x * 0.5f ;
			if (int(VelocityRandomness.w) != 0)p.velocity.y += rand() %  int(VelocityRandomness.w) - VelocityRandomness.z * 0.5f;
			p.id = rand() % 100000;
			Particles.push_back(p);
		}
	}
void ParticleEmiter::Spawn(glm::vec2 position, glm::vec2 velocity, int amount, float LifeTime)
	{
		for (int i = 0; i < amount; i++)
		{

			Particle p;
			p.position = position;
			p.Rotation = InitialRotation;
			p.RotationVelocity = RotationVelocity;
			p.velocity = velocity;
			if (LifeTime < 0.0f)
				p.time = lifetime;
			else
				p.time = LifeTime;
			p.OrbitalVelocity = InitialOrbitalVelocity;

			p.time += (lifetime * (rand() % int(1000) / 1000.0f) - lifetime * 1.0f) * lifetimeRandomness;
			if (int(RotationRandomness) != 0) p.Rotation += rand() % int(RotationRandomness) - RotationRandomness * 0.5f;
			if (int(OrbitalVelocityRandomness * 1000) != 0) p.OrbitalVelocity += rand() % int(OrbitalVelocityRandomness * 1000) / 1000.0f - OrbitalVelocityRandomness * 0.5f;
			if (int(VelocityRandomness.y) != 0)p.velocity.x += rand() %  int(VelocityRandomness.y) - VelocityRandomness.x * 0.5f ;
			if (int(VelocityRandomness.w) != 0)p.velocity.y += rand() %  int(VelocityRandomness.w) - VelocityRandomness.z * 0.5f;
			p.id = rand() % 100000;
			Particles.push_back(p);
		}
	}
void ParticleEmiter::SpawnInCube(glm::vec2 position, glm::vec2 scale, int amount)
	{
		for (int a = 0; a < amount; a++)
		{
			glm::vec2 pos = position;
			if ((int)scale.x != 0)
				pos.x += rand() % (int)scale.x * 2.0f - scale.x;
			else
				pos.x = scale.x;
			if((int)scale.y!=0)
				pos.y += rand() % (int)scale.y * 2.0f - scale.y;
			else
				pos.y = scale.y;
			Spawn(pos, 1);
		}
	}
void ParticleEmiter::SpawnInCircle(glm::vec2 position, float r, int amount)
	{
		for (int a = 0; a < amount; a++)
		{
			glm::vec2 pos = position;

			float Radius = 1;
			if ((int)r != 0)
				Radius = rand() % (int)r * 0.5f;
			else
				Radius = rand() % 1 * 0.5f;

			float angle = rand();
			pos += glm::vec2((cos(angle) - sin(angle)) * Radius, (cos(angle) + sin(angle)) * Radius);

			Spawn(pos, 1);
		}

	}
void ParticleEmiter::SpawnInCircle(glm::vec2 position, float r, int amount, glm::vec2 velocity)
{
	for (int a = 0; a < amount; a++)
	{
		glm::vec2 pos = position;

		float Radius = 1;
		if ((int)r != 0)
			Radius = rand() % (int)r * 0.5f;
		else
			Radius = rand() % 1 * 0.5f;

		float angle = rand();
		pos += glm::vec2((cos(angle) - sin(angle)) * Radius, (cos(angle) + sin(angle)) * Radius);

		Spawn(pos, velocity, 1);
	}

}
