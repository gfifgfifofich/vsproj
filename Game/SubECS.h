#pragma once

class DestructableStaticBall : public CO_Ball
{
public:	

	DestructableStaticBall();
	virtual ~DestructableStaticBall() override;
	float health = 10.0f;

	int DestrParticleAmount = 10;
	float DestrParticleSpeed = 10;
	bool SpawnInShapeOrPoint = true;
	bool ExplodeOnOverheat = false;
	bool ExplodeOnDeath = false;
	bool DieOnOverheat = false;
	bool Invincibility = false;
	float temperature = 0.0f;
	float maxTemperature = 10.0f;
	float trgTemperature = 0.0f;
	float passiveCooling = 1.0f;//per sec

	float explodionR = 10.0f;
	float explodionDamage = 2.5f;
	float explodionLifeTime = 1.0f;
	float explodionRecoil = 40.0f;
	
	glm::vec4 HeatColor = glm::vec4(60.0f,10.0f,2.0f,0.0f);
	glm::vec4 ColdColor = glm::vec4(0.04f,2.0f,10.0f,0.0f);
	glm::vec4 color = glm::vec4(1.0f);

	ParticleAsset* partAsset = NULL;
	std::string partAssetName = "None";

	UI_DataPack GetUIDataDestructableStaticBall();
	virtual std::vector<UI_DataPack> GetUIData() override;
	void MTProcess(float dt) override;

};

class DestructableStaticCube : public CO_Cube
{
public:	

	DestructableStaticCube();
	virtual ~DestructableStaticCube() override;
	float health = 10.0f;

	int DestrParticleAmount = 10;
	float DestrParticleSpeed = 10;
	bool SpawnInShapeOrPoint = true;
	bool ExplodeOnOverheat = false;
	bool ExplodeOnDeath = false;
	bool DieOnOverheat = false;
	bool Invincibility = false;
	float temperature = 0.0f;
	float maxTemperature = 10.0f;
	float trgTemperature = 0.0f;
	float passiveCooling = 1.0f;//per sec

	float explodionR = 10.0f;
	float explodionDamage = 2.5f;
	float explodionLifeTime = 1.0f;
	float explodionRecoil = 40.0f;
	
	glm::vec4 HeatColor = glm::vec4(60.0f,10.0f,2.0f,0.0f);
	glm::vec4 ColdColor = glm::vec4(0.04f,2.0f,10.0f,0.0f);
	glm::vec4 color = glm::vec4(1.0f);

	ParticleAsset* partAsset = NULL;
	std::string partAssetName = "None";
	UI_DataPack GetUIDataDestructableStaticCube();
	virtual std::vector<UI_DataPack> GetUIData() override;
	void MTProcess(float dt) override;
	
};

inline std::vector<DestructableStaticBall*> DestructableStaticBalls;
inline std::vector<DestructableStaticCube*> DestructableStaticCubes;

class StaticDamageSphere : public Node
{
public:	
	DamageSphere ds;
	StaticDamageSphere();
	virtual ~StaticDamageSphere() override;
	
	bool disable = false;
	bool attractive = false;
	float attractionForce = 0.0f;
	float Friction = 1.0f;
	float damage = 0.0f;
	float heat = 0.0f;
	float radius = 1.0f;

	UI_DataPack GetUIDataStaticDamageSphere();
	virtual std::vector<UI_DataPack> GetUIData() override;
	void PreProcess() override;
	
	virtual void DebugDraw()override;
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)override;
	virtual bool SelectionCheck(glm::vec2 point)override;
};

class TriggerCube : public Node
{
public:	
	TriggerCube();
	~TriggerCube() override;
	
	bool determineInside = false;
	bool disable = false;
	bool triggered = false;
	glm::vec2 Scale = {1.0f,1.0f};

	UI_DataPack GetUIDataTriggerCube();
	virtual std::vector<UI_DataPack> GetUIData() override;
	void MTPreProcess() override;
	
	virtual void DebugDraw()override;
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)override;
	virtual bool SelectionCheck(glm::vec2 point)override;
};

inline std::vector<TriggerCube*> TriggerCubes;

class Monitor : public Object
{
public:	
	Monitor();
	
	//blink
	float timer = 0.0f;
	bool flip = false;

	float textScale = 0.16f;
	float textStep = -8.0f;
	glm::vec2 textOffset = {4.0f,-10.0f};
	bool bleepLastLine = false;
	bool disable = false;

	// cant be bothered with storing arrays
	std::string text1 = "";
	std::string text2 = "";
	std::string text3 = "";
	std::string text4 = "";
	std::string text5 = "";
	std::string text6 = "";
	std::string text7 = "";
	std::string text8 = "";
	std::string text9 = "";
	std::string text10 = "";

	glm::vec4 color1  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color2  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color3  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color4  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color5  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color6  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color7  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color8  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color9  = {1.0f,1.0f,1.0f,1.0f};
	glm::vec4 color10 = {1.0f,1.0f,1.0f,1.0f};


	UI_DataPack GetUIDataMonitor();
	virtual std::vector<UI_DataPack> GetUIData() override;
  
	virtual void Draw()override;
};
