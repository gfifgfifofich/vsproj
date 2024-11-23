#pragma once
// openAL
#include "../Include/Helper.h"
#include "../Include/sounds.h"


#include <Objbase.h>
void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line)
{
	
}
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

FMOD::Sound* LoadSound(const char* FileName, bool sound_3d)
{
	FMOD::Sound* sound = nullptr;
	_sound_result = _sound_system->createSound(FileName, sound_3d ? FMOD_3D : FMOD_2D, 0, &sound);
	ERRCHECK(_sound_result);
	
	return sound;
}
void DeleteSound(FMOD::Sound* sound)
{
	_sound_result = sound->release();
	ERRCHECK(_sound_result);
}

void Common_Init(void** /*extraDriverData*/)
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

void FMOD_init(int maxsources, float distancefactor)
{
	void* extradriverdata = 0;
	Common_Init(&extradriverdata);
	channelcount = 0;
	_sound_result = FMOD::System_Create(&_sound_system);
	ERRCHECK(_sound_result);
	
	_sound_result = _sound_system->init(maxsources, FMOD_INIT_MIX_FROM_UPDATE, extradriverdata);
	ERRCHECK(_sound_result);

	_sound_result = _sound_system->set3DSettings(1.0, distancefactor, 1.0f);
	ERRCHECK(_sound_result);


	std::cout << "\n\n\n inited sound \n\n\n";
}


void  FMOD_Destroy()
{
	_sound_result = _sound_system->close();
	ERRCHECK(_sound_result);
	_sound_result = _sound_system->release();
	ERRCHECK(_sound_result);
}

void UpdateListenerPosition()
{

	listenerPos.x = listenerPos.x / WIDTH * soundscale.x;
	listenerPos.y = listenerPos.y / WIDTH * soundscale.y;
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR listenpos = { listenerPos.x, listenerPos.y,listenerPos.z };
	FMOD_VECTOR listenvel = { listenerVel.x, listenerVel.y,listenerVel.z };


	_sound_result = _sound_system->set3DListenerAttributes(0, &listenpos, &listenvel, &forward, &up);
	ERRCHECK(_sound_result);

	_sound_result = _sound_system->update();
	ERRCHECK(_sound_result);

	_sound_system->getChannelsPlaying(&channelcount);
}

