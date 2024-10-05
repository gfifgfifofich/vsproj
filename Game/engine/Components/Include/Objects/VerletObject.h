#pragma once

// Very simple physics model. CPU cheap, but inaccurate, and aquaires more substeps to be stable
struct VerletObject
{

	float r = 10;
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec2 ppos = glm::vec2(0.0f);
	glm::vec2 cpos = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	glm::vec2 acceleration = glm::vec2(0.0f);
	float deltatime = 0.017f;
	void UpdatePos();

};



