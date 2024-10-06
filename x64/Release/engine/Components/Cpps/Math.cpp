#include "../Include/Mathematics.h"



glm::vec2 Normalize(glm::vec2 a)
{
	float length = sqrt(a.x * a.x + a.y * a.y);
	if(length< 0.000001f)
		return {0.0f,1.0f};// just up vector
	return a / length;
}

float length(glm::vec2 a)
{
	float length = sqrt(a.x * a.x + a.y * a.y);
	return length;
}
float sqrlength(glm::vec2 a)
{
	float length = a.x * a.x + a.y * a.y;
	return length;
}


float DOT(glm::vec2 v1, glm::vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

glm::vec2 NormalizeSquered(glm::vec2 a)
{
	// Fast, not preciese at all
	int negative[2];
	negative[0] = -std::signbit(a.x) * 2 + 1;
	negative[1] = -std::signbit(a.y) * 2 + 1;
	float sqrlen = a.x * a.x + a.y * a.y;
	return glm::vec2((a.x * a.x) / sqrlen * negative[0], (a.y * a.y) / sqrlen * negative[1]);

}

float get_angle_between_points(glm::vec2 vec1, glm::vec2 vec2)
{
	glm::vec2 vec3 = vec1 - vec2;
	float tan = vec3.y / vec3.x;
	float  angle = atan(tan);
	if (vec1.x >= vec2.x)
	{
		angle += pi;
	}
	return angle - pi * 0.5f;
}
float get_relative_angle_between_points(glm::vec2 a, glm::vec2 b)
{
	float cos = DOT(a, b) / (length(a) * length(b));

	float  angle = acos(cos);

	if (cos > 0.999f)
		angle = 0.0f;

	if (cos < -0.999f)
		angle = pi;
	glm::vec2 norm = glm::vec2(-b.y, b.x);


	if (DOT(a, norm) <= 0.0f)
		angle *= -1;



	return angle;
}

int Facorial(int a)
{
	if (a == 0)
		return 1;
	int n = 1;

	for (int i = 1; i <= a; i++)
		n *= i;
	return n;
}
int C(int a, int b)
{
	return Facorial(b) / (Facorial(a) * Facorial(b - a));
}


glm::vec2 GetNormal(glm::vec2 a)// i'm stupid, so this function rotates vec 'a' by 90 degrees lol
{
	return Normalize(glm::vec2(-a.y, a.x));
}

glm::mat3x3 CreateTransformMat3x3(glm::vec2 position, glm::vec2 scale, float rotation)
{

	float cosPhi = cos(rotation);
	float sinPhi = sin(rotation);


	return glm::mat3x3{ cosPhi * scale.x,	-sinPhi * scale.y,  cosPhi * position.x - sinPhi * position.y,
						sinPhi * scale.x, 	cosPhi * scale.y,     cosPhi * position.y + sinPhi * position.x,
						0.0f,						0.0f,		1.0f };
}
float LinearInterpolation(float a0, float a1, float w) {
	return (a1 - a0) * w + a0;
}

float SmoothStep(float a0, float a1, float w) {
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float SmootherStep(float a0, float a1, float w) {
	return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

glm::vec2 Rotate(glm::vec2 a, float angle)
{
	float Sin = sin(angle);
	float Cos = cos(angle);
	return glm::vec2(Cos * a.x - Sin * a.y, Sin * a.x + Cos * a.y);

}



float sigmoid(float x)
{
	return 1 / (1 + pow(2.71828, -x));
}

float sigmoidApprox(float x)
{



	if (x > -2.05f && x < 2.05f)
		return x * 0.2f + 0.5f;

	if (x < -5)
		return 0.0f;

	if (x > 5)
		return 1.0f;

	if (x < -2.05f)
		return x * 0.03f + 0.15f;

	if (x > 2.05f)
		return x * 0.03f + 0.85f;
}
float ReLu(float x)
{
	return x >= 0.0f ? x : 0.0f;
}