FMOD::Channel* GenSource(FMOD::Sound* sound,int priority)
{
	FMOD::Channel* channel = nullptr;
	_sound_result = _sound_system->playSound(sound, 0, true, &channel);
	ERRCHECK(_sound_result);
	_sound_result = channel->setPriority(priority);
	ERRCHECK(_sound_result);
	return channel;
}
void SetSourcePitch(FMOD::Channel* source, float pitch)// stock = 1.0f;
{
	_sound_result = source->setFrequency(48000.0f * pitch);
	ERRCHECK(_sound_result);


}
void SetSourceLooping(FMOD::Channel* source, bool looping)// stock = 1.0f;
{
	_sound_result = source->setMode(looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	ERRCHECK(_sound_result);
}
void SetSourceGain(FMOD::Channel* source, float gain)// stock = 1.0f;
{
	_sound_result = source->setVolume(gain*0.8f);
	ERRCHECK(_sound_result);
}
void SetSource3D(FMOD::Channel* source, glm::vec3 position, glm::vec3 velocity)// stock = 0.0f,0.0f,0.0f;
{
	glm::vec3 gpos;
	gpos.x = position.x / WIDTH * soundscale.x;
	gpos.y = position.y / WIDTH * soundscale.y;
	gpos.z = position.z * soundscale.z;
	glm::vec3 gvel;
	gvel.x = velocity.x / WIDTH * soundscale.x;
	gvel.y = velocity.y / WIDTH * soundscale.y;
	gvel.z = velocity.z * soundscale.z;
	FMOD_VECTOR pos = { gpos.x,gpos.y,gpos.z },
		vel = { gvel.x,gvel.y,gvel. z};

	_sound_result = source->set3DAttributes(&pos,&vel);
	if (_sound_result != FMOD_ERR_NEEDS3D)
		ERRCHECK(_sound_result);
}
void SetSource3D(FMOD::Channel* source, glm::vec2 position, glm::vec2 velocity)// stock = 0.0f,0.0f,0.0f;
{
	glm::vec3 gpos;
	gpos.x = position.x / WIDTH * soundscale.x;
	gpos.y = position.y / WIDTH * soundscale.y;
	gpos.z = 0;
	glm::vec3 gvel;
	gvel.x = velocity.x / WIDTH * soundscale.x;
	gvel.y = velocity.y / WIDTH * soundscale.y;
	gvel.z = 0;
	FMOD_VECTOR pos = { gpos.x,gpos.y,gpos.z },
		vel = { gvel.x,gvel.y,gvel.z };
	
	_sound_result = source->set3DAttributes(&pos, &vel);
	if(_sound_result != FMOD_ERR_NEEDS3D)
		ERRCHECK(_sound_result);
}

void PlaySource(FMOD::Channel* source, bool reset)
{
	if (!SourcePlaying(source))
	{
		_sound_result = source->setPaused(false);
		ERRCHECK(_sound_result);
	}
	if (reset)
	{
		_sound_result = source->setPosition(0.0f, FMOD_TIMEUNIT_MS);
		ERRCHECK(_sound_result);
	}
}

void StopSource(FMOD::Channel* source)
{
	SetSourceGain(source, 0.0f);
	_sound_result = source->setPaused(true);
	ERRCHECK(_sound_result);
}

bool SourcePlaying(FMOD::Channel* source)
{
	bool playing = false;
	_sound_result = source->getPaused(&playing);
	ERRCHECK(_sound_result);
	return !playing;
}

void DeleteSource(FMOD::Channel* source)
{
	SetSourceGain(source, 0.0f);
	source->stop();

}
#undef PlaySound




void soundpool::Update()
{

	for (int i = 0; i < sourcePlayPositions.size(); i++)
	{

		if (avggains[i] > 0.0f && sourceUsageAmount[i] > 0)
		{


			avgpositions[i] /= (float)sourceUsageAmount[i];
			avgvelocities[i] /= (float)sourceUsageAmount[i];
			avgpitches[i] /= (float)sourceUsageAmount[i];
			
			if (sourcesBuff[i] != nullptr)
			{
				SetSourceGain(sourcesBuff[i], avggains[i]);
				SetSource3D(sourcesBuff[i], avgpositions[i], avgvelocities[i]);
				SetSourcePitch(sourcesBuff[i], avgpitches[i]);
				SetSourceLooping(sourcesBuff[i], continuous);
				if (continuous)
				{
					if (!SourcePlaying(sourcesBuff[i]))
						PlaySource(sourcesBuff[i]);
				}
				else
				{
					PlaySource(sourcesBuff[i],true);
				}
				_sound_result = sourcesBuff[i]->getPosition(&sourcePlayPositions[i], FMOD_TIMEUNIT_MS);
				ERRCHECK(_sound_result);
			}
			else
			{
				sourcesBuff[i] = GenSource(sound);
				SetSourceGain(sourcesBuff[i], avggains[i]);
				SetSource3D(sourcesBuff[i], avgpositions[i], avgvelocities[i]);
				SetSourcePitch(sourcesBuff[i], avgpitches[i]);
				SetSourceLooping(sourcesBuff[i], continuous);

				if (continuous)
				{
					_sound_result = sourcesBuff[i]->setPosition(sourcePlayPositions[i], FMOD_TIMEUNIT_MS);
					ERRCHECK(_sound_result);
				}
				PlaySource(sourcesBuff[i]);

			}

		}
		else if (avggains[i] < 0.001f && sourcesBuff[i] != nullptr && continuous)
		{
			DeleteSource(sourcesBuff[i]);
			sourcesBuff[i] = nullptr;
		}

		avgvelocities[i] = glm::vec2(0.0f);
		avggains[i] = 0.0f;
		avgpitches[i] = 0.0f;
		avgpositions[i] = ListenerPos;
		sourceUsageAmount[i] = 0;

	}
}
void soundpool::AddSound(glm::vec2 position, float gain, float pitch, glm::vec2 velocity, bool startover)
{
	if (sqrlength(ListenerPos - position) > (3000) * (3000))
		return;
	float ll = -1;
	int id = -1;
	bool first = true;
	for (int i = 0; i < avgpositions.size(); i++)
	{
		float score = 0.0f;
		glm::vec2 spos = avgpositions[i];
		if (sourceUsageAmount[i] > 1)
			spos = spos / (float)sourceUsageAmount[i];
		score += sqrlength(spos - position);
		score -= sourceUsageAmount[i] * 1000.0f;
		//if(sourceUsageAmount[i]==0)
		//{
		//	id = i;
		//	return;
		//}
		if (score < ll || first)
		{
			ll = score;
			id = i;
			first = false;
		}
	}
	if (id < 0)
	{
		return;
	}
	sourceUsageAmount[id]++;
	if (sourceUsageAmount[id] == 1)
		avgpositions[id] = position;
	else
		avgpositions[id] += position;

	avgvelocities[id] += velocity;
	avggains[id] += gain;
	avgpitches[id] += pitch;
	if (startover || !continuous)
	{
		sourcePlayPositions[id] = 0.0f;

		if (sourcesBuff[id] == nullptr)
			return;
		if (SourcePlaying(sourcesBuff[id]))
		{
			DeleteSource(sourcesBuff[id]);
			sourcesBuff[id] = nullptr;
		}
	}

}
void soundpool::Delete()
{
	for (int i = 0; i < sourcesBuff.size(); i++)
	{
		if (sourcesBuff[i] == nullptr)
			continue;
		DeleteSource(sourcesBuff[i]);
		sourcesBuff[i] = nullptr;
	}
}


void addsound(FMOD::Sound* sound, bool looping, unsigned int sourceamount)
{
	soundpool ss;
	ss.sound = sound;
	ss.sourceamount = sourceamount;
	ss.continuous = looping;
	ss.sourcesBuff.resize(sourceamount);
	ss.avgpositions.resize(sourceamount);
	ss.avgvelocities.resize(sourceamount);
	ss.avggains.resize(sourceamount);
	ss.avgpitches.resize(sourceamount);
	ss.sourceUsageAmount.resize(sourceamount);
	ss.sourcePlayPositions.resize(sourceamount);
	ss.startover.resize(sourceamount);
	
	int divider = sqrt(sourceamount);
	for (int i = 0; i < sourceamount; i++)
	{
		ss.sourcesBuff[i] = nullptr;
		ss.startover[i] = false;
		int y = i / divider;

		ss.avgpositions[i] = glm::vec2(((i % divider) - divider / 2), y - divider / 2);
		ss.avgpositions[i].x = ss.avgpositions[i].x;
		ss.avgpositions[i].y = ss.avgpositions[i].y;
		ss.avgvelocities[i] = glm::vec2(0.0f);
		ss.avggains[i] = 0.0f;
		ss.avgpitches[i] = 0.0f;
		ss.sourceUsageAmount[i] = 0;
	}
	Sounds.push_back(ss);
}
void playsound(FMOD::Sound* sound, glm::vec2 position, float gain, float pitch, glm::vec2 velocity, bool startover)
{
	for (int i = 0; i < Sounds.size(); i++)
		if (Sounds[i].sound == sound) Sounds[i].AddSound(position, gain, pitch, velocity, startover);
}