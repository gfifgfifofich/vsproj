#pragma once

class line
{
public:
	int id = 0;
	int Z_Index = 0;
	// Level - on which level it is, Mask - with which level it collides
	int Collision_Level = 0; // -1 - do not collide, >=0 - collision levels
	int Collision_Mask = 0; // -1 - do not collide, >=0 - collision levels

	float roughness = 1.0f;
	float width = 1.0f;
	glm::vec2 P1 = glm::vec2(0.0f);
	glm::vec2 P2 = glm::vec2(0.0f);
	glm::vec2 normal = glm::vec2(0.0f);

	line(glm::vec2 p1, glm::vec2 p2, bool Calculate_normal = true);

	void Calculate_normal();
};


