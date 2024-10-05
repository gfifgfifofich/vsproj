#ifndef PARTICLESH
#define PARTICLESH

#include <thread>
#include <algorithm>
#include <execution>

// Contains and Process data for particles. Pushback ptr to ParticleEmiters array, or Process() somewhere else 
inline bool ParticleMultithreading;


class ParticleEmiter
{
private:



public:
	void _Process(int thr);



	struct Particle
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(0.0f);
		glm::vec4 color = glm::vec4(0.0f);
		float Rotation = 0.0f;
		float RotationVelocity = 0.0f;
		float time = 0.0f;
		float OrbitalVelocity = 0.0f;
		int id = 0;
	};

	struct influenceSphere
	{
		glm::vec2 position;
		float r;

		glm::vec2 velocity;
		bool attractive = false;
		float attractionStrength = 1.0f;
	};
	struct influenceCube
	{
		glm::vec2 position;
		glm::vec2 scale;

		glm::vec2 velocity;
		bool attractive = false;
		float attractionStrength = 1.0f;
	};
	struct EmitionPoint
	{
		glm::vec2 position = glm::vec2(0.0f);

		glm::vec2 velocity = glm::vec2(0.0f);
		int amount = 1;
		float tick = 0.017f;
		float t = 0.0f;
	};
	struct EmitionCircle
	{
		glm::vec2 position = glm::vec2(0.0f);
		float r;

		glm::vec2 velocity = glm::vec2(0.0f);
		int amount = 1;
		float tick = 0.017f;
		float t = 0.0f;
	};
	struct EmitionCube
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 scale = glm::vec2(0.0f);

		glm::vec2 velocity = glm::vec2(0.0f);
		int amount = 1;
		float tick = 0.017f;
		float t = 0.0f;
	};
	struct LightSphere
	{
		glm::vec2 position = glm::vec2(0.0f);
		float r = 300.0f;

		glm::vec4 Color = glm::vec4(1.0f);
	};
	struct LightCube
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 scale = glm::vec2(0.0f);

		glm::vec4 Color = glm::vec4(1.0f);
	};



	glm::vec2 acceleration = glm::vec2(0.0f);
	glm::vec2 InitialVelocity = glm::vec2(0.0f);
	glm::vec4 VelocityRandomness = glm::vec4(0.0f);// (-x,x,-y,y), rand from -x to x, from -y to y
	float VelocityDamper =0.0f;//"friction"

	glm::vec2 StartSize = glm::vec2(1.0f);
	glm::vec2 EndSize = glm::vec2(1.0f);

	glm::vec4 StartColor = glm::vec4(1.0f);
	glm::vec4 EndColor = glm::vec4(1.0f);
	
	// all rorations are only for QUAD's
	float InitialRotation = 0.0f;
	float RotationRandomness = 0.0f;
	float RotationVelocity = 0.0f;
	float RotationAcceleration = 0.0f;
	float RotationDamper = 0.0f;//"friction"
	
	float InitialOrbitalVelocity = 0.0f;
	float OrbitalVelocityRandomness = 0.0f;

	float lifetime = 1.0f;
	float lifetimeRandomness = 0.0f;
	std::vector <Particle> Particles;

	std::string Type = "QUAD";// QUAD, LINE, CIRCLE, TEXTURED ( for QUAD & TEXTURED - Size(width,height), Line - Size(length,width), Circle - Size(r,nothing))

	int Textureid = -1;
	int NormalMapid = -1;
	Material material;
	
	
	int TQA = -1;
	
	bool influenced = false;
	std::vector<influenceSphere> CaptureSpheres; // different function for attraction, from SpheresOfInfluence

	std::vector<influenceSphere> SpheresOfInfluence;
	std::vector<influenceCube> CubesOfInfluence;

	std::vector<EmitionPoint> EmitionPoints;
	std::vector<EmitionCircle> EmitionCircles;
	std::vector<EmitionCube> EmitionCubes;

	bool lighted = false;
	std::vector<LightSphere> LightSpheres;
	std::vector<LightCube> LightCubes;

	std::string Name = "Particles yay";
	int id = 0;


	int Z_Index = 0;
	bool Additive = false;
	bool DrawToNormalMap = false;

	bool ShowWindow = false;

	int start = 0;
	int Normastart = 0;
	int threadcount = std::thread::hardware_concurrency();
	float Partdelta = 0.017f;

	int SceneLayerIndex = -1;
	int NormalMapIndex = -1;

	int AddSpheresOfInfluence(glm::vec2 position, float r, glm::vec2 velocity, bool Attractive = false, float AttractionStrength = 1.0f);
	int AddCubeOfInfluence(glm::vec2 position, glm::vec2 scale, glm::vec2 velocity, bool Attractive = false, float AttractionStrength = 1.0f);
	int AddCubeEmiter(glm::vec2 position, glm::vec2 scale, glm::vec2 velocity, float amount, float tick);
	int AddCircleEmiter(glm::vec2 position, glm::vec2 velocity, float r, float amount, float tick);
	int AddPointEmiter(glm::vec2 position, glm::vec2 velocity, float amount, float tick);
	int AddLightSphere(glm::vec2 position, float r, glm::vec4 Color = glm::vec4(1.0f));
	int AddLightCube(glm::vec2 position, glm::vec2 scale, glm::vec4 Color = glm::vec4(1.0f));

	void Process(float dt = 0.017f);

	void Spawn(glm::vec2 position, int amount = 1);
	void Spawn(glm::vec2 position, glm::vec2 velocity, int amount = 1, float LifeTime = -1.0f);
	void SpawnInCube(glm::vec2 position, glm::vec2 scale, int amount);
	void SpawnInCircle(glm::vec2 position, float r, int amount);
	void SpawnInCircle(glm::vec2 position, float r, int amount, glm::vec2 velocity);
};



#endif //PARTICLESH