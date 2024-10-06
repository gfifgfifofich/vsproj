#pragma once



struct cube
{
	bool lighted = false;
	int Shaderid = -1;
	int Textureid = -1;
	int NormalMapId = -1;
	int id = -1;
	int type = -1;
	int Z_Index = 0;
	// Level - on which level it is, Mask - with which level it collides
	int Collision_Level = 0; // -1 - do not collide, >=0 - collision levels
	int Collision_Mask = 0; // -1 - do not collide, >=0 - collision levels

	float Rotation = 0.0f;// only visual yet
	unsigned int Texture;

	float width = 0.0f;
	float height = 0.0f;
	bool CollisionSides[4];
	float roughness = 1.0f;
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);

	glm::vec4 color = glm::vec4(1.0f);
};
static std::vector <cube*> cubes;
