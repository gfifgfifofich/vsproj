#pragma once
#include "Shader.h"

#define FT_FREETYPE_H
#include <ft2build.h>
#include <freetype/freetype.h>

//Text

void LoadFont(const char* font, int size = 48);
glm::vec2 getTextSize(std::string Text, GLfloat scale);
void _DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color = glm::vec4(1.0f));
void DrawText(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);
void DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);
