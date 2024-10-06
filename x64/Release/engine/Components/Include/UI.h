#pragma once



//Draws in screen coords
void UI_NormalMapDraw(glm::vec2 position, glm::vec2 scale, unsigned int NormalMap = BallNormalMapTexture, float rotation = 0.0f, int Z_Index = 0, unsigned int Texture = NULL, bool Additive = false, float depth = 0.0f, unsigned int HeightMap = NULL);


void UI_DrawQuadWithMaterial(glm::vec2 position, glm::vec2 scale, Material material, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f),bool flipY = false, int Z_Index = 0,  bool Additive = false , float depth = 0.0f);
void UI_DrawQuadWithMaterial(cube c, Material material, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f),bool flipY = false, int Z_Index = 0, bool Additive = false, float depth = 0.0f);
void UI_DrawSmoothQuad(glm::vec2 position, glm::vec2 scale, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false, bool flipX = false, bool flipY = false);

void UI_DrawTexturedQuad(glm::vec2 position, glm::vec2 scale, unsigned int texture, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, unsigned int NormalMap = NULL, bool Additive = false, bool flipX = false, bool flipY = false, float depth = 0.0f, unsigned int HeightMap = NULL);
void UI_DrawTexturedQuad(cube c, unsigned int texture, glm::vec4 color = glm::vec4(1.0f), float rotation = 0.0f, int Z_Index = 0, unsigned int NormalMap = NULL, bool Additive = false, bool flipX = false, bool flipY = false, float depth = 0.0f, unsigned int HeightMap = NULL);
void UI_DrawTexturedLine(unsigned int Texture, glm::vec2 p1, glm::vec2 p2, float width = 1.0f, glm::vec4 color = glm::vec4(1.0f), unsigned int NormalMap = NULL, int Z_Index = 0, float depth = 0.0f, unsigned int HeightMap = NULL);
void UI_DrawCircle(glm::vec2 position, float r, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = BallNormalMapTexture, int Z_Index = 0, bool Additive = false);
void UI_DrawCircle(ball b, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = BallNormalMapTexture, int Z_Index = 0, bool Additive = false);
void UI_DrawCube(glm::vec2 position, glm::vec2 scale, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = CubeNormalMapTexture, int Z_Index = 0, bool Additive = false);
void UI_DrawCube(cube c, glm::vec4 color = glm::vec4(1.0f), float rotation = 0.0f, bool Lighted = false, unsigned int NormalMap = NULL, int Z_Index = 0, bool Additive = false);
void UI_DrawLine(glm::vec2 p1, glm::vec2 p2, float width = 1.0f, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = CubeNormalMapTexture, int Z_Index = 0);
void UI_DrawBall(ball b, glm::vec4 Color1 = glm::vec4(1.0f), glm::vec4 Color2 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), bool Lighted = false, unsigned int NormalMap = NULL, int Z_Index = 0);

void _UI_DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color);

//UI returns size of object
glm::vec2 UI_DrawText(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);

//UI returns size of object
glm::vec2 UI_DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color = glm::vec4(1.0f), int Z_Index=0, bool Additive=false);



//UI returns size of object
glm::vec2 UI_DrawTextOnPlate(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color = glm::vec4(1.0f), glm::vec4 platecolor = glm::vec4(0.0f, 0.0f, 0.0f, 0.7f), int Z_Index = 0, bool Additive = false);

//UI returns size of object
glm::vec2 UI_CheckBox(bool* param, const char* text, glm::vec2 scrPosition, float scale = 18.0f, float textScale = 0.35f, glm::vec4 colorON = glm::vec4(1.0f), glm::vec4 ColorOFF = glm::vec4(0.7f), int Z_Index = 0, bool Additive = false);
//UI returns size of object
glm::vec2 UI_buttonOnlyON(bool* param, const char* text, glm::vec2 scrPosition, float scale = 18.0f, float textScale = 0.35f, glm::vec4 colorON = glm::vec4(1.0f), glm::vec4 ColorOFF = glm::vec4(0.7f), int Z_Index = 0, bool Additive = false);
glm::vec2 UI_button(bool* param, const char* text, glm::vec2 scrPosition, glm::vec2 scale = glm::vec2(150.0f, 35.0f), float textScale = 0.35f, glm::vec4 TextColorAdder = glm::vec4(0.5f), glm::vec4 ColorON = glm::vec4(0.8f), glm::vec4 ColorOFF = glm::vec4(0.5f), int Z_Index = 0, bool Additive = false);


inline int iDragBuff;

inline float fDragBuff;

inline bool Dragging;

	

//UI returns size of object
glm::vec2 UI_Slider(float* param, const char* text, glm::vec2 scrPosition, float min = 0.0f, float max=1.0f, glm::vec2 scale = glm::vec2(150.0f, 5.0f), float textScale = 0.35f, glm::vec4 Lcolor = glm::vec4(0.9f), glm::vec4 Bcolor = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);
//UI returns size of object
glm::vec2 UI_SliderInt(int* param, const char* text, glm::vec2 scrPosition, int min = 0, int max=1, glm::vec2 scale = glm::vec2(150.0f, 5.0f), float textScale = 0.35f, glm::vec4 Lcolor = glm::vec4(0.9f), glm::vec4 Bcolor = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);

//UI returns size of object
glm::vec2 UI_Drag(float* param, const char* text, glm::vec2 scrPosition, float speed = 1.0f, glm::vec2 scale = glm::vec2(150.0f,15.0f), float textScale = 0.35f, glm::vec4 Backcolor = glm::vec4(0.4f), glm::vec4 Textcolor = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);
//UI returns size of object
glm::vec2 UI_DragInt(int* param, const char* text, glm::vec2 scrPosition, float speed = 1.0f , glm::vec2 scale = glm::vec2(150.0f, 15.0f), float textScale = 0.35f, glm::vec4 Backcolor = glm::vec4(0.4f), glm::vec4 Textcolor = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);

//UI returns size of object
glm::vec2 UI_TextBox(std::string* text, glm::vec2 scrPosition, int maxTextSize =-1, glm::vec2 scale = glm::vec2(150.0f, 15.0f), float textScale = 0.35f, glm::vec4 Backcolor = glm::vec4(0.4f), glm::vec4 Textcolor = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false);
