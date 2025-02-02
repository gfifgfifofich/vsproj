#pragma once
void Rope(ball* a, ball* b, float length);
void MRope(ball* a, ball* b, float length);
class DecorativeRope
{
public:
	ball rope[5];

	ball* b1 = nullptr;
	ball* b2 = nullptr;
	float length = 0.0f;

	float width = 1.0f;
	unsigned int Texture = NULL;
	unsigned int NormalMap = NULL;

	glm::vec2 Force = { 0.0f,0.0f };
	glm::vec4 color = glm::vec4(1.0f);
	void Init(ball* b1, ball* b2, float length = 100.0f);
	void Process(float dt, bool updateb1b2 = true);
	void Draw(int Z_Index = 0);

};