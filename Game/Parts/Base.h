#pragma once
struct CostMaterial
{
	int Matter = 0;

	////Raw
	//int Fe = 0;
	//int C = 0;
	//int Al = 0;
	//int Cu = 0;
	//int Au = 0;
	//int U = 0;
	//int Ti = 0;

	////Processed
	//int Steel = 0;
};



enum CONNECTION
{
	STRUT = 0,
	SPRING = 1,
	HEATPIPE = 2,
	ROPE = 3
};
enum TYPE
{
	STRUCTUREPART = 0,
	WEAPON = 1,
	PROPULSION = 2
};
enum PART
{
	BALLBODY = 0,
	ROCKETENGINE =1,
	GUN =2,
	LASERGUN =3,
	RADIATOR =4,
	ROTOR =5,
	ROCKETLAUNCHER =6,
	MINIGUN =7,
	RAILGUN =8,
	STATICPOINT =9,
	CENTRALPART =10,
	AIAIM =11,
	GLIDER =12,
	LASTPART
};

#define SpawnablePartAmount 9-1
inline DataStorage PartsData;
inline bool drawHeat;
inline bool drawHealth;
class BodyComponent : public Node
{
public:
	struct BoolDataPoint
	{
		bool source;
		std::string name;
		bool data = false;
		glm::vec2 lastpos = glm::vec2(0.0f);
		bool connected;
	};
	struct FloatDataPoint
	{
		bool source = true;
		std::string name;
		float data = 0.0f;
		glm::vec2 lastpos = glm::vec2(0.0f);
		bool connected;
	};
	struct VectorDataPoint
	{
		bool source = true;
		std::string name;
		glm::vec2 data = { 0.0f ,0.0f};
		glm::vec2 lastpos = glm::vec2(0.0f);
		bool connected;
	};

	std::vector<BoolDataPoint> bDataConnections;
	std::vector<FloatDataPoint> fDataConnections;
	std::vector<VectorDataPoint> vDataConnections;

	int bDCsize=0;
	int fDCsize=0;
	int vDCsize=0;

	std::string name = "NO NAME";

	std::vector<int> body;
	std::vector<int> BodyIdsWithCollision;
	int bodysize = 1;
	float freq = 1.0f;
	int parttype = TYPE::STRUCTUREPART;
	int partid = -1;
	int id = 0;

	int questid = -1;

	bool player=false;

	bool dead = false;
	bool debris = true;
	bool deactivated = false;

	bool attached = false;



	float Health = 1.0f;
	float maxHealth = 1.0f;

	

	float throtle = 0.0f;

	bool shot = false;
	bool shutdown = false;
	float shutdownTemp = 10.0f;
	float MaxTemp = 30.0f;
	float MinAutocooltemp = 0.0f;
	float MinTemp = -30.0f;

	float CoolingSpeed = 0.5f;

	float Power = 350.0f;

	bool WasHitThisFrame = false;
	float dmgThisFrame = 0;

	glm::vec4 BaseColor = glm::vec4(1.0f);
	glm::vec4 HeatColor = glm::vec4(60.0f,10.0f,2.0f,0.0f);
	glm::vec4 ColdColor = glm::vec4(0.04f,2.0f,10.0f,0.0f);
	glm::vec4 color = glm::vec4(1.0f);

	glm::vec2 targetrotpoint = {0.0f,0.0f};
	glm::vec2 dir = {0.0f,1.0f};
	glm::vec2 mid = {0.0f,0.0f};


	CostMaterial Cost;
	CostMaterial Contains;
	void OnPartCreate();
	
	void ProcessBody(float dt);
	
	virtual void Create(glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f);
	virtual void DeletePart();
	
	virtual ~BodyComponent() override;
	
	virtual	void DrawDataConnections(int partid, bool b, bool f, bool v,int Z_Index = 0);
	
	void DamageSphereCollide(DamageSphere* DS);
	void CreateBody(int size, int boolDCsize = 0,int floatDCsize=0, int vectorDCsize = 0);
	void DeleteBody();
	
};

inline std::vector<BodyComponent*> PartInstances;
inline float DrawingDistance;
class PartsPile
{
public:

	std::vector<BodyComponent*> Parts;

	std::vector<DamageSphere*> CloseDamageSpheres;

	void Process(float dt, int iter,bool lastiter);
	void DeletePart(int  index);

	void SpawnPart(int type, glm::vec2 position, float size = 20.0f);
	void SpawnRandomPart( glm::vec2 position, float size = 20.0f);

};
inline PartsPile Debris;



inline int GrabbedBall;
inline int SelectedPart;

inline std::vector<int> PurchasableParts;
void InitParts();
BodyComponent* CreatePart(int type, glm::vec2 position, glm::vec2 direction, float size, float mass = 1.0f);