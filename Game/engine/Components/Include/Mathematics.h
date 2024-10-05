#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const float pi = 3.14159;

glm::vec2 Normalize(glm::vec2 a);

float length(glm::vec2 a);
float sqrlength(glm::vec2 a);


float DOT(glm::vec2 v1, glm::vec2 v2);

glm::vec2 NormalizeSquered(glm::vec2 a);

float get_angle_between_points(glm::vec2 vec1, glm::vec2 vec2);
float get_relative_angle_between_points(glm::vec2 a, glm::vec2 b);

int Facorial(int a);
int C(int a, int b);


glm::vec2 GetNormal(glm::vec2 a);

glm::mat3x3 CreateTransformMat3x3(glm::vec2 position, glm::vec2 scale, float rotation);
float LinearInterpolation(float a0, float a1, float w);

float SmoothStep(float a0, float a1, float w);

float SmootherStep(float a0, float a1, float w);

glm::vec2 Rotate(glm::vec2 a, float angle);



float sigmoid(float x);
float sigmoidApprox(float x);

float ReLu(float x);