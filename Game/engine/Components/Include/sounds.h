#pragma once
// openAL
#include <AL/alext.h>
#include <map>
inline std::vector <ALuint*> sources;
inline std::vector <ALuint*> sounds;
inline std::vector <unsigned int> soundsArray;
inline std::vector <unsigned int*> soundsArray2;
inline ALCdevice* Device = alcOpenDevice(NULL);
inline ALCcontext* Context = NULL;
inline ALCboolean contextMadeCurrent;
inline glm::vec3 listenerPos = { 0.0,0.0,0.0 };
inline glm::vec3 listenerVel = { 0.0,0.0,0.0 };
inline ALfloat listenerOri[] = { 0.0,0.0,1.0, 0.0,-1.0,0.0 };
inline glm::vec3 soundscale = { 1.0f,1.0f ,1.0f };
inline glm::vec3 soundSpeedscale = { 1.0f,1.0f ,1.0f };
inline glm::vec2 ListenerPos;// for soundPools


unsigned int LoadSound(const char* FileName);
void DeleteSound(unsigned int* sound);

void AL_init();
void AL_Reload();
void  AL_Destroy();
void UpdateListenerPosition();
void GenSource(unsigned int* source);
void SetSourcePitch(unsigned int* source, float pitch);
void SetSourceGain(unsigned int* source, float gain);
void SetSourceRefDist(unsigned int* source, float Distance);
void SetSourceMaxDistance(unsigned int* source, float Distance);
void SetSourceRollOff(unsigned int* source, float rolloff);
void SetSourceRelative(unsigned int* source, bool relative);

void SetSourcePosition(unsigned int* source, glm::vec3 position);
void SetSourcePosition(unsigned int* source, glm::vec2 position);

void SetSourceVelocity(unsigned int* source, glm::vec3 velocity);
void SetSourceLooping(unsigned int* source, bool looping);

void SetSourceSound(unsigned int* source, unsigned int* sound);

// helpfull when source is running on loop
void SwapSourceSound(unsigned int* source, unsigned int* sound);
void PlaySource(unsigned int* source);
void StopSource(unsigned int* source);
bool SourcePlaying(unsigned int* source);
void DeleteSource(unsigned int* source);
void PlaySound(unsigned int* sound, glm::vec2 position, float pitch = 1.0f, float gain = 1.0f);
void PlaySound(unsigned int* dst, unsigned int* sound, glm::vec2 position, float pitch = 1.0f, float gain = 1.0f);
void PlaySound(unsigned int** dst, unsigned int* sound, glm::vec2 position, float pitch = 1.0f, float gain = 1.0f);
void ProcessAL();




struct soundpool
{
	unsigned int sound =0;
	unsigned int sourceamount = 0;
	std::vector<unsigned int> ssources;
	std::vector<int> sourceUsageAmount;
	std::vector<glm::vec2> avgpositions;
	std::vector<glm::vec2> avgvelocities;
	std::vector<float> avggains;
	std::vector<float> avgpitches;
	bool continuous = false;
	int LastSourceAmout = 0;

	void Update();
	void AddSound(glm::vec2 position,float gain,float pitch,glm::vec2 velocity,bool startover);
	void Clear();
	void Delete();
};
inline std::vector<soundpool> Sounds;


void addsound(unsigned int sound, bool looping = false, unsigned int sourceamount = 10);

void playsound(unsigned int sound, glm::vec2 position,float gain = 1.0f,float pitch = 1.0f,glm::vec2 velocity = {0.0f,0.0f},bool startover = false);