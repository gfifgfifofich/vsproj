#pragma once

#include "Shader.h"
#include "Objects/Ball.h"
#include "Objects/Quad.h"
#include "Objects/Polygon.h"




#ifndef DRAWINGIMPLEMENTATION
extern
#endif


GLuint
NoizeGenShader,
GradientGenShader,
AddTexturesShader,
RoundShader,
GenNormalMapShader,
GenLightSphereShader,
GenPrimitiveTextureShader,
CopyTextureShader,


FlatColorTexture,
FlatColorCircleTexture,
//Drawing
//Quad
FillScreenShader,
FillShader,
TexturedQuadShader,

InctanceSmoothQuadShader,
InctanceQuadShader,
InstanceTexturedQuadShader,
InstanceFlippedTexturedQuadShader,

//Circle
CircleShader,

//Lighting
InstancedNormalMapShader,
NormalMapDrawShader,
LightShader,

//Triangle
FillTriangleShader,
TexturedTriangleShader,


//Post processing
BlurShader,
UpsampleBlur,
DownsampleBlur,

Chromatic,
shaderBloom,
ScrShade,

//Textures
BallNormalMapTexture,
CubeNormalMapTexture,
NegativeNormalMapTexture,
LightSphereTexture,

///////Was moved into window class
//Buffers
//FrameBuffer, ColorBuffer,
//NormalMapFBO, NormalMapColorBuffer,
//LightColorFBO, LightColorBuffer,


//VertexObjects
quadVAO, quadVBO,
ScreenVAO, ScreenVBO,
CircleVAO, CircleVBO,
TriangleVAO, TriangleVBO,
TexturedTriangleVAO, TexturedTriangleVBO,

TextVAO, TextVBO,
TextShader;
;


#ifndef DRAWINGIMPLEMENTATION
extern
#endif
float ScaleMultiplyer,
ScreenAspectRatio,
ScreenDivisorX,
ScreenDivisorY;

#ifndef DRAWINGIMPLEMENTATION
extern
#endif

int window_id;


enum TextureShape
{
	SQUERE = 0,
	SMOOTH_EDGE = 1,
	ROUND = 2
};




struct LightSource
{
	float volume = 0.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0, -0.5f);
	glm::vec3 scale = glm::vec3(0.0f);
	float rotation = 0.0f;
	glm::vec4 color = glm::vec4(0.0f);
	unsigned int texture = LightSphereTexture;
	int TextureId = 0;
	std::string name = "LightSource";
};
class Texture
{
public:
	int id = 0;//meaningless number
	std::string FileName = "";
	int Type = 0;// 0-Texture, 1-ROUNDNOIZE, 2-SQUERENOIZE, 3-SMOOTH_EDGENOIZE;
	unsigned int texture = NULL;

	float Noize_Frequency = 10;
	int Noize_Layers = 3;

	float Size = 100.0f;

	bool filter = 0; // 0-linear, 1 - nearest/pixels;

	glm::vec4 Gradient_Color1 = glm::vec4(1.0f);
	glm::vec4 Gradient_Color2 = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

	void Load();
	void Delete();
};
class Material
{
public:
	unsigned int Texture = 0;
	unsigned int NormalMap = 0;

	unsigned int HeightMap = 0;// not implemented;
	unsigned int Specular = 0;// not implemented;
	unsigned int Reflective = 0;// not implemented;

	bool flipX = false;
	bool flipY = false;


	bool operator== (Material& m)
	{
		return 
			Texture == m.Texture &&
			NormalMap == m.NormalMap &&
			HeightMap == m.HeightMap &&
			Specular == m.Specular &&
			Reflective == m.Reflective && 
			flipX == m.flipX &&
			flipY == m.flipY;
	}
};
struct TexturedQuadArray
{
	Material material;
	std::vector <glm::vec4> QuadPosScale;
	std::vector <float> QuadRotations;
	std::vector <float> QuadDepth;
	std::vector <glm::vec4> Quadcolors;

	void clear()
	{
		QuadPosScale.clear();
		QuadRotations.clear();
		QuadDepth.clear();
		Quadcolors.clear();
	}
};
struct PolygonArray
{
	GLuint VAO;
	GLuint Size;
	Material material;
	std::vector <glm::vec4> colors;
	std::vector <glm::vec4> PosScale;
	std::vector <float> Rotations;
	std::vector <float> Depths;
	void clear()
	{
		colors.clear();
		PosScale.clear();
		Rotations.clear();
		Depths.clear();
	}
};



struct Character {
	GLuint     TextureID; 
	glm::ivec2 Size;      
	glm::ivec2 Bearing;   
	GLint     Advance;   
};
struct TextLine
{
	std::string text;
	GLfloat x;
	GLfloat y;
	GLfloat scale;
	glm::vec4 color = glm::vec4(1.0f);
	bool aboveEverything = false;
};

struct SceneLayer
{
	int Z_Index = 0;

	bool Additive = false;

	std::vector <TextLine> TextLines;
	std::vector <TextLine> UI_TextLines;


