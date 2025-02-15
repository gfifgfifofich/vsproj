#pragma once
class CentralPart : public BodyComponent
{
public:
	int saveid = -1;
	bool missionPawn = false;

	glm::vec2 midvel = {0.0f,0.0f};
	glm::vec2 avgvel = {0.0f,0.0f};
	float rotationvelocity = 0.0f;
	const char* name = "Entity1";

	float strutMaxLength = 15.0f;
	float strutMinLength = 0.9f;

	float freq = 1.0f;

	struct DataConnection
	{
		int regenp1 = -1;
		int regenp2 = -1;

		int type;// 0=bool 1=float 2=vec2

		int part1 = 0;
		int part2 = 0;

		int index1 = 0;
		int index2 = 0;

		bool failed = false;
		glm::vec2 position1 = {0.0f,0.0f};
		glm::vec2 position2 = {0.0f,0.0f};

		void Process(CentralPart* ent);
		
		void Draw();
		
	};

	struct Connection
	{
	private:
		int frame = 0;
	public:
		int regenp1 = -1;
		int regenp2 = -1;
		int part1 = 0;
		int part2 = 0;

		int index1 = 0;
		int index2 = 0;

		float width = 0.125f;
		float length = 1.0f;
		float stiffnes = 1.0f;
		float absorbtion = 2.0f;

		float HeatTransferSpeed = 1.0f;

		int type = CONNECTION::STRUT;

		int b1 = -1;
		int b2 = -1;

		float accumDT = 0.0f;

		BodyComponent* bod1;
		BodyComponent* bod2;

		DecorativeRope rope;

		unsigned int Texture = 0;
		unsigned int NormalMap = 0;

		void UpdateLinks(CentralPart* ent);
		void Init();
		void Process(CentralPart* ent, float dt);
		void Draw();

	};


	struct RegenPart
	{
		/*
			Regen if part with id of PartConnections exists, and attempt to create all connections.
		*/
		std::vector<Connection> Connections;
		std::vector<DataConnection> DataConnections;

		std::vector<int> PartConnections;
		std::vector<std::vector<glm::vec2>> Positions;

		int Type = -1;
	};
	std::vector<RegenPart> RegenParts;

	float regendt = 0.5f;
	float regenaftertime = 0.5f;
	float regenTimer = 10.0f;

	
	int id = -1;
	bool Alive = true;
	bool destroyed = false;

	int back = -1;
	int front = -1;
	glm::vec2 prevfrontpos = {0.0f,0.0f};
	glm::vec2 prevbackpos = {0.0f,0.0f};

	glm::vec2 direction = glm::vec2(0.0f, 1.0f);

	float RotationalFriction = 1.0f;

	float Rotation = 0.0f;
	float RotationVelocity = 0.0f;

	float AquieredRotationVelocity = 0.0f;

	glm::vec2 LookAt = glm::vec2(1.0f, -1.0f);
	glm::vec2 ThrustDirection = glm::vec2(0.0f, 0.0f);


	glm::vec2 CenterOfMass = glm::vec2(0.0f);

	float avgheat = 0.0f;
	float sumheat = 0.0f;

	float sumHealth = 0.0f;
	float prevsumHealth = 0.0f;

	std::vector<int> Balls;
	std::vector<BodyComponent*> Parts;
	std::vector<BodyComponent*> Engines;

	float mass = 0.0f;
	float ForceToThrustDirection = 0.0f;
	float maxVelocity = 0.0f;
	glm::vec2 trgPos = { 0.0f,0.0f };
	glm::vec2 Playerpos = { 0.0f,0.0f };
	// 0 stay, 1 dirrect attack
	int AIState = 0;
	bool autocontrol = false;

	std::vector<Connection> Connections;
	std::vector<DataConnection> DataConnections;
	glm::vec2 LookAtOnTarget = glm::vec2(0.0f, 1.0f);


	glm::vec2 GunsTargetrotPoint = glm::vec2(0.0f);
	bool FireGuns = false;

	bool firstdrawafterload = true;

	std::vector<DamageSphere*> CloseDamageSpheres;

	float maxR = 0.0f;
	CentralPart();
	void Create(glm::vec2 position, glm::vec2 direction, float size,float mass = 1.0f) override;
	void ProcessConnections();
	void ProcessRegen(float dt);
	void MTProcess (float dt) override;
	void Process(float dt) override;
	void Draw() override;
	
	
	void DeletePart() override;
	void DeleteConnections( int index, int index2);
	void DeleteSomeConnections( int Partindex, float chance);
	void DestroyPart(int  index);
	void DetachPart( int  index);
	void DetachDetachedParts();
	bool CheckPartConnections(Connection* c);
	void CheckPartsConnections();
	int ClaimPart(int index);
	void AddConnection(int type, float len, float width, float stiffness, float absorbtion, float HeatTransferSpeed,
		int part1, int  index1,
		int part2, int  index2, bool onlyCreate = false);
	void AddDataConnection(int type, int part1, int  index1, int part2, int  index2, bool onlyCreate = false);
	void SaveTo(std::string filename, bool AllSave = false,bool fullpath = false);
	void LoadFrom(std::string filename, bool AllSave = false, bool fullpath = false);
	void Clear();
	void Destroy(bool silent = false);
};
inline std::vector <CentralPart*> Entities;
