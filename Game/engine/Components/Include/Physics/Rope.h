#pragma once
void Rope(ball* a, ball* b, float length);
void MRope(ball* a, ball* b, float length);
class DecorativeRope
{
	ball rope[5];
public:

	ball* b1;
	ball* b2;
	float length = 0.0f;

	float width = 1.0f;
	unsigned int Texture = NULL;
	unsigned int NormalMap = NULL;

	glm::vec2 Force = { 0.0f,0.0f };
	glm::vec4 color = glm::vec4(1.0f);
	void Init(ball* b1, ball* b2, float length = 100.0f);
	void Process(float dt);
	void Draw(int Z_Index = 0);

};