	std::vector <glm::vec4> CirclePosScale;
	std::vector <float> CircleRotations;
	std::vector <glm::vec4> Circlecolors;

	std::vector <glm::vec4> QuadPosScale;
	std::vector <float> QuadRotations;
	std::vector <glm::vec4> Quadcolors;

	std::vector <glm::vec4> SmoothQuadPosScale;
	std::vector <float> SmoothQuadRotations;
	std::vector <glm::vec4> SmoothQuadcolors;

	std::vector <TexturedQuadArray> TexturedQuads;

	std::vector <TexturedQuadArray> NormalMaps;


	std::vector <PolygonArray> PolygonNormalMaps;
	std::vector <PolygonArray> Polygons;



};


#ifndef DRAWINGIMPLEMENTATION
extern
#endif
std::map<GLchar, Character> Characters;

#ifndef DRAWINGIMPLEMENTATION
extern
#endif
std::vector <LightSource> LightSources;

#ifndef DRAWINGIMPLEMENTATION
extern
#endif
std::vector <SceneLayer> SceneLayers;


// A class that can be used to draw scene unto a texture.
class Window
{

	bool inited = false;

public:

	bool Lighting = true;

	glm::vec2 Position = { 0.0f,0.0f };
	glm::vec2 Scale = { 1.0f,1.0f };
	bool linearFilter; 
	bool hdr;
	//ScreenMousePosition, but in window
	glm::vec2 WindowMousePosition;


	float w_ScreenAspectRatio = -1.0f;


	float w_ScreenDivisorX = -1.0f;
	float w_ScreenDivisorY = -1.0f;
	float w_ScaleMultiplyer = -1.0f;

	// Given after creation. May change when after creation or delition of window.
	unsigned int id = 0;

	unsigned int framebuffer = NULL;
	//Texture on wich window is drawn.
	unsigned int Texture = NULL;


	unsigned int
		NormalMapFBO = NULL,
		NormalMapColorBuffer = NULL,
		LightColorFBO = NULL,
		LightColorBuffer = NULL;


	std::vector <LightSource> w_LightSources;
	std::vector <SceneLayer> w_SceneLayers;
	glm::vec2 w_CameraPosition = { 0.0f,0.0f };
	glm::vec2 w_CameraScale = {1.0f,1.0f};

	glm::vec2 ViewportSize = { 400,400 };


	// Allows UI interactions (mouse clicks, button presses) ( Not implemented)
	bool active = true;

	glm::vec4 backgroundColor = { 0.0f,0.0f,0.0f,1.0f };


	bool Autoclear = true;// Automatically clear window each frame
	bool AutoActive = true;// Automatically activate window when mouse is over it, deactivate otherwise
	bool AutoDraw = true;// Automatically call _Draw() function after On_Update()

	bool Destroyed = false;

	float w_AmbientLight = 1.0f;
	float w_DirectionalLight = 1.0f;

	glm::vec2 w_LastJustPressedLMBMousePos = glm::vec2(0.0f);
	glm::vec2 w_LastJustPressedRMBMousePos = glm::vec2(0.0f);
	glm::vec2 w_LastJustPressedMMBMousePos = glm::vec2(0.0f);

	glm::vec2 w_LastJustPressedLMBScrMousePos = glm::vec2(0.0f);
	glm::vec2 w_LastJustPressedRMBScrMousePos = glm::vec2(0.0f);
	glm::vec2 w_LastJustPressedMMBScrMousePos = glm::vec2(0.0f);


	// Do it if you changed ViewportSize
	void RecalculateSize();


	void Init(glm::vec2 ViewportSize = { 400.0f,400.0f }, bool linearFilter = true, bool hdr = true);

	// Sets this window's texture as drawing target. 
	void Use(bool ProcessControls = true);

	void Clear(glm::vec4 Color);

	// Sets everything back to normal, after Use(). 
	void End();

	void _Draw();

	void Draw(int Z_Index =0);
	glm::vec2 GetSize();

	void Destroy();

};


inline Window* CurrentWindow;

inline std::vector<Window> Windows;

void SortSceneLayers();
int FindSceneLayer(int Z_Index, bool Additive);

int CreateWindow();

//One time use. May become invalid after adding or deleating window, safe if otherwise.
Window* GetWindow(int id);

void UpdateWindows();


void UseWindow(int id);
void EndOfWindow();


