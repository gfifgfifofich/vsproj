#pragma once
#include <list>
class Graph
{
public:
	std::list<float> data;
	

	float stepX = 1.0f;
	float stepY = 1.0f;
	float originX = 0.0f;
	float originY = 0.0f;
	glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
	float width = 1.0f;

	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 scale = glm::vec2(1.0f);
	glm::vec2 DataScale = { 1.0f,1.0f };

	void Inspect(glm::vec2 Coord);

	void Inspect(float x);

	void Draw(bool NullLines = true);

	Graph(int DataSize, float stepX = 1.0f, float stepY = 1.0f, float originX = 0.0f, float originY = 0.0f);
};

