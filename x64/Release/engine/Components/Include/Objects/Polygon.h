#pragma once
#include "Line.h"
#include "../Collisions/BallToLine.h"

class SubTriangle
{
public:
	line l1 = line(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	line l2 = line(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	line l3 = line(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
};
class Triangle
{
public:
	// Level - on which level it is, Mask - with which level it collides
	int Collision_Level = 0; // -1 - do not collide, >=0 - collision levels
	int Collision_Mask = 0; // -1 - do not collide, >=0 - collision levels


	std::vector <SubTriangle> SubTriangles;
	float roughness = 1.0f;

	line lines[3] = { lines[0] = line(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
					  lines[1] = line(glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
					  lines[2] = line(glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f)) };



	glm::vec2 points[3];
	glm::vec2 midlePosition = glm::vec2(0.0f);
	void Update_Shape();

	Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);


	void DrawOutline(glm::vec4 color = glm::vec4(1.0f));
	void DrawNormals();
};

struct polygonData
{
	glm::vec2 RawmidlePosition = { 0.0f,0.0f };
	std::vector <glm::vec2> Rawpoints;
	std::vector <glm::vec2> TexturePoints;
	std::vector <glm::ivec3> indexes;

	std::vector<GLuint> indices;
	std::vector<GLfloat> data;

	unsigned int VAO;
	unsigned int VBO;

	std::string Name;//
	std::string FilePath;//

	int state = 0;
	void add_Point(glm::vec2 point, bool addindex);
	void SaveAs(std::string name);
	void Load(std::string name);
	void Update();
	~polygonData()
	{
		if (VAO != NULL)
			glDeleteVertexArrays(1, &VAO);

		if (VBO != NULL)
			glDeleteBuffers(1, &VBO);

	}
};
// Should be defined clockwise
class polygon
{
public:
	bool lighted = false;
	int Textureid = -1;
	int NormalMapId = -1;
	int id = -1;
	int Z_Index = 0;
	bool Additive = false;
	// Level - on which level it is, Mask - with which level it collides
	int Collision_Level = 0; // -1 - do not collide, >=0 - collision levels
	int Collision_Mask = 0; // -1 - do not collide, >=0 - collision levels
	float roughness = 1.0f;
	float bounciness = 1.0f;
	std::vector <Triangle> triangles;

	glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};


	//glm::vec2 position = glm::vec2(0.0f);
	polygonData* Data;
	int MeshID = -1;
	std::vector < glm::vec2> Transofromedpoints;

	glm::vec2 midlePosition = { 0.0f,0.0f };

	glm::vec2 Position = { 0.0f,0.0f };
	glm::vec2 Velocity = { 0.0f,0.0f };
	glm::vec2 Force = { 0.0f,0.0f };
	float  Mass = 1.0f;

	float Rotation = 0.0f;
	float RotationVelocity = 0.0f;
	float RotationForce = 0.0f;
	float RotationMass = 0.0f;


	glm::vec2 Scale = { 1.0f,1.0f };

	unsigned int Texture;
	unsigned int NormalMap;



	void Process(float dt);


	void Update_MidlePos();
	void Update_Shape();




	void DrawOutline(glm::vec4 color = glm::vec4(1.0f));
	void DrawTrianglesOutlines(glm::vec4 color = glm::vec4(1.0f));

	void DrawTriangles();


};