void PreLoadShaders();
void DrawLight(glm::vec2 position, glm::vec2 scale, glm::vec4 color, float volume = 0.0f, float rotation = 0.0f, unsigned int texture = LightSphereTexture);
void DrawLight(glm::vec3 position, glm::vec2 scale, glm::vec4 color, float volume = 0.0f, float rotation = 0.0f, unsigned int texture = LightSphereTexture);
void DrawLight(glm::vec3 position, glm::vec3 scale, glm::vec4 color, float volume = 0.0f, float rotation = 0.0f, unsigned int texture = LightSphereTexture);
void NormalMapDraw(glm::vec2 position, glm::vec2 scale, unsigned int NormalMap = BallNormalMapTexture, float rotation = 0.0f, int Z_Index = 0, unsigned int Texture = NULL, bool Additive = false, float depth = 0.0f, unsigned int HeightMap = NULL);
void NormalMapDrawTriangle(
	glm::vec2 p1,
	glm::vec2 p2,
	glm::vec2 p3,
	unsigned int NormalMap,
	glm::vec2 texcoord1 = glm::vec2(0.0f, 1.0f),
	glm::vec2 texcoord2 = glm::vec2(0.5f, 0.0f),
	glm::vec2 texcoord3 = glm::vec2(1.0f, 1.0f)
);
void DrawCircle(glm::vec2 position, float r, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = BallNormalMapTexture, int Z_Index = 0, bool Additive = false);
void DrawCircle(ball b, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = BallNormalMapTexture, int Z_Index = 0, bool Additive = false);
void DrawCube(glm::vec2 position, glm::vec2 scale, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = CubeNormalMapTexture, int Z_Index = 0, bool Additive = false);
void DrawCube(cube c, glm::vec4 color = glm::vec4(1.0f), float rotation = 0.0f, bool Lighted = false, unsigned int NormalMap = NULL, int Z_Index = 0, bool Additive = false);
void DrawLine(glm::vec2 p1, glm::vec2 p2, float width = 1.0f, glm::vec4 color = glm::vec4(1.0f), bool Lighted = false, unsigned int NormalMap = CubeNormalMapTexture, int Z_Index = 0);
void DrawBall(ball b, glm::vec4 Color1 = glm::vec4(1.0f), glm::vec4 Color2 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), bool Lighted = false, unsigned int NormalMap = NULL, int Z_Index = 0);

void LoadTexture(const char* filename, unsigned int* texture, int chanelsAmount = 4);
void LoadTextureFromData(unsigned int* texture, int width, int height, unsigned char* Data, int chanelsAmount = 4);
void GenNoizeTexture(unsigned int* texture1, int Size, int Layers = 3, float freq = 10, int shape = ROUND);
void GenPrimitiveTexture(unsigned int* texture1, int Size, int shape = ROUND,bool filter = 0);
void GenNormalMapTexture(unsigned int* texture1, int Size, int shape = ROUND);
void GenLightSphereTexture(unsigned int* texture1, int Size);
void GenGradientTexture(unsigned int* texture1, glm::vec4 Color1 = glm::vec4(1.0f), glm::vec4 Color2 = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), int Size = 100);

void CopyTexture(glm::vec2 size, unsigned int* to,unsigned int from,int mode);// mode: 0 - all, 1 - (r,g,b,1.0f) , 2 - (0,0,0,a), 3 - (a,a,a,a)  

void DrawShaderedQuad(glm::vec2 position, glm::vec2 scale, float rotation, unsigned int shaderProgram);

void DrawQuadWithMaterial(glm::vec2 position, glm::vec2 scale, Material material , float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false, float depth = 0.0f);
void DrawQuadWithMaterial(cube c, Material material , float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, bool Additive = false, float depth = 0.0f);
void DrawSmoothQuad(glm::vec2 position, glm::vec2 scale, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0,bool Additive = false, bool flipX = false, bool flipY=false);
void DrawTexturedQuad(glm::vec2 position, glm::vec2 scale, unsigned int texture, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), int Z_Index = 0, unsigned int NormalMap = NULL, bool Additive = false, bool flipX = false, bool flipY=false, float depth = 0.0f, unsigned int HeightMap = NULL);
void DrawTexturedQuad(cube c, unsigned int texture, glm::vec4 color = glm::vec4(1.0f), float rotation = 0.0f, int Z_Index = 0, unsigned int NormalMap = NULL, bool Additive = false, bool flipX = false, bool flipY = false, float depth = 0.0f, unsigned int HeightMap = NULL);
void DrawTexturedLine(unsigned int Texture, glm::vec2 p1, glm::vec2 p2, float width =1.0f, glm::vec4 color=glm::vec4(1.0f), unsigned int NormalMap = NULL, int Z_Index = 0, bool Additive = false, bool flipX = false, bool flipY = false, float depth = 0.0f, unsigned int HeightMap = NULL);
void DrawLineWithMaterial(Material mater, glm::vec2 p1, glm::vec2 p2, float width = 1.0f, glm::vec4 color = {1.0f,1.0f,1.0f,1.0f},int Z_Index = 0, bool Additive = false, float depth = 0.0f, unsigned int HeightMap = NULL);
void DrawTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color = glm::vec4(1.0f));

void DrawTexturedTriangle(
	glm::vec2 p1,
	glm::vec2 p2,
	glm::vec2 p3,
	unsigned int texture,
	glm::vec4 color = glm::vec4(1.0f),
	glm::vec2 texcoord1 = glm::vec2(0.0f, 1.0f),
	glm::vec2 texcoord2 = glm::vec2(0.5f, 0.0f),
	glm::vec2 texcoord3 = glm::vec2(1.0f, 1.0f)
);

void DrawPolygon(polygon* p, int Z_Index = 0, bool Additive = false);


