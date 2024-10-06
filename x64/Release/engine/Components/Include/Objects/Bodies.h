#pragma once
#include "Ball.h"
#include "Quad.h"
struct MegaStruct
{
	ball b;
	cube c;
};

class KinematicBody
{
public:
	MegaStruct body;
	glm::vec2* position;
	glm::vec4 Color = glm::vec4(1.0f);
	int type = 0;
	void init(int type, glm::vec4 color = glm::vec4(1.0f));
	void Draw();
	void PreProcess(float dt);

	virtual void Process(float dt);
};




class RigidBody
{
public:
	ball body;
	glm::vec4 Color = glm::vec4(1.0f);
	glm::vec4 Color2 = glm::vec4(1.0f,0.0f,0.0f,1.0f);
	void init(glm::vec2 position, float radius, glm::vec4 color = glm::vec4(1.0f));
	void Draw();
	void PreProcess(float dt);
	virtual void Process(float dt);
};


