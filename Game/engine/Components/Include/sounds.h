#pragma once
// fmod
#include <map>
#include "fmod.hpp"
inline std::vector <int*> sources;
inline std::vector <int*> sounds;
inline int channelcount;

inline FMOD::System* _sound_system = nullptr;
inline FMOD_RESULT _sound_result;

inline glm::vec3 listenerPos = { 0.0,0.0,0.0 };
inline glm::vec3 listenerVel = { 0.0,0.0,0.0 };
inline float listenerOri[] = { 0.0,0.0,1.0, 0.0,-1.0,0.0 };
inline glm::vec3 soundscale = { 1.0f,1.0f ,1.0f };
inline glm::vec3 soundSpeedscale = { 1.0f,1.0f ,1.0f };
inline glm::vec2 ListenerPos;// for soundPools



FMOD::Sound* LoadSound(const char* FileName, bool sound_3d = true);
void DeleteSound(FMOD::Sound* sound);
void FMOD_init(int maxsources = 2048, float distancefactor = 1.0f);
void FMOD_Destroy();
void UpdateListenerPosition();
FMOD::Channel* GenSource(FMOD::Sound* sound, int priority = 0);
void SetSourcePitch(FMOD::Channel* source, float pitch);
void SetSourceLooping(FMOD::Channel* source, bool looping);
void SetSourceGain(FMOD::Channel* source, float gain);
void SetSource3D(FMOD::Channel* source, glm::vec3 position, glm::vec3 velocity);
void SetSource3D(FMOD::Channel* source, glm::vec2 position, glm::vec2 velocity);
void PlaySource(FMOD::Channel* source, bool reset = false);
void StopSource(FMOD::Channel* source);
bool SourcePlaying(FMOD::Channel* source);
void DeleteSource(FMOD::Channel* source);

void PlaySound(FMOD::Sound* sound, glm::vec2 position, float gain = 1.0f, float pitch= 1.0f);


struct soundpool
{
	FMOD::Sound* sound = 0;
	unsigned int sourceamount = 0;
	std::vector<FMOD::Channel*> sourcesBuff;
	std::vector<unsigned int> sourcePlayPositions;
	std::vector<int> sourceUsageAmount;
	std::vector<glm::vec2> avgpositions;
	std::vector<glm::vec2> avgvelocities;
	std::vector<float> avggains;
	std::vector<float> avgpitches;
	std::vector<bool> startover;
	bool continuous = false;
	int LastSourceAmout = 0;

	void Update();
	void AddSound(glm::vec2 position, float gain, float pitch, glm::vec2 velocity, bool startover);
	void Delete();
};
inline std::vector<soundpool> Sounds;


void addsound(FMOD::Sound* sound, bool looping = false, unsigned int sourceamount = 10);

void playsound(FMOD::Sound* sound, glm::vec2 position, float gain = 1.0f, float pitch = 1.0f, glm::vec2 velocity = { 0.0f,0.0f }, bool startover = false);