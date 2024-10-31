#include "../Redactor.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <latch>
#include <execution>

#include "../Include/sounds.h"

//std::string MapFileName =  "Maps/MenuScene.sav";
//std::string MapFileName =  "Maps/base.sav";
//std::string MapFileName =  "Maps/Background.sav";
//std::string MapFileName =  "../../HEAT/Maps/base.sav";

//std::string MapFileName = "Maps/Shadertest.sav";
/*


	Corner.y += UI_DrawText("Fragment path: ", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_TextBox(&CurrentShader->FragmentPath, Corner, &Texteditcurspos, &textedit, -1).y * -1.0f - step;
	Corner.y += UI_buttonOnlyON(&b,"Load", Corner).y * -1.0f - step;
	Corner.y += UI_CheckBox(&CurrentParticleEmiter->ShowWindow,"ShowSettings", Corner).y * -1.0f - step;


	float xsize = 0.0f;

	Corner.y += UI_Drag(&CurrentShader->uniformfloat[CurrentShader->Uniforms[i].type_id], CurrentShader->Uniforms[i].name.c_str(), Corner, 0.01f).y * -1.0f - step;
	Corner.y += UI_DragInt(&CurrentShader->uniformint[CurrentShader->Uniforms[i].type_id], CurrentShader->Uniforms[i].name.c_str(), Corner, 0.01f).y * -1.0f - step;
	
	Corner.y += UI_DrawText(CurrentShader->Uniforms[i].name.c_str(), Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&CurrentShader->uniformvec2[CurrentShader->Uniforms[i].type_id].x, "x", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&CurrentShader->uniformvec2[CurrentShader->Uniforms[i].type_id].y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_DrawText(CurrentShader->Uniforms[i].name.c_str(), Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&CurrentShader->uniformvec3[CurrentShader->Uniforms[i].type_id].x, "x", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&CurrentShader->uniformvec3[CurrentShader->Uniforms[i].type_id].y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&CurrentShader->uniformvec3[CurrentShader->Uniforms[i].type_id].z, "z", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		
	Corner.y += UI_DrawText(CurrentShader->Uniforms[i].name.c_str(), Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].x, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].y, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].z, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].w, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
*/


inline bool _Scenethreads_stop;

inline std::thread* _Scenethreads;
inline std::condition_variable* _SceneConVars;
inline std::mutex* _SceneMutexes;
static inline std::atomic<bool>* _ScenethreadsStates;// 1 waiting | 0 working | -1 done/broken

void _Scenethead_Process(int thr);
void _StartScenethread(int t);
void _StartScenethreads();
void _DeleteScenethreads();
void _mt_SceneProcess(int thr);

void _Scenethead_Process(int thr)
{
	std::unique_lock<std::mutex> lm(_SceneMutexes[thr]);
	_SceneConVars[thr].wait(lm);
	
	if(GameScene!=NULL)
		_mt_SceneProcess(thr);
	
}


void _StartScenethread(int t)
{
	while(!_Scenethreads_stop)
	{
		
		_Scenethead_Process(t);
		_ScenethreadsStates[t] = 1;
	}
}

void _StartScenethreads()
{
	_Scenethreads_stop = false;
	_SceneMutexes = new std::mutex[threadcount];
	_SceneConVars = new std::condition_variable[threadcount];
	_Scenethreads = new std::thread[threadcount];
	_ScenethreadsStates = new std::atomic<bool>[threadcount];
	for(int i=0;i<threadcount;i++)
	{
		_Scenethreads[i] = std::thread(_StartScenethread,i);
		_ScenethreadsStates[i] = 1;
	}
}


void _DeleteScenethreads()
{
	_Scenethreads_stop = true;
	for(int thr = 0;thr<threadcount;thr++)
	{
		_Scenethreads[thr].~thread();
	}
}

bool Running = false;
bool Paused = false;
bool threadsprepass = false;
int threadNodestep = 1;
int threadNodeEnd = 1;
void _mt_SceneProcess(int thr)
{
	int step = threadNodestep;
	int begin = thr * step;
	int end = (thr + 1) * step;
	if (thr == 0)
		begin =0;

	if (thr == threadcount-1)
		end = threadNodeEnd;

	for (int i = begin; i < end; i++)
	{
		if(threadsprepass)
			GameScene->Nodes[i]->MTPreProcess();
		else if(!Paused && Running)
			GameScene->Nodes[i]->MTProcess(GameScene->dt);
	}
}

int InspectorWindowID;
int ProjectWindowID;
int ConsoleWindowID;

unsigned int NormalMapScreen = 0;
unsigned int HeightMapScreen = 0;

Texture tex;

std::string TexturePath;

glm::vec2 PrevDifference = { 0.0f,0.0f };
glm::vec2 Corner = { 0.0f,0.0f };

glm::ivec3 tmpIndex = glm::ivec3(-1);
int indexCreationState = 0;
float Simulation_speed = 1.0f;
bool grid = true;
float grid_step = 15.0f;

int NewObjectId = 0;
int NewAssetId = 1;

float vel[2] = { 0.0f,0.0f };
float size[2] = { 10.0f,10.0f };
float radius = 10.0f;
float tick = 0.017f;
int amount = 1;
int Id = 0;
float AttractionForce = 10.0f;
bool attracticve = false;
float LightColor[4] = { 1.0f, 1.0f,1.0f,1.0f };
bool Lighted = false;

int noizeiterator = 0;


polygonData DefaultTriangle;

Node* GrabbedNode = NULL;
int GrabbedNodeID = -1;
bool grabbed = false;

Node* SelectedNode = NULL;
int SelectedNodeID = -1;

Asset* SelectedAsset = NULL;
int SelectedAssetID = -1;
void ProcessSelectedNodeUI()
{
	float xsize = 0.0f;
	float step = 15.0f;
	float xoffset = 0.0f;
	std::vector<UI_DataPack> datapacks;
	if(SelectedNode!=NULL)
	{
		datapacks = SelectedNode->GetUIData();
		Corner.y += UI_DrawText(SelectedNode->Name, Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;	
	}
	else if (SelectedAsset !=NULL)
	{
		datapacks = SelectedAsset->GetUIData();
		Corner.y += UI_DrawText(SelectedAsset->Name, Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;	
	}
	else
		return;

	for(int i=0;i<datapacks.size();i++)
	{
		xsize = 0.0f;
		xoffset=20.0f;
		Corner.y += UI_DrawText(datapacks[i].name, Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;
		xoffset = 40.0f;
		for(int a = 0; a<datapacks[i].bdata.size();a++)
			Corner.y += UI_CheckBox(datapacks[i].bdata[a],datapacks[i].bdatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f)).y * -1.0f - step;
			
		for(int a = 0; a<datapacks[i].idata.size();a++)
			Corner.y += UI_DragInt(datapacks[i].idata[a],datapacks[i].idatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f),0.2f).y * -1.0f - step;
			

		for(int a = 0; a<datapacks[i].fdata.size();a++)
			Corner.y += UI_Drag(datapacks[i].fdata[a],datapacks[i].fdatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f),0.2f).y * -1.0f - step;
			
		for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
			Corner.y += UI_SliderInt(&datapacks[i].iSliderdata[a]->x,datapacks[i].iSliderdatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f),datapacks[i].iSliderdata[a]->y,datapacks[i].iSliderdata[a]->z).y * -1.0f - step;

		for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
			Corner.y += UI_Slider(&datapacks[i].fSliderdata[a]->x,datapacks[i].fSliderdatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f),datapacks[i].fSliderdata[a]->y,datapacks[i].fSliderdata[a]->z).y * -1.0f - step;

		for(int a = 0; a<datapacks[i].v2data.size();a++)
		{
			Corner.y += UI_DrawText(datapacks[i].v2datanames[a], Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&datapacks[i].v2data[a]->x, "x", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&datapacks[i].v2data[a]->y, "y", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
			xsize = 0.0f;
		}
		for(int a = 0; a<datapacks[i].v3data.size();a++)
		{
			Corner.y += UI_DrawText(datapacks[i].v3datanames[a], Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&datapacks[i].v3data[a]->x, "x", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&datapacks[i].v3data[a]->y, "y", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&datapacks[i].v3data[a]->z, "z", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
			xsize = 0.0f;
		}
		for(int a = 0; a<datapacks[i].v4data.size();a++)
		{
			Corner.y += UI_DrawText(datapacks[i].v4datanames[a], Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&datapacks[i].v4data[a]->x, "x", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&datapacks[i].v4data[a]->y, "y", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&datapacks[i].v4data[a]->z, "z", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&datapacks[i].v4data[a]->w, "w", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
			xsize = 0.0f;
		}
		for(int a = 0; a<datapacks[i].colordata.size();a++)
		{
			Corner.y += UI_DrawText(datapacks[i].colordatanames[a], Corner + glm::vec2(xoffset,0.0f), 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&datapacks[i].colordata[a]->x, "r", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&datapacks[i].colordata[a]->y, "g", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&datapacks[i].colordata[a]->z, "b", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&datapacks[i].colordata[a]->w, "a", Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
			xsize = 0.0f;
		}

		for(int a = 0; a<datapacks[i].textdata.size();a++)
		{
			xsize = UI_DrawText(datapacks[i].textdatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f),0.35f).x;
			Corner.y += UI_TextBox(datapacks[i].textdata[a], Corner+ glm::vec2(xoffset,0.0f)  + glm::vec2(xsize, 0.0f)).y * -1.0f - step;
			xsize = 0.0f;
		}

		
		for(int a = 0; a<datapacks[i].texturedata.size();a++)
		{
			xsize = UI_DrawText(datapacks[i].texturedatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f),0.35f).x;
			Corner.y += UI_TextBox(datapacks[i].texturedata[a], Corner+ glm::vec2(xoffset,0.0f)  + glm::vec2(xsize, 0.0f)).y * -1.0f - step;
			xsize = 0.0f;
		}
		for(int a = 0; a<datapacks[i].materialdata.size();a++)
		{
			xsize = UI_DrawText(datapacks[i].materialdatanames[a].c_str(), Corner + glm::vec2(xoffset,-7.5f) + glm::vec2(xsize, 0.0f),0.35f).x;
			Corner.y += UI_TextBox(datapacks[i].materialdata[a], Corner+ glm::vec2(xoffset,0.0f)  + glm::vec2(xsize, 0.0f)).y * -1.0f - step;
			xsize = 0.0f;
		}

		for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
		{
			xsize = UI_DrawText(datapacks[i].ParticleAssetdatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f) + glm::vec2(xsize, 0.0f),0.35f).x;
			Corner.y += UI_TextBox(datapacks[i].ParticleAssetdata[a], Corner+ glm::vec2(xoffset,0.0f)  + glm::vec2(xsize, 0.0f)).y * -1.0f - step;
			xsize = 0.0f;
		}

		
		for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
		{
			
			Corner.y += UI_DrawText(datapacks[i].t_texturedatanames[a].c_str(), Corner + glm::vec2(xoffset,0.0f),0.35f).y * -1.0f - step;
			xoffset+= 20.0f;
			Corner += glm::vec2(xoffset,0.0f);
					
			int typ = datapacks[i].t_texturedata[a]->Type;

			Corner.y += UI_SliderInt(&datapacks[i].t_texturedata[a]->Type, "type", Corner, 0, 11).y * -1.0f - step;

			if (datapacks[i].t_texturedata[a]->Type == 0)
				Corner.y += UI_DrawText("LoadFromName",Corner,0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 1)
				Corner.y += UI_DrawText("Round Noize",Corner,0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 2)
				Corner.y += UI_DrawText("Squere Noize",Corner,0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 3)
				Corner.y += UI_DrawText("Smooth Squere Noize",Corner,0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 4)
				Corner.y += UI_DrawText("Gradient", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 5)
				Corner.y += UI_DrawText("Gradient Sphere", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 6)
				Corner.y += UI_DrawText("Sphere", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 7)
				Corner.y += UI_DrawText("Quad", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 8)
				Corner.y += UI_DrawText("SphereNormalMap", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 9)
				Corner.y += UI_DrawText("QuadNormalMap", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 10)
				Corner.y += UI_DrawText("SphereHeightMap", Corner, 0.35f).y * -1.0f - step;
			if (datapacks[i].t_texturedata[a]->Type == 11)
				Corner.y += UI_DrawText("QuadHeightMap", Corner, 0.35f).y * -1.0f - step;

			std::string filter = "Linear/blurry";
			if (datapacks[i].t_texturedata[a]->filter)
				filter = "Nearest/pixelart";
			bool filt = datapacks[i].t_texturedata[a]->filter;
			Corner.y += UI_CheckBox(&datapacks[i].t_texturedata[a]->filter,filter.c_str(), Corner,18.0f, 0.35f).y * -1.0f - step;

			if (filt != datapacks[i].t_texturedata[a]->filter)
			{

				datapacks[i].t_texturedata[a]->Delete();
				datapacks[i].t_texturedata[a]->Load();
			}


			if (typ != datapacks[i].t_texturedata[a]->Type)
			{
				datapacks[i].t_texturedata[a]->Delete();
				datapacks[i].t_texturedata[a]->Load();
			}

			if(datapacks[i].t_texturedata[a]->Type  == 0)
			{
				xsize = UI_DrawText("FileName", Corner,0.35f).x;
				Corner.y += UI_TextBox(&datapacks[i].t_texturedata[a]->FileName, Corner + glm::vec2(xsize, 0.0f)).y * -1.0f - step;
				xsize = 0.0f;
			}
			if (datapacks[i].t_texturedata[a]->Type > 0 && datapacks[i].t_texturedata[a]->Type < 4)
			{
				float Tex_Freq = datapacks[i].t_texturedata[a]->Noize_Frequency;
				Corner.y += UI_Drag(&datapacks[i].t_texturedata[a]->Noize_Frequency,"Frequency ",Corner, 0.01f).y * -1.0f - step;
				if (Tex_Freq != datapacks[i].t_texturedata[a]->Noize_Frequency)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}
				int NoizeLayers = datapacks[i].t_texturedata[a]->Noize_Layers;

				Corner.y += UI_DragInt(&datapacks[i].t_texturedata[a]->Noize_Layers, "Layers ", Corner, 0.01f).y * -1.0f - step;

				if (NoizeLayers != datapacks[i].t_texturedata[a]->Noize_Layers)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}
				float Tex_Size = datapacks[i].t_texturedata[a]->Size;
				Corner.y += UI_Drag(&datapacks[i].t_texturedata[a]->Size, "Size ", Corner, 0.1f).y * -1.0f - step;
				if (Tex_Size != datapacks[i].t_texturedata[a]->Size)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}
			}
			else if (datapacks[i].t_texturedata[a]->Type == 4)
			{
				glm::vec4 color = datapacks[i].t_texturedata[a]->Gradient_Color1;


				Corner.y += UI_DrawText("Gradient Color1", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color1.r, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color1.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color1.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color1.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

				if (color != datapacks[i].t_texturedata[a]->Gradient_Color1)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}

				color = datapacks[i].t_texturedata[a]->Gradient_Color2;

				Corner.y += UI_DrawText("Gradient Color2", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color2.r, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color2.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color2.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&datapacks[i].t_texturedata[a]->Gradient_Color2.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

				if (color != datapacks[i].t_texturedata[a]->Gradient_Color2)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}

				float Tex_Size = datapacks[i].t_texturedata[a]->Size;
				Corner.y += UI_Drag(&datapacks[i].t_texturedata[a]->Size, "Size ", Corner, 0.1f).y * -1.0f - step;

				if (Tex_Size != datapacks[i].t_texturedata[a]->Size)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}

			}
			else
			{
				
				float Tex_Size = datapacks[i].t_texturedata[a]->Size;
				Corner.y += UI_Drag(&datapacks[i].t_texturedata[a]->Size, "Size ", Corner, 0.1f).y * -1.0f - step;

				if (Tex_Size != datapacks[i].t_texturedata[a]->Size)
				{
					datapacks[i].t_texturedata[a]->Delete();
					datapacks[i].t_texturedata[a]->Load();
				}

			}

			Corner -= glm::vec2(xoffset,0.0f);
			xsize = 0.0f;
			xoffset-= 20.0f;
		}

		
	}
	if(SelectedNode!=NULL)
	{
		SelectedNode->CustomUIDraw(&Corner,xoffset,step);
		if(JustPressedkey[GLFW_KEY_DELETE])
		{
			SelectedNode->Delete = true;
			SelectedNode = NULL;
			SelectedNodeID = -1;
		}
	}
	else if (SelectedAsset !=NULL)
	{
		SelectedAsset->CustomUIDraw(&Corner,xoffset,step);
		if(JustPressedkey[GLFW_KEY_DELETE])
		{
			SelectedAsset->Delete = true;
			SelectedAsset = NULL;
			SelectedAssetID = -1;
		}
	}

}

/*
void ShowParticleObjectRedactorWindow(ParticleEmiter* PE,int type,int i)
{
	
	if(type == -1 || i == -1)
		return;
	float step = 20.0f;
	float xsize = 0.0f;

	Corner.y += UI_DrawText("Particle Object", Corner, 0.35f).y * -1.0f - step;
	if (type == 0)
	{


		Corner.y += UI_DrawText("Sphere of influence", Corner, 0.35f).y * -1.0f - step;


		Corner.y += UI_Drag(&PE->SpheresOfInfluence[i].attractionStrength, "attractionForce", Corner, 0.01f).y * -1.0f - step;
		Corner.y += UI_CheckBox(&PE->SpheresOfInfluence[i].attractive, "attractive", Corner).y * -1.0f - step;

		Corner.y += UI_Drag(&PE->SpheresOfInfluence[i].r, "Radius", Corner, 0.1f).y * -1.0f - step;


		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->SpheresOfInfluence[i].position.x, "x", Corner , 1.0f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->SpheresOfInfluence[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->SpheresOfInfluence[i].velocity.x, "x", Corner , 1.0f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->SpheresOfInfluence[i].velocity.y, "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 80.0f,15.0f }).y * -1.0f - step;

		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->SpheresOfInfluence[i] = PE->SpheresOfInfluence[PE->SpheresOfInfluence.size() - 1];
			PE->SpheresOfInfluence.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}

	}
	else if (type == 1)
	{
		Corner.y += UI_DrawText("Cube of influence", Corner, 0.35f).y * -1.0f - step;

		Corner.y += UI_Drag(&PE->CubesOfInfluence[i].attractionStrength, "attractionForce", Corner, 0.01f).y * -1.0f - step;
		Corner.y += UI_CheckBox(&PE->CubesOfInfluence[i].attractive, "attractive", Corner).y * -1.0f - step;

		Corner.y += UI_DrawText("Scale", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->CubesOfInfluence[i].scale.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->CubesOfInfluence[i].scale.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->CubesOfInfluence[i].position.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->CubesOfInfluence[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->CubesOfInfluence[i].velocity.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->CubesOfInfluence[i].velocity.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;
		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->CubesOfInfluence[i] = PE->CubesOfInfluence[PE->CubesOfInfluence.size() - 1];
			PE->CubesOfInfluence.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}
	}
	else if (type == 2)
	{
		Corner.y += UI_DrawText("Point Emiter", Corner, 0.35f).y * -1.0f - step;


		Corner.y += UI_Drag(&PE->EmitionPoints[i].tick, "Tick", Corner, 0.01f).y * -1.0f - step;
		Corner.y += UI_DragInt(&PE->EmitionPoints[i].amount, "Amount", Corner, 0.01f).y * -1.0f - step;

		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionPoints[i].position.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionPoints[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionPoints[i].velocity.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionPoints[i].velocity.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;
		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->EmitionPoints[i] = PE->EmitionPoints[PE->EmitionPoints.size() - 1];
			PE->EmitionPoints.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}
	}
	else if (type == 3)
	{
		Corner.y += UI_DrawText("Crcle Emiter", Corner, 0.35f).y * -1.0f - step;


		Corner.y += UI_Drag(&PE->EmitionCircles[i].tick, "Tick", Corner, 0.01f).y * -1.0f - step;
		Corner.y += UI_DragInt(&PE->EmitionCircles[i].amount, "Amount", Corner, 0.01f).y * -1.0f - step;

		Corner.y += UI_Drag(&PE->EmitionCircles[i].r, "Radius", Corner, 0.01f).y * -1.0f - step;


		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionCircles[i].position.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionCircles[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionCircles[i].velocity.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionCircles[i].velocity.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;
		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->EmitionCircles[i] = PE->EmitionCircles[PE->EmitionCircles.size() - 1];
			PE->EmitionCircles.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}
	}
	else if (type == 4)
	{
		Corner.y += UI_DrawText("Cube Emiter", Corner, 0.35f).y * -1.0f - step;


		Corner.y += UI_Drag(&PE->EmitionCubes[i].tick, "Tick", Corner, 0.01f).y * -1.0f - step;
		Corner.y += UI_DragInt(&PE->EmitionCubes[i].amount, "Amount", Corner, 0.01f).y * -1.0f - step;


		Corner.y += UI_DrawText("scale", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionCubes[i].scale.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionCubes[i].scale.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionCubes[i].position.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionCubes[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->EmitionCubes[i].velocity.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->EmitionCubes[i].velocity.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;
		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->EmitionCubes[i] = PE->EmitionCubes[PE->EmitionCubes.size() - 1];
			PE->EmitionCubes.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}
	}
	else if (type == 5)
	{
		Corner.y += UI_DrawText("Light sphere", Corner, 0.35f).y * -1.0f - step;


		Corner.y += UI_Drag(&PE->LightSpheres[i].r, "Radius", Corner, 0.01f).y * -1.0f - step;


		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->LightSpheres[i].position.x, "x", Corner, 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->LightSpheres[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;


		Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->LightSpheres[i].Color.x, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&PE->LightSpheres[i].Color.y, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&PE->LightSpheres[i].Color.z, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->LightSpheres[i].Color.w, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->LightSpheres[i] = PE->LightSpheres[PE->LightSpheres.size() - 1];
			PE->LightSpheres.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}
	}
	else if (type == 6)
	{
		Corner.y += UI_DrawText("Light cube", Corner, 0.35f).y * -1.0f - step;


		Corner.y += UI_DrawText("scale", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->LightCubes[i].scale.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->LightCubes[i].scale.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;

		Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->LightCubes[i].position.x, "x", Corner , 0.01f, { 80.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->LightCubes[i].position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 80.0f,15.0f }).y * -1.0f - step;


		Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&PE->LightCubes[i].Color.x, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&PE->LightCubes[i].Color.y, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&PE->LightCubes[i].Color.z, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&PE->LightCubes[i].Color.w, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		bool b = false;
		Corner.y += UI_CheckBox(&b, "delete", Corner).y * -1.0f - step;
		if (b)
		{
			PE->LightCubes[i] = PE->LightCubes[PE->LightCubes.size() - 1];
			PE->LightCubes.pop_back();

			CurrentRedactingParticleObjectType = -1;
			CurrentRedactingParticleObject = -1;
			return;
		}
	}
	else
	{

		CurrentRedactingParticleObject = -1;
		CurrentRedactingParticleObjectType = -1;
	}
}
void ShowRedactorWindow(ParticleEmiter* ParticleEmiter)
{
	float step = 20.0f;
	float xsize = 0.0f;
	Corner.y += UI_DrawText(CurrentParticleEmiter->Name, Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_CheckBox(&CurrentParticleEmiter->ShowWindow,"ShowSettings", Corner).y * -1.0f - step;


	int tip = 0;
	if (CurrentParticleEmiter->Type == "QUAD") tip = 0;
	if (CurrentParticleEmiter->Type == "LINE") tip = 1;
	if (CurrentParticleEmiter->Type == "CIRCLE") tip = 2;
	if (CurrentParticleEmiter->Type == "TEXTURED") tip = 3;
	if (CurrentParticleEmiter->Type == "TEXTUREDLINE") tip = 4;
	Corner.y += UI_SliderInt(&tip, "Type", Corner,0,4).y * -1.0f - step;
	if (tip == 0)CurrentParticleEmiter->Type = "QUAD";
	if (tip == 1)CurrentParticleEmiter->Type = "LINE";
	if (tip == 2)CurrentParticleEmiter->Type = "CIRCLE";
	if (tip == 3)CurrentParticleEmiter->Type = "TEXTURED";
	if (tip == 4)CurrentParticleEmiter->Type = "TEXTUREDLINE";
	Corner.y += UI_DrawText(CurrentParticleEmiter->Type, Corner, 0.35f).y * -1.0f - step;

	bool r = RedactingParticlesEmiter;
	Corner.y += UI_CheckBox(&RedactingParticlesEmiter, "Redact", Corner).y * -1.0f - step;

	if (r != RedactingParticlesEmiter)
	{
		GrabSelectTool = false;
		RedactingScene = false;
		RedactingPolygon = false;
	}

	if (RedactingParticlesEmiter)
	{

		Corner.y += UI_DrawText("object Type:", Corner, 0.35f).y * -1.0f - step;
		const char* str;
		if (ParticleObject == 0)str = "InfluenceSphere";
		if (ParticleObject == 1)str = "InfluenceQuad";
		if (ParticleObject == 2)str = "EmiterPoint";
		if (ParticleObject == 3)str = "EmiterSphere";
		if (ParticleObject == 4)str = "EmiterQuad";
		if (ParticleObject == 5)str = "LightSphere";
		if (ParticleObject == 6)str = "LightQuad";
		Corner.y += UI_SliderInt(&ParticleObject,str, Corner, 0, 6).y * -1.0f - step;
		if (ParticleObject > 1 && ParticleObject < 5)
		{
			Corner.y += UI_DragInt(&amount, "Amount", Corner, 0.1f).y * -1.0f - step;
			Corner.y += UI_Drag(&tick, "tick", Corner, 0.001f).y * -1.0f - step;

			Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&vel[0], "x", Corner + glm::vec2(xsize, 0.0f), 0.01f, {40.0f,15.0f}).x * 0.5f;
			Corner.y += UI_Drag(&vel[1], "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

		}

		Corner.y += UI_CheckBox(&AddRedact, "Add / Redact", Corner).y * -1.0f - step;
		if (AddRedact)
		{
			Corner.y += UI_DrawText("Hold Shift to add 1 object/click ", Corner, 0.35f).y * -1.0f - step;

			if (ParticleObject == 0)
			{

				Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&vel[0], "x", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&vel[1], "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_Drag(&radius, "Radius", Corner, 0.1f).y * -1.0f - step;
				Corner.y += UI_Drag(&AttractionForce, "AttractionForce", Corner, 0.1f).y * -1.0f - step;

				Corner.y += UI_CheckBox(&attracticve, "attracticve", Corner).y * -1.0f - step;
				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			if (ParticleObject == 1)
			{
				Corner.y += UI_DrawText("velocity", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&vel[0], "x", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&vel[1], "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_DrawText("Size", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&size[0], "x", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&size[1], "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_Drag(&AttractionForce, "AttractionForce", Corner, 0.1f).y * -1.0f - step;

				Corner.y += UI_CheckBox(&attracticve, "attracticve", Corner).y * -1.0f - step;
				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			if (ParticleObject == 2)
			{
				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			if (ParticleObject == 3)
			{
				Corner.y += UI_Drag(&radius, "Radius", Corner, 0.1f).y * -1.0f - step;
				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			if (ParticleObject == 4)
			{
				Corner.y += UI_DrawText("Size", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&size[0], "x", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&size[1], "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			if (ParticleObject == 5)
			{
				Corner.y += UI_Drag(&radius, "Radius", Corner, 0.1f).y * -1.0f - step;

				Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&LightColor[0], "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&LightColor[1], "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&LightColor[2], "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&LightColor[3], "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			if (ParticleObject == 6)
			{
				Corner.y += UI_DrawText("Size", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&size[0], "x", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&size[1], "y", Corner + glm::vec2(xsize, 0.0f), 1.0f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
				xsize = UI_Drag(&LightColor[0], "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&LightColor[1], "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				xsize += UI_Drag(&LightColor[2], "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
				Corner.y += UI_Drag(&LightColor[3], "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

				Corner.y += UI_DrawText("if holding Shift, drag mouse to change raduis", Corner, 0.35f).y * -1.0f - step;
			}
			
		}
		else
		{
			if (ParticleObject == 0)
			{
				Corner.y += UI_DrawText("SpheresOfInfluence:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->SpheresOfInfluence.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 0;
						CurrentRedactingParticleObject = i;
					}
				}
			}
			if (ParticleObject == 1)
			{
				Corner.y += UI_DrawText("CubeOfInfluence:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->CubesOfInfluence.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 1;
						CurrentRedactingParticleObject = i;
					}
				}
			}
			if (ParticleObject == 2)
			{
				Corner.y += UI_DrawText("EmitionPoints:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->EmitionPoints.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 2;
						CurrentRedactingParticleObject = i;
					}
				}
			}
			if (ParticleObject == 3)
			{
				Corner.y += UI_DrawText("EmitionCircles:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->EmitionCircles.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 3;
						CurrentRedactingParticleObject = i;
					}
				}
			}
			if (ParticleObject == 4)
			{
				Corner.y += UI_DrawText("EmitionCubes:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->EmitionCubes.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 4;
						CurrentRedactingParticleObject = i;
					}
				}
			}
			if (ParticleObject == 5)
			{
				Corner.y += UI_DrawText("LightSpheres:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->LightSpheres.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 5;
						CurrentRedactingParticleObject = i;
					}
				}
			}
			if (ParticleObject == 6)
			{
				Corner.y += UI_DrawText("LightCubes:", Corner, 0.35f).y * -1.0f - step;
				for (int i = 0; i < CurrentParticleEmiter->LightCubes.size(); i++)
				{
					bool pressed = false;
					Corner.y += UI_buttonOnlyON(&pressed, std::to_string(i).c_str(), Corner).y * -1.0f - step;
					if (pressed)
					{
						CurrentRedactingParticleObjectType = 6;
						CurrentRedactingParticleObject = i;
					}
				}
			}
		}
		

		
	}
	else
	{

		Corner.y += UI_DragInt(&CurrentParticleEmiter->id, "id", Corner, 0.1f).y * -1.0f - step;
		Corner.y += UI_DragInt(&CurrentParticleEmiter->Z_Index, "Z_Index", Corner, 0.1f).y * -1.0f - step;
		int idbuff = CurrentParticleEmiter->Textureid;
		Corner.y += UI_SliderInt(&CurrentParticleEmiter->Textureid, "TextureId", Corner, 0, Map.Textures.size() - 1).y * -1.0f - step;

		if (selectedTexture >= 0 && selectedTexture < Map.Textures.size())
		{
			Corner.y += UI_DrawText(Map.Textures[selectedTexture].FileName.c_str(), Corner, 0.35f).y * -1.0f - step;

			if (idbuff != CurrentParticleEmiter->Textureid)
				CurrentParticleEmiter->material.Texture = Map.Textures[CurrentParticleEmiter->Textureid].texture;
		}
	}
}

void ShowRedactorWindow(polygonData* Data)
{
	float step = 20.0f;
	float xsize = 0.0f;
	Corner.y += UI_DrawText("Mesh", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_TextBox(&SelectedMesh->Name, Corner).y * -1.0f - step;


	Corner.y += UI_DrawText("FileName", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_TextBox(&SelectedMesh->FilePath, Corner).y * -1.0f - step;


	bool b = false;
	Corner.y += UI_CheckBox(&b, "Load", Corner).y * -1.0f - step;
	if (b)
	{
		SelectedMesh->Load(SelectedMesh->FilePath);
		SelectedMesh->Update();
	}
	Corner.y += UI_DrawText(std::to_string(SelectedMesh->VAO), Corner, 0.35f).y * -1.0f - step;
	b = false;
	Corner.y += UI_CheckBox(&b, "Save", Corner).y * -1.0f - step;
	if (b)
		SelectedMesh->SaveAs(SelectedMesh->FilePath);


	b = false;
	Corner.y += UI_CheckBox(&b, "Redact", Corner).y * -1.0f - step;
	if (b)
		ShowPolygonTools = !ShowPolygonTools;

	b = false;
	Corner.y += UI_CheckBox(&b, "Points", Corner).y * -1.0f - step;
	if (b)
		ShowPolygonPositions = !ShowPolygonPositions;

	b = false;
	Corner.y += UI_CheckBox(&b, "TexturePositions", Corner).y * -1.0f - step;
	if (b)
		ShowPolygonTexturePositions = !ShowPolygonTexturePositions;

	if (ShowPolygonPositions)
	{
		for (int i = 0; i < SelectedMesh->Rawpoints.size(); i++)
		{
			Corner.y += UI_DrawText(std::to_string(i), Corner, 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&SelectedMesh->Rawpoints[i].x, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&SelectedMesh->Rawpoints[i].y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;
		}
	}
	if (ShowPolygonTexturePositions)
	{
		for (int i = 0; i < SelectedMesh->TexturePoints.size(); i++)
		{
			Corner.y += UI_DrawText(std::to_string(i), Corner, 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&SelectedMesh->TexturePoints[i].x, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&SelectedMesh->TexturePoints[i].y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;
		}
	}


}
void ShowRedactorWindow(polygon* Polygon)
{
	float step = 20.0f;
	float xsize = 0.0f;
	Corner.y += UI_DrawText("Polygon", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_SliderInt(&SelectedPolygon->id, "id", Corner).y * -1.0f - step;
	Corner.y += UI_SliderInt(&SelectedPolygon->MeshID, "MeshID", Corner,0,Map.polygonMeshes.size()-1).y * -1.0f - step;
	
	if(SelectedPolygon->MeshID < Map.polygonMeshes.size())
		Corner.y += UI_DrawText(Map.polygonMeshes[SelectedPolygon->MeshID].Name.c_str(), Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedPolygon->Textureid, "Texture", Corner,-1,Map.Textures.size()-1).y * -1.0f - step;
	if (SelectedPolygon->Textureid > -1)
		Corner.y += UI_DrawText(Map.Textures[SelectedPolygon->Textureid].FileName.c_str(), Corner, 0.35f).y * -1.0f - step;
	else
		Corner.y += UI_DrawText("Clear Color", Corner, 0.35f).y * -1.0f - step;
	

	Corner.y += UI_SliderInt(&SelectedPolygon->NormalMapId, "NormalMap", Corner,-1,Map.NormalMaps.size()-1).y * -1.0f - step;
	if (SelectedPolygon->NormalMapId > -1)
		Corner.y += UI_DrawText(Map.NormalMaps[SelectedPolygon->NormalMapId].FileName.c_str(), Corner, 0.35f).y * -1.0f - step;
	else
		Corner.y += UI_DrawText("Empty", Corner, 0.35f).y * -1.0f - step;

	


	Corner.y += UI_DrawText("Scale", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&SelectedPolygon->Scale.x, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedPolygon->Scale.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&SelectedPolygon->Position.x, "x", Corner, 0.1f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedPolygon->Position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.1f, { 70.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_DragInt(&SelectedPolygon->Z_Index, "Z_Index", Corner, 0.1f).y * -1.0f - step;
	Corner.y += UI_Drag(&SelectedPolygon->Rotation, "Rotation", Corner, 0.01f).y * -1.0f - step;
	SelectedPolygon->Update_Shape();
	Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&SelectedPolygon->color.r, "r", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&SelectedPolygon->color.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&SelectedPolygon->color.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedPolygon->color.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;






	return;
}

void ShowRedactorWindow(LightSource* ls)
{

	float step = 20.0f;
	Corner.y += UI_DrawText("Light source", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_DrawText(CurrentLightSource->name.c_str(), Corner, 0.35f).y * -1.0f - step;


	Corner.y += UI_DrawText("Scale", Corner, 0.35f).y * -1.0f - step;
	float xsize = UI_Drag(&CurrentLightSource->scale.x, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&CurrentLightSource->scale.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&CurrentLightSource->position.x, "x", Corner, 0.1f, { 70.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&CurrentLightSource->position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.1f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&CurrentLightSource->position.z, "z", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&CurrentLightSource->color.r, "r", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&CurrentLightSource->color.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&CurrentLightSource->color.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&CurrentLightSource->color.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_Drag(&CurrentLightSource->volume, "Volume", Corner, 0.01f, { 70.0f,15.0f }).y * -1.0f - step;

	if (CurrentLightSource->TextureId >= 0)
		Corner.y += UI_DrawText(Map.Textures[CurrentLightSource->TextureId].FileName.c_str(), Corner, 0.35f).y * -1.0f - step;
	else
		Corner.y += UI_DrawText("Light Sphere Texture", Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_SliderInt(&CurrentLightSource->TextureId, "Texture", Corner, -1, Map.Textures.size() - 1).y * -1.0f - step;
}
void ShowRedactorWindow(ball* Ball)
{
	float step = 10.0f;
	Corner.y += UI_DrawText("Ball",Corner,0.35f).y*-1.0f-step;
	Corner.y += UI_DragInt(&SelectedBall->id,"id",Corner, 0.1f).y*-1.0f-step;
	Corner.y += UI_SliderInt(&SelectedBall->type,"type",Corner, -1,1).y*-1.0f-step;
	Corner.y += UI_DragInt(&SelectedBall->Z_Index,"Z_Index",Corner, 0.1f).y*-1.0f-step;
	Corner.y += UI_Drag(&SelectedBall->r,"Radius",Corner).y*-1.0f-step;
	Corner.y += UI_Drag(&SelectedBall->rotation,"Rotation",Corner,0.01f).y*-1.0f-step;

	Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
	float xsize = UI_Drag(&SelectedBall->position.x, "x", Corner, 0.01f, {70.0f,15.0f}).x*0.5f;
	Corner.y += UI_Drag(&SelectedBall->position.y,"y",Corner + glm::vec2(xsize,0.0f),0.01f, { 70.0f,15.0f }).y*-1.0f-step;
	

	Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&SelectedBall->color.r, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&SelectedBall->color.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&SelectedBall->color.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedBall->color.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedBall->Textureid, "Texture", Corner, -1, Map.Textures.size() - 1).y * -1.0f - step;

	if (SelectedBall->Textureid > -1 && SelectedBall->Textureid < Map.Textures.size())
		Corner.y += UI_DrawText(Map.Textures[SelectedBall->Textureid].FileName, Corner, 0.35f).y * -1.0f - step;
	else if (SelectedBall->Textureid == -1)
		Corner.y += UI_DrawText("Clear color", Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedBall->Shaderid, "Shader", Corner, -1, Map.Shaders.size() - 1).y * -1.0f - step;

	if (SelectedBall->Shaderid > -1)
		Corner.y += UI_DrawText(Map.Shaders[SelectedBall->Shaderid].Name, Corner, 0.35f).y * -1.0f - step;
	if (SelectedBall->Shaderid == -1)
		Corner.y += UI_DrawText("No shader", Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedBall->NormalMapId, "NormalMap", Corner, -1, Map.NormalMaps.size() - 1).y * -1.0f - step;


	Corner.y += UI_CheckBox(&SelectedBall->lighted, "Lighted",Corner).y * -1.0f - step;
	return;
}
void ShowRedactorWindow(cube* Cube)
{
	float step = 10.0f;
	Corner.y += UI_DrawText("Cube", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_DragInt(&SelectedCube->id, "id", Corner, 0.1f).y * -1.0f - step;
	Corner.y += UI_SliderInt(&SelectedCube->type, "type", Corner, -1, 1).y * -1.0f - step;
	Corner.y += UI_DragInt(&SelectedCube->Z_Index, "Z_Index", Corner, 0.1f).y * -1.0f - step;
	//Corner.y += UI_Drag(&SelectedCube->Width, "Scale", Corner).y * -1.0f - step;

	Corner.y += UI_DrawText("Scale", Corner, 0.35f).y * -1.0f - step;
	float xsize = UI_Drag(&SelectedCube->width, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedCube->height, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;

	//Corner.y += UI_Drag(&SelectedCube->rotation, "Rotation", Corner, 0.01f).y * -1.0f - step;

	Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&SelectedCube->position.x, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedCube->position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;


	Corner.y += UI_DrawText("Color", Corner, 0.35f).y * -1.0f - step;
	xsize = UI_Drag(&SelectedCube->color.r, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&SelectedCube->color.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	xsize += UI_Drag(&SelectedCube->color.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedCube->color.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedCube->Textureid, "Texture", Corner, -1, Map.Textures.size() - 1).y * -1.0f - step;

	if (SelectedCube->Textureid > -1 && SelectedCube->Textureid < Map.Textures.size())
		Corner.y += UI_DrawText(Map.Textures[SelectedCube->Textureid].FileName, Corner, 0.35f).y * -1.0f - step;
	else if (SelectedCube->Textureid == -1)
		Corner.y += UI_DrawText("Clear color", Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedCube->Shaderid, "Shader", Corner, -1, Map.Shaders.size() - 1).y * -1.0f - step;

	if (SelectedCube->Shaderid > -1)
		Corner.y += UI_DrawText(Map.Shaders[SelectedCube->Shaderid].Name, Corner, 0.35f).y * -1.0f - step;
	if (SelectedCube->Shaderid == -1)
		Corner.y += UI_DrawText("No shader", Corner, 0.35f).y * -1.0f - step;

	Corner.y += UI_SliderInt(&SelectedCube->NormalMapId, "NormalMap", Corner, -1, Map.NormalMaps.size() - 1).y * -1.0f - step;


	Corner.y += UI_CheckBox(&SelectedCube->lighted, "Lighted", Corner).y * -1.0f - step;
	return;

}
void ShowRedactorWindow(Node* point)
{
	float step = 10.0f;
	Corner.y += UI_DrawText("point", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_DragInt(&SelectedPoint->id, "id", Corner, 0.1f).y * -1.0f - step;

	Corner.y += UI_DrawText("Position", Corner, 0.35f).y * -1.0f - step;
	float xsize = UI_Drag(&SelectedPoint->position.x, "x", Corner, 0.01f, { 70.0f,15.0f }).x * 0.5f;
	Corner.y += UI_Drag(&SelectedPoint->position.y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 70.0f,15.0f }).y * -1.0f - step;
}

int DeletetextureCountDown = 2;
void ShowRedactorWindow(Texture* Texture)
{


	float step = 20.0f;
	Corner.y += UI_DrawText("Texture", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_DrawText(CurrentTexture->FileName.c_str(), Corner, 0.35f).y * -1.0f - step;



	Corner.y += UI_TextBox(&CurrentTexture->FileName,Corner,-1).y * -1.0f - step;



	float xsize = 0;

	std::string tmp = "Texture ID: ";
	tmp += std::to_string(CurrentTexture->texture);
	Corner.y += UI_DrawText(tmp, Corner, 0.35f).y * -1.0f - step;

	int typ = CurrentTexture->Type;

	Corner.y += UI_SliderInt(&CurrentTexture->Type, "type", Corner, 0, 7).y * -1.0f - step;

	if (CurrentTexture->Type == 0)
		Corner.y += UI_DrawText("LoadFromName",Corner,0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 1)
		Corner.y += UI_DrawText("Round Noize",Corner,0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 2)
		Corner.y += UI_DrawText("Squere Noize",Corner,0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 3)
		Corner.y += UI_DrawText("Smooth Squere Noize",Corner,0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 4)
		Corner.y += UI_DrawText("Gradient", Corner, 0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 5)
		Corner.y += UI_DrawText("Gradient Sphere", Corner, 0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 6)
		Corner.y += UI_DrawText("Sphere", Corner, 0.35f).y * -1.0f - step;
	if (CurrentTexture->Type == 7)
		Corner.y += UI_DrawText("Quad", Corner, 0.35f).y * -1.0f - step;

	std::string filter = "Linear/blurry";
	if (CurrentTexture->filter)
		filter = "Nearest/pixelart";
	bool filt = CurrentTexture->filter;
	Corner.y += UI_CheckBox(&CurrentTexture->filter,filter.c_str(), Corner,18.0f, 0.35f).y * -1.0f - step;

	if (filt != CurrentTexture->filter)
	{

		CurrentTexture->Delete();
		CurrentTexture->Load();
	}


	if (typ != CurrentTexture->Type)
	{
		CurrentTexture->Delete();
		CurrentTexture->Load();
	}


	if (CurrentTexture->Type > 0 && CurrentTexture->Type < 4)
	{
		float Tex_Freq = CurrentTexture->Noize_Frequency;
		Corner.y += UI_Drag(&CurrentTexture->Noize_Frequency,"Frequency ",Corner, 0.01f).y * -1.0f - step;
		if (Tex_Freq != CurrentTexture->Noize_Frequency)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}
		int NoizeLayers = CurrentTexture->Noize_Layers;

		Corner.y += UI_DragInt(&CurrentTexture->Noize_Layers, "Layers ", Corner, 0.01f).y * -1.0f - step;

		if (NoizeLayers != CurrentTexture->Noize_Layers)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}
		float Tex_Size = CurrentTexture->Size;
		Corner.y += UI_Drag(&CurrentTexture->Size, "Size ", Corner, 0.1f).y * -1.0f - step;
		if (Tex_Size != CurrentTexture->Size)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}
	}
	else if (CurrentTexture->Type == 4)
	{
		glm::vec4 color = CurrentTexture->Gradient_Color1;


		Corner.y += UI_DrawText("Gradient Color1", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&CurrentTexture->Gradient_Color1.r, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&CurrentTexture->Gradient_Color1.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&CurrentTexture->Gradient_Color1.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&CurrentTexture->Gradient_Color1.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

		if (color != CurrentTexture->Gradient_Color1)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}

		color = CurrentTexture->Gradient_Color2;

		Corner.y += UI_DrawText("Gradient Color2", Corner, 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&CurrentTexture->Gradient_Color2.r, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&CurrentTexture->Gradient_Color2.g, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&CurrentTexture->Gradient_Color2.b, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&CurrentTexture->Gradient_Color2.a, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;

		if (color != CurrentTexture->Gradient_Color2)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}

		float Tex_Size = CurrentTexture->Size;
		Corner.y += UI_Drag(&CurrentTexture->Size, "Size ", Corner, 0.1f).y * -1.0f - step;

		if (Tex_Size != CurrentTexture->Size)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}

	}
	else
	{
		
		float Tex_Size = CurrentTexture->Size;
		Corner.y += UI_Drag(&CurrentTexture->Size, "Size ", Corner, 0.1f).y * -1.0f - step;

		if (Tex_Size != CurrentTexture->Size)
		{
			CurrentTexture->Delete();
			CurrentTexture->Load();
		}

	}
	bool del = false;
	xsize = UI_buttonOnlyON(&del,"DeleteTexture", Corner).x * 0.5f;
	if(del)
		DeletetextureCountDown--;

	Corner.y += UI_DrawText(std::to_string(DeletetextureCountDown),Corner+glm::vec2(xsize,0.0f),0.35f).y*-1.0f - step;
	if (DeletetextureCountDown <= 0)
	{
		DeletetextureCountDown = 2;
		CurrentTexture->Delete();
		int i = 0;
		bool found = false;
		while (!found && i < Map.Textures.size())
		{
			i++;
			if (Map.Textures[i].id == CurrentTexture->id)
				found = true;
		}
		Map.Textures[i] = Map.Textures[Map.Textures.size() - 1];
		Map.Textures.pop_back();
		CurrentTexture = NULL;
		return;
	}

}
void ShowRedactorWindow(Shader* shader)
{
	float step = 20.0f;
	Corner.y += UI_DrawText("Shader", Corner, 0.35f).y * -1.0f - step;

	std::string tmp = "Program ID: ";
	tmp += std::to_string(CurrentShader->program);
	Corner.y += UI_DrawText(tmp, Corner, 0.35f).y * -1.0f - step;


	Corner.y += UI_DrawText("Name: ", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_TextBox(&CurrentShader->Name, Corner, -1).y * -1.0f - step;


	Corner.y += UI_DrawText("Vertex path: ", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_TextBox(&CurrentShader->VertexPath, Corner, -1).y * -1.0f - step;


	Corner.y += UI_DrawText("Fragment path: ", Corner, 0.35f).y * -1.0f - step;
	Corner.y += UI_TextBox(&CurrentShader->FragmentPath, Corner, -1).y * -1.0f - step;

	bool b = false;
	Corner.y += UI_buttonOnlyON(&b,"Load", Corner).y * -1.0f - step;

	if (b)
	{
		CurrentShader->ClearUniforms();
		CurrentShader->Load();
		CurrentShader->GetUniforms();
	}
	for (int i = 0; i < CurrentShader->Uniforms.size(); i++)
	{

		float xsize = 0.0f;

		if (CurrentShader->Uniforms[i].type == 0)
			Corner.y += UI_Drag(&CurrentShader->uniformfloat[CurrentShader->Uniforms[i].type_id], CurrentShader->Uniforms[i].name.c_str(), Corner, 0.01f).y * -1.0f - step;
		if (CurrentShader->Uniforms[i].type == 1)
			Corner.y += UI_DragInt(&CurrentShader->uniformint[CurrentShader->Uniforms[i].type_id], CurrentShader->Uniforms[i].name.c_str(), Corner, 0.01f).y * -1.0f - step;
		if (CurrentShader->Uniforms[i].type == 2)
		{

			Corner.y += UI_DrawText(CurrentShader->Uniforms[i].name.c_str(), Corner, 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&CurrentShader->uniformvec2[CurrentShader->Uniforms[i].type_id].x, "x", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&CurrentShader->uniformvec2[CurrentShader->Uniforms[i].type_id].y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		}
		if (CurrentShader->Uniforms[i].type == 3)
		{

			Corner.y += UI_DrawText(CurrentShader->Uniforms[i].name.c_str(), Corner, 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&CurrentShader->uniformvec3[CurrentShader->Uniforms[i].type_id].x, "x", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&CurrentShader->uniformvec3[CurrentShader->Uniforms[i].type_id].y, "y", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&CurrentShader->uniformvec3[CurrentShader->Uniforms[i].type_id].z, "z", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		}
		if (CurrentShader->Uniforms[i].type == 4)
		{

			Corner.y += UI_DrawText(CurrentShader->Uniforms[i].name.c_str(), Corner, 0.35f).y * -1.0f - step;
			xsize = UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].x, "r", Corner, 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].y, "g", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			xsize += UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].z, "b", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
			Corner.y += UI_Drag(&CurrentShader->uniformvec4[CurrentShader->Uniforms[i].type_id].w, "a", Corner + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		}
	}



	ImGui::End();
}

*/

int KastylID;

float SceneWindowScroll = 0.0f;
float InspectorWindowScroll = 0.0f;
float ProjectWindowScroll = 0.0f;
float ConsoleWindowScroll = 0.0f;

float SceneWindowMaxScroll = 0.0f;
float InspectorWindowMaxScroll = 0.0f;
float ProjectWindowMaxScroll = 0.0f;
float ConsoleWindowMaxScroll = 0.0f;


bool Test[10];
bool sTestb[10];
int sTesti[10];
int TestWindowID = -1;

Shader sh;

int ProjectWindowSelection = 0;// [0]-nodes, [1]-settings;
int AssetWindowSelection = 0;// [0]-assets, [1]-EventGraph, [2]-animation; 
int SceneWindowSelection = 0;// [0]-View, [1]-NormalMaps, [2]-Lightingmap, [3]-Collisionmap; 
int InspectorWindowSelection = 0;// placeholder 

std::string ProjectWindowSelectionNames[2] = {"Nodes","Settings"};// [0]-nodes, [1]-settings;
std::string AssetWindowSelectionNames[3]= {"Assets","EventGraph","Animation"};// [0]-assets, [1]-EventGraph, [2]-animation; 
std::string InspectorWindowSelectionNames[3]= {"Properties","none","none"};// placeholder 


glm::vec2 PrevMousePos = glm::vec2(0.0f);
bool MMBJustPressedWindow[4];

bool grabbedWindow[3];// bools for window resizing
bool grabbedAnyWindow = false;// bools for window resizing
glm::vec2 GrabStartMousePos = { 0.0f,0.0f };
bool initialsizecalc = true;

// Undo implementation:
// any action == +new item in list, clear all steps forward (no redo options)
// undo == load map 1 step before
// redo == load map 1 step above if possible
// on undo/redo, save();load();
// Normal implementation would be a text comparison like in github, because there is already a DataStorage, which is a txt save. 
std::vector<DataStorage> UndoLine;
bool ActionDoneThisFrame = false;


void ProcessScene(Scene* scn,bool mt,bool mainScene)
{
	GameScene = scn;
	scn->dt = delta * Simulation_speed /substeps;
	
	
	//listenerVel = { Entities[0]->CP.midvel.x ,Entities[0]->CP.midvel.y ,1.0f };
	listenerPos.z = 1.0f / (CameraScale.x);
	std::vector <int> iter;
	iter.resize(threadcount);
	
	
	
	for (int i = 0; i < threadcount; i++)
		iter[i] = i;

	scn->Update();
	threadNodestep = scn->Nodes.size()/threadcount;
	threadNodeEnd = scn->Nodes.size();
	threadsprepass = true;
	
	if (scn->Nodes.size() > threadcount && mt)
	{
		//for(auto thr : iter)
		//{
		//	_ScenethreadsStates[thr] = 0;
		//	std::unique_lock<std::mutex> lm(_SceneMutexes[thr]);
		//	_SceneConVars[thr].notify_one();
		//}
		//bool wait = true;
		//float startWaittime = glfwGetTime(); 
		//
		//while(wait)
		//{
		//	wait = false;
		//	for(int thr = 0;thr<threadcount;thr++)
		//	{
		//		if(!_ScenethreadsStates[thr].load())
		//			wait = true;
		//	}
		//	if(glfwGetTime() - startWaittime  > delta*3.0f) // something happend with threads
		//	{
		//		std::cout<<"something happend with threads\n";
		//		for(int thr = 0;thr<threadcount;thr++)
		//		{						
		//			_SceneConVars[thr].notify_one();
		//			_ScenethreadsStates[thr].store(1);
		//		}
		//		break;
		//	}	
		//}
		std::vector<int> threadints;
		threadints.resize(threadcount);
		for (int i = 0; i < threadcount; i++)
			threadints[i] = i;

		std::for_each(std::execution::par_unseq,threadints.begin(), threadints.end(), [](int thr)
			{
				int step = threadNodestep;
				int begin = thr * step;
				int end = (thr + 1) * step;
				if (thr == 0)
					begin = 0;

				if (thr == threadcount - 1)
					end = threadNodeEnd;

				for (int i = begin; i < end; i++)
				{
					if (threadsprepass)
						GameScene->Nodes[i]->MTPreProcess();
					else if (!Paused && Running)
						GameScene->Nodes[i]->MTProcess(GameScene->dt);
				}
			}
		);
	
	}
	else
	{
		int buf = threadcount;
		threadcount=1;
		_mt_SceneProcess(0);
		threadcount = buf;

	}
	threadsprepass = false;
	for(int s=0;s<substeps ;s++)
	{

		if (scn->Nodes.size() > threadcount && mt)
		{
			//for(auto thr : iter)
			//{
			//	_ScenethreadsStates[thr].store(0);
			//	std::unique_lock<std::mutex> lm(_SceneMutexes[thr]);
			//	_SceneConVars[thr].notify_one();
			//}
			//bool wait = true;
			//float startWaittime = glfwGetTime(); 
			//while(wait)
			//{
			//	wait = false;
			//	for(int thr = 0;thr<threadcount;thr++)
			//	{
			//		if(!_ScenethreadsStates[thr].load())
			//			wait = true;
			//	}
			//	if(glfwGetTime() - startWaittime > delta*3.0f) // something happend with threads
			//	{
			//		std::cout<<"\nsomething happend with threads";
			//		for(int thr = 0;thr<threadcount;thr++)
			//		{						
			//			_SceneConVars[thr].notify_one();
			//			_ScenethreadsStates[thr].store(1);
			//		}
			//		break;
			//	}	
			//}
			std::vector<int> threadints;
			threadints.resize(threadcount);
			for (int i = 0; i < threadcount; i++)
				threadints[i] = i;

			std::for_each(std::execution::par_unseq,threadints.begin(), threadints.end(), [](int thr)
				{
					int step = threadNodestep;
					int begin = thr * step;
					int end = (thr + 1) * step;
					if (thr == 0)
						begin = 0;

					if (thr == threadcount - 1)
						end = threadNodeEnd;

					for (int i = begin; i < end; i++)
					{
						if (threadsprepass)
							GameScene->Nodes[i]->MTPreProcess();
						else if (!Paused && Running)
							GameScene->Nodes[i]->MTProcess(GameScene->dt);
					}
				}
			);
		}
		else
		{
			int buf = threadcount;
			threadcount=1;
			_mt_SceneProcess(0);
			threadcount = buf;

		}

		if(!Paused && Running)
		{
			scn->Process(delta * Simulation_speed / substeps);
			if(mainScene)SubSteppedProcess(delta * Simulation_speed / substeps, s);
		}
	}
	if(!Paused && Running &&mainScene)
	{
		SelectedNode = NULL;
		SelectedNodeID = -1;
		SelectedAsset =NULL;
		SelectedAssetID=-1;
		Process(delta * Simulation_speed);
	}
	if(Paused || !Running)
	{
		if(mainScene)UpdateListenerPosition();
		scn->Draw(delta * Simulation_speed);
	}
	
	GameScene = &Map;
}



void On_Create()
{
	
	std::cout<<"On_Create\n";
	_FillVectorsOfNodesAndAssets();

	AL_init();
	sTesti[0] = 0;
	Map.LoadAssets = true;
	std::cout<<"On_Create loading map\n";
	Map.LoadFrom(MapFileName);
	std::cout<<"On_Create loaded map\n";

	//SetShader2f(&sh.program, "position",MousePosition)

	//DefaultTriangle.add_Point(PrevMousePosition - glm::vec2(5, 5 * 0.5f), false);
	//DefaultTriangle.add_Point(PrevMousePosition - glm::vec2(-5, 5 * 0.5f), false);
	//DefaultTriangle.add_Point(PrevMousePosition + glm::vec2(0, 5), false);
	//DefaultTriangle.indexes.push_back(glm::ivec3(0, 1, 2));
	//DefaultTriangle.Update();
	//Map.polygonMeshes.push_back(DefaultTriangle);
	GetWindow(0)->backgroundColor = EditorColor;



	SceneWindowID = CreateWindow();
	Window* w = GetWindow(SceneWindowID);
	w->Init({ 1280,720 });
	w->Position = { 0.0f,w->ViewportSize.y * 0.2f };

	InspectorWindowID = CreateWindow();
	Window* iw = GetWindow(InspectorWindowID);
	iw->Init({ ((WIDTH - 1280) * 0.5f),HEIGHT });
	iw->Position = { w->ViewportSize.x * 0.5f + iw->ViewportSize.x * 0.5f,0.0f };

	ProjectWindowID = CreateWindow();
	Window* pw = GetWindow(ProjectWindowID);
	pw->Init({ ((WIDTH - 1280) * 0.5f),HEIGHT });
	pw->Position = { -w->ViewportSize.x * 0.5f - pw->ViewportSize.x * 0.5f,0.0f };

	ConsoleWindowID = CreateWindow();
	Window* cw = GetWindow(ConsoleWindowID);
	cw->Init({ 1280,HEIGHT - 720 });
	cw->Position = { 0.0f,w->Position.y - w->ViewportSize.y * 0.5f };


	TestWindowID = CreateWindow();
	Window* tw = GetWindow(TestWindowID);
	tw->Init({ 300,300 });
	tw->Position = { 0.0f,w->Position.y - w->ViewportSize.y * 0.5f };


	
	KastylID = CreateWindow();
	Window* kw = GetWindow(KastylID);
	kw->Init({ 300,300 });
	kw->Position = MousePosition;
	kw->AutoDraw = false;
	kw->Autoclear = false;

	

	w->RecalculateSize();

	std::cout << "On_Create windows created\n";


	


	//Map.Nodes.push_back(new Node());
	//Map.Nodes.push_back(new CollisionObject());
	//Map.Nodes.push_back(new CO_Ball());
	//Map.Nodes.push_back(new CO_Cube());
	//Map.Nodes.push_back(new LightSourceObject());
	//Map.Assets.push_back(new TextureObject());
	//Map.Assets.push_back(new MaterialObject());

	w->Use();
	
	GameScene = &Map;
	
	Ready();
	w->End();
	std::cout << "On_Create'd\n";
	
	//_StartScenethreads();
}



void On_Update()
{
	ProcessAL();


	Window* w = GetWindow(SceneWindowID);
	Window* iw = GetWindow(InspectorWindowID);
	Window* pw = GetWindow(ProjectWindowID);
	Window* cw = GetWindow(ConsoleWindowID);
	Window* tw = GetWindow(TestWindowID);
	Window* kw = GetWindow(KastylID);

	/*kw->Use();
	DrawShaderedQuad({ 0.0f ,0.0f}, { 100,100 }, 0.0f, sh.program);
	kw->End();*/

	if (!grabbedAnyWindow)
		GrabStartMousePos = MousePosition;

	if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
	{
		if (MousePosition.x > WIDTH * 0.5f - iw->ViewportSize.x - 3 && MousePosition.x < WIDTH * 0.5f - iw->ViewportSize.x + 3)
		{
			grabbedWindow[1] = true;
			grabbedAnyWindow = true;
		}
		if ((MousePosition.x > pw->ViewportSize.x - 3 - WIDTH * 0.5f) && (MousePosition.x < pw->ViewportSize.x + 3 - WIDTH * 0.5f))
		{
			grabbedWindow[0] = true;
			grabbedAnyWindow = true;
		}
		if ((MousePosition.y > cw->ViewportSize.y - 3 - HEIGHT * 0.5f) && (MousePosition.y < cw->ViewportSize.y + 3 - HEIGHT * 0.5f))
		{
			grabbedWindow[2] = true;
			grabbedAnyWindow = true;
		}
		GrabStartMousePos = MousePosition;
	}

	if (grabbedWindow[0])//project window /// left
	{
		pw->Scale.x = 1.0f + (MousePosition.x - GrabStartMousePos.x) / pw->ViewportSize.x;
		if (pw->Scale.x * pw->ViewportSize.x < 25) pw->Scale.x = 25 / pw->ViewportSize.x;
		if (pw->Scale.x * pw->ViewportSize.x > WIDTH - 100 - iw->ViewportSize.x) pw->Scale.x = (WIDTH - 100 - iw->ViewportSize.x) / pw->ViewportSize.x;
	}
	if (grabbedWindow[1])//inspector window /// right
	{
		iw->Scale.x = 1.0f + (GrabStartMousePos.x - MousePosition.x) / iw->ViewportSize.x;
		if (iw->Scale.x * iw->ViewportSize.x < 25) iw->Scale.x = 25 / iw->ViewportSize.x;
		if (iw->Scale.x * iw->ViewportSize.x > WIDTH - 100 - pw->ViewportSize.x) iw->Scale.x = (WIDTH - 100 - pw->ViewportSize.x) / iw->ViewportSize.x;
	}
	if (grabbedWindow[2])//inspector window /// right
	{
		cw->Scale.y = 1.0f + (MousePosition.y - GrabStartMousePos.y) / cw->ViewportSize.y;
		if (cw->Scale.y * cw->ViewportSize.y < 25) cw->Scale.y = 25 / cw->ViewportSize.y;
		if (cw->Scale.y * cw->ViewportSize.y > HEIGHT - 100) cw->Scale.y = (HEIGHT - 100) / cw->ViewportSize.y;
	}


	float rightx = pw->ViewportSize.x * pw->Scale.x - WIDTH * 0.5f + 2.0f;
	float leftx = WIDTH * 0.5f - iw->ViewportSize.x * iw->Scale.x - 2.0f;

	float maxy = HEIGHT * 0.5f;
	float miny = cw->ViewportSize.y * cw->Scale.y - HEIGHT * 0.5f + 2.0f;
	if (grabbedAnyWindow || initialsizecalc)
	{
		cw->Scale.x = (leftx - rightx) / cw->ViewportSize.x;

		w->Scale.x = (leftx - rightx) / w->ViewportSize.x;
		w->Scale.y = (maxy - miny - 25.0f) / w->ViewportSize.y ;
	}
	if ( grabbedAnyWindow || initialsizecalc)
	{

		w->ViewportSize *= w->Scale;
		iw->ViewportSize *= iw->Scale;
		pw->ViewportSize *= pw->Scale;
		cw->ViewportSize *= cw->Scale;

		w->Scale = { 1.0f,1.0f };
		iw->Scale = { 1.0f,1.0f };
		pw->Scale = { 1.0f,1.0f };
		cw->Scale = { 1.0f,1.0f };





		iw->RecalculateSize();
		pw->RecalculateSize();
		cw->RecalculateSize();
		w->RecalculateSize();
		Rescale(w->ViewportSize.x,w->ViewportSize.y);
		/*
		std::cout << "w" << w->ViewportSize.x << "  " << w->ViewportSize.y<<"\n";
		std::cout << "cw" << cw->ViewportSize.x << "  " << cw->ViewportSize.y<<"\n";
		std::cout << "iw" << iw->ViewportSize.x << "  " << iw->ViewportSize.y<<"\n";
		std::cout << "pw" << pw->ViewportSize.x << "  " << pw->ViewportSize.y<<"\n";*/
		initialsizecalc = false;
		GrabStartMousePos = MousePosition;
	}
	if(JustReleasedbutton[GLFW_MOUSE_BUTTON_1])
	{
		for (int i = 0; i < 3; i++)
			grabbedWindow[i] = false;
		grabbedAnyWindow = false;
	}

	//CountourSize


	iw->Position = glm::vec2(WIDTH * 0.5f - iw->ViewportSize.x * iw->Scale.x * 0.5f + 2.0f, 0.0f);
	iw->Draw(1001);

	pw->Position = glm::vec2(pw->ViewportSize.x * pw->Scale.x * 0.5f - WIDTH * 0.5f - 2.0f, 0.0f);
	pw->Draw(1002);

	cw->Position = glm::vec2((rightx + leftx) * 0.5f, cw->ViewportSize.y * cw->Scale.y * 0.5f - HEIGHT * 0.5f - 2.0f);
	cw->Draw(1003);

	w->Position.x = (rightx + leftx) * 0.5f;
	w->Position.y = (maxy + miny) * 0.5f - 25.0f;

	switch (SceneWindowSelection)
	{
	case 0:
		{
			UI_DrawTexturedQuad(w->Position, w->GetSize(), w->Texture, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1000, false,false,false,true);
		}break;
	case 1:
		{
			CopyTexture(w->ViewportSize,&NormalMapScreen,w->NormalMapColorBuffer,1);
			UI_DrawTexturedQuad(w->Position, w->GetSize(), NormalMapScreen, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1000, false,false,false,true);
		}break;
	case 2:
		{
			UI_DrawTexturedQuad(w->Position, w->GetSize(), w->LightColorBuffer, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1000, false,false,false,true);
		}break;
	case 3:
		{
			UI_DrawTexturedQuad(w->Position, w->GetSize(), w->Texture, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1000, false,false,false,true);
		}break;
	case 4:
		{
			CopyTexture(w->ViewportSize,&HeightMapScreen,w->NormalMapColorBuffer,5);
			UI_DrawTexturedQuad(w->Position, w->GetSize(), HeightMapScreen, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1000, false,false,false,true);
		}break;
	
	default:
		break;
	}

	/*
	kw->Position = MousePosition;
	kw->Scale = {1.0f,1.0f};
	kw->Draw(10000);*/

	//tw->Position = MousePosition;
	//tw->Draw(1005);

	float step = 20.0f;

	float xstep = 0.0f;
	float bSizeX = (pw->ViewportSize.x - step * 3.0f) / 2.0f;// 2 buttons = 3 steps  s bbb s bbb s
	bool b = false;
	GetWindow(ConsoleWindowID)->Use();

	ConsoleWindowScroll += scrollmovement * 50.0f;
	CameraPosition.y += (ConsoleWindowScroll - CameraPosition.y) * 0.25f;

	glm::vec2 dif = glm::vec2(0.0f);
	if (buttons[GLFW_MOUSE_BUTTON_MIDDLE] && !JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE] && MMBJustPressedWindow[0])
		dif = PrevMousePos - MousePosition;
	CameraPosition += dif.y;
	ConsoleWindowScroll += dif.y;
	MousePosition += dif.y;
	if(cw->active)
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE])
		{
			PrevMousePos = MousePosition;
			MMBJustPressedWindow[0]=true;
			MMBJustPressedWindow[1]=false;
			MMBJustPressedWindow[2]=false;
			MMBJustPressedWindow[3]=false;
		}

	Corner = { WIDTH * -0.5f , HEIGHT * 0.5f - CameraPosition.y };
	Corner += glm::vec2(20.0f, -25.0f);

	xstep = 0.0f;
	bSizeX = (cw->ViewportSize.x - step * 4.0f) / 3.0f;// 3 buttons = 4 steps  s bbb s bbb s bbb s
	if(bSizeX>150.0f) bSizeX = 150.0f;
	if(bSizeX<50.0f) bSizeX = 50.0f;
	b=AssetWindowSelection == 0;
	xstep += UI_button(&b,"Assets", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		AssetWindowSelection =0;
	b=AssetWindowSelection == 1;
	xstep += UI_button(&b,"Event Graph", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		AssetWindowSelection =1;
	b=AssetWindowSelection == 2;
	Corner.y += UI_button(&b,"Animation", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).y * -1.0f - step;
	if (b)
		AssetWindowSelection =2;


	xstep = 0.0f;
	switch (AssetWindowSelection)
	{
	case 0:
		{
		float size = 100;
		int AssetStep = 20.0f;
		int MaxAmountRow = cw->ViewportSize.x/(size + AssetStep * 2.0f)-1; 	
		int counterX = 0;
		float AssetstepX = 0.0f;


		AssetstepX +=UI_SliderInt(&NewAssetId,"Asset id",Corner + glm::vec2(AssetstepX,0.0f),1,AssetConstructorNames.size()-1).x *0.5f + AssetStep;
		int newassetidmapped = -1;
		int id= 0;
		for(auto x : AssetConstructorNames)
		{
			if(id == NewAssetId)
			{
				newassetidmapped = x.first;
				break;
			}
			id++;
		}
		AssetstepX+= UI_DrawText(AssetConstructorNames[newassetidmapped], Corner + glm::vec2(AssetstepX,0.0f), 0.35f).x + AssetStep;

		b = false;
		Corner.y += UI_button(&b,"Create", Corner + glm::vec2(AssetstepX,0.0f)).y *-1.0f - AssetStep*2.0f;
		if(b)
		{
			Map.Assets.push_back(AssetConstructors[newassetidmapped]());
		}

		AssetstepX = 0.0f;
		for(int i=0;i<Map.Assets.size();i++)
		{
			
			if(counterX>=MaxAmountRow)
			{
				AssetstepX =0.0f;
				counterX = 0;
			}
			counterX++;
			b = false;
			
			Map.Assets[i]->DrawPreview(Corner + glm::vec2(AssetstepX + size*0.5f + 10.0f,0.0f),{size*0.5f,size*0.5f});
			glm::vec2 UIObjSize = UI_button(&b, "", Corner + glm::vec2(AssetstepX,0.0f),{size+20.0f,size+20.0f},0.35f,glm::vec4(0.9f),glm::vec4(0.1f),glm::vec4(0.0f));
			UI_DrawText(Map.Assets[i]->Name.c_str(), Corner + glm::vec2(AssetstepX,UIObjSize.y*-0.5f - step*0.5f), 0.35f).x + AssetStep;
			//

			if(counterX<MaxAmountRow)
				AssetstepX += UIObjSize.x + AssetStep;
			else
				Corner.y += UIObjSize.y *-1.0f - step;
			if(b)
			{
				SelectedNode = NULL;
				SelectedNodeID = -1;
				SelectedAsset = Map.Assets[i];
				SelectedAssetID=i;
			}
			
		}
		}
		break;
	
	default:
		break;
	}

	

	// if (CurrentRedactingParticleObjectType > -1 && CurrentRedactingParticleObject > -1)
	// 	ShowParticleObjectRedactorWindow(CurrentParticleEmiter, CurrentRedactingParticleObjectType, CurrentRedactingParticleObject);


	
	b = false;
	


	if (HEIGHT * 0.5f < abs(Corner.y + CameraPosition.y))
	{
		if (ConsoleWindowScroll > 0.0f)
			ConsoleWindowScroll = 0.0f;
		else if (ConsoleWindowScroll < (Corner.y + CameraPosition.y))
			ConsoleWindowScroll = (Corner.y + CameraPosition.y);
	}
	else
		ConsoleWindowScroll = 0.0f;


	GetWindow(ConsoleWindowID)->End();


	GetWindow(InspectorWindowID)->Use();


	InspectorWindowScroll += scrollmovement * 50.0f;
	CameraPosition.y += (InspectorWindowScroll - CameraPosition.y) * 0.25f;

	
	dif = glm::vec2(0.0f);
	if (buttons[GLFW_MOUSE_BUTTON_MIDDLE]&& !JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE] && MMBJustPressedWindow[1])
		dif = PrevMousePos - MousePosition;
	CameraPosition += dif.y;
	InspectorWindowScroll += dif.y;
	MousePosition += dif.y;
	if(iw->active)
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE])
		{
			PrevMousePos = MousePosition;
			MMBJustPressedWindow[0]=false;
			MMBJustPressedWindow[1]=true;
			MMBJustPressedWindow[2]=false;
			MMBJustPressedWindow[3]=false;
		}
	
	Corner = { WIDTH * -0.5f, HEIGHT * 0.5f - CameraPosition.y };
	Corner += glm::vec2(20.0f, -25.0f);
	if(!Paused && Running)
	{
		SelectedNode = NULL;
		SelectedNodeID = -1;
		SelectedAsset =NULL;
		SelectedAssetID=-1;
	}
	if(SelectedAsset!=NULL || SelectedNode!=NULL)
		ProcessSelectedNodeUI();

	if (HEIGHT * 0.5f < abs(Corner.y + CameraPosition.y))
	{
		if (InspectorWindowScroll > 0.0f)
			InspectorWindowScroll = 0.0f;
		else if (InspectorWindowScroll < (Corner.y + CameraPosition.y))
			InspectorWindowScroll = (Corner.y + CameraPosition.y);
	}
	else
		InspectorWindowScroll = 0.0f;

	GetWindow(InspectorWindowID)->End();




	GetWindow(ProjectWindowID)->Use();




	ProjectWindowScroll += scrollmovement * 50.0f;
	CameraPosition.y += (ProjectWindowScroll - CameraPosition.y) * 0.25f;

	Corner = { WIDTH * -0.5f, HEIGHT * 0.5f - CameraPosition.y };
	Corner += glm::vec2(20.0f, -25.0f);

	
	dif = glm::vec2(0.0f);
	if (buttons[GLFW_MOUSE_BUTTON_MIDDLE]&& !JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE] && MMBJustPressedWindow[2])
		dif = PrevMousePos - MousePosition;
	CameraPosition += dif.y;
	ProjectWindowScroll += dif.y;
	MousePosition += dif.y;
	if(pw->active)
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE])
		{
			PrevMousePos = MousePosition;
			MMBJustPressedWindow[0]=false;
			MMBJustPressedWindow[1]=false;
			MMBJustPressedWindow[2]=true;
			MMBJustPressedWindow[3]=false;
		}

	//if (Test[0])
	//	DirectionalLight = 1.0f;
	//else
	//	DirectionalLight = 0.0f;



	//Corner.y += UI_Drag(&bloomLevels[0],"bloom 0", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[1],"bloom 1", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[2],"bloom 2", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[3],"bloom 3", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[4],"bloom 4", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[5],"bloom 5", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[6],"bloom 6", Corner, 0.35f).y * -1.0f - step;
	//Corner.y += UI_Drag(&bloomLevels[7],"bloom 7", Corner, 0.35f).y * -1.0f - step;
	xstep = 0.0f;
	bSizeX = (pw->ViewportSize.x - step * 3.0f) / 2.0f;// 2 buttons = 3 steps  s bbb s bbb s
	if(bSizeX>150.0f) bSizeX = 150.0f;
	if(bSizeX<50.0f) bSizeX = 50.0f;
	b=ProjectWindowSelection == 0;
	xstep += UI_button(&b,"Nodes", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		ProjectWindowSelection =0;
	b=ProjectWindowSelection == 1;
	Corner.y += UI_button(&b,"Settings", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).y * -1.0f - step;
	if (b)
		ProjectWindowSelection =1;


	xstep = 0.0f;
	bool cleanSelection = false;
	switch (ProjectWindowSelection)
	{
	case 0:
		{
			//Add stuff Tool
			Corner.y += UI_SliderInt(&NewObjectId,"Spawn id",Corner,0,NodeConstructorNames.size()-1).y * -1.0f - step;
			int newobjectidmapped = -1;
			int id= 0;
			for(auto x : NodeConstructorNames)
			{
				if(id == NewObjectId)
				{
					newobjectidmapped = x.first;
					break;
				}
				id++;
			}

			// if (NewObjectId == NodeType::NODE)Corner.y +=  UI_DrawText("NODE", Corner, 0.35f).y * -1.0f- step;
			// else if (newobjectidmapped == NodeType::OBJECT)Corner.y += UI_DrawText("OBJECT", Corner, 0.35f).y * -1.0f- step;
			// else if (newobjectidmapped == NodeType::COLLISIONOBJECT)Corner.y +=  UI_DrawText("COLLISIONOBJECT", Corner, 0.35f).y* -1.0f - step;
			// else if (newobjectidmapped == NodeType::LIGHTSOURCEOBJECT)Corner.y +=  UI_DrawText("LIGHTSOURCEOBJECT", Corner, 0.35f).y * -1.0f- step;
			// else if (newobjectidmapped == NodeType::CO_BALL)Corner.y +=  UI_DrawText("CO_BALL", Corner, 0.35f).y * -1.0f- step;
			// else if (newobjectidmapped == NodeType::CO_CUBE)Corner.y +=  UI_DrawText("CO_CUBE", Corner, 0.35f).y * -1.0f- step;
			// else if (newobjectidmapped == NodeType::CO_POLYGON)Corner.y +=  UI_DrawText("CO_POLYGON", Corner, 0.35f).y * -1.0f - step;
			// else if (newobjectidmapped == NodeType::PARTICLEOBJECT)
			Corner.y +=  UI_DrawText(NodeConstructorNames[newobjectidmapped], Corner, 0.35f).y * -1.0f - step;
			b = false;
			Corner.y += UI_button(&b, "Spawn", Corner).y * -1.0f - step;
			if(b)
			{
				Node* NewSpawnedNode = NodeConstructors[newobjectidmapped]();
				NewSpawnedNode->position = w->w_CameraPosition;
				Map.Nodes.push_back(NewSpawnedNode);
			}
			
			Corner.y +=UI_DrawText("Nodes:",Corner,0.45f).y * -1.0f;
			for(int i=0;i<Map.Nodes.size();i++)
			{
				b = false;
				UI_DrawText(Map.Nodes[i]->Name.c_str(),Corner - glm::vec2(0.0f,5.0f),0.35f);
				UI_DrawCube(Corner + glm::vec2(250*0.5f,0.0f), glm::vec2(250.0f,20.0f) * 0.5f, 0.0f, glm::vec4(0.07f));
				Corner.y += UI_button(&b, "", Corner,{250,20},0.35f,glm::vec4(0.0f),glm::vec4(0.5f),glm::vec4(0.0f)).y * -1.0f - 0;
				if(b)
				{
					SelectedNode = Map.Nodes[i];
					SelectedNodeID = i;
					SelectedAsset =NULL;
					SelectedAssetID=-1;
				}
			}
		}
	break;
	
	case 1:
	{

		std::string s = "CameraPosition X = ";
		s += std::to_string(w->w_CameraPosition.x);
		for (int i = 0; i < 4;i++)s.pop_back();
		s += ";  Y = ";
		s += std::to_string(w->w_CameraPosition.y);
		for (int i = 0; i < 4; i++)s.pop_back();
		Corner.y += UI_DrawText(s, Corner, 0.35f).y * -1.0f - step;

		s = "CameraScale X =";
		s += std::to_string(w->w_CameraScale.x);
		for (int i = 0; i < 4; i++)s.pop_back();
		s += ";  Y = ";
		s += std::to_string(w->w_CameraScale.y);
		for (int i = 0; i < 4; i++)s.pop_back();
		Corner.y += UI_DrawText(s, Corner, 0.35f).y * -1.0f - step;

		Corner.y += UI_DrawText(std::to_string(1.0f/delta), Corner, 0.35f).y * -1.0f - step;

		b = false;
		Corner.y += UI_button(&b, "Reset Camera", Corner).y * -1.0f - step;

		if (b)
		{
			w->w_CameraPosition = { 0.0f,0.0f };
			//w->w_CameraScale = { 1.0f,1.0f };
			AqueredCameraScale = { 1.0f,1.0f };
		}

		
		b = false;
		Corner.y += UI_button(&b, "VSync", Corner).y * -1.0f - step;

		if(b) 
		{
			VSync = !VSync;
			glfwSwapInterval(VSync);
		}
		Corner.y += UI_DrawText("Save file:", Corner, 0.35f).y * -1.0f - step;
		Corner.y += UI_TextBox(&MapFileName, Corner,128).y * -1.0f - step;
		b = false;
		Corner.y += UI_button(&b, "Save", Corner).y * -1.0f - step;

		if (b || keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_S])
			Map.SaveAs(MapFileName);
		
		b = false;
		Corner.y += UI_button(&b, "Load", Corner).y * -1.0f - step;
		if (b)
		{

			Map.LoadFrom(MapFileName);
			SelectedNode = NULL;
			SelectedNodeID = -1;
			SelectedAsset =NULL;
			SelectedAssetID=-1;
			UndoLine.clear();
			
		}

		
		Corner.y += UI_CheckBox(&grid, "Grid", Corner).y * -1.0f - step;
		if(grid)
		{
			
			Corner.y += UI_Drag(&grid_step, "Gtid step", Corner , 1.0f).y * -1.0f - step;
		}

		Corner.y += UI_CheckBox(&GameScene->filter, "Filter", Corner).y * -1.0f - step;
		if(GameScene->filter)
		{
			Corner.y += UI_SliderInt(&GameScene->filter_object,"object type",Corner,0,NodeConstructorNames.size()-1).y * -1.0f - step;
			int newobjectidmapped = -1;
			int id= 0;
			for(auto x : NodeConstructorNames)
			{
				if(id == GameScene->filter_object)
				{
					newobjectidmapped = x.first;
					break;
				}
				id++;
			}
			Corner.y +=  UI_DrawText(NodeConstructorNames[newobjectidmapped], Corner, 0.35f).y * -1.0f - step;
		}

		
		std::string ulinesizestr = "Undo buffer size: ";
		ulinesizestr += std::to_string(UndoLine.size());
		Corner.y += UI_DrawText(ulinesizestr, Corner , 0.35f).y * -1.0f - step;
		
		Corner.y += UI_DrawText("EditorColor", Corner , 0.35f).y * -1.0f - step;
		float xsize = 0.0f;
		xsize = UI_Drag(&EditorColor.x, "r", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&EditorColor.y, "g", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&EditorColor.z, "b", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&EditorColor.w, "a", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		xsize = 0.0f;	
		Corner.y += UI_DrawText("SceneBackgroundColor", Corner , 0.35f).y * -1.0f - step;
		xsize = UI_Drag(&SceneBackgroundColor.x, "r", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&SceneBackgroundColor.y, "g", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		xsize += UI_Drag(&SceneBackgroundColor.z, "b", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).x * 0.5f;
		Corner.y += UI_Drag(&SceneBackgroundColor.w, "a", Corner  + glm::vec2(xsize, 0.0f), 0.01f, { 40.0f,15.0f }).y * -1.0f - step;
		xsize = 0.0f;


		Corner.y += UI_Slider(&w->w_AmbientLight, "Ambient light", Corner).y * -1.0f - step;
		Corner.y += UI_Slider(&w->w_DirectionalLight, "Directional light", Corner).y * -1.0f - step;
		Corner.y += UI_Slider(&Simulation_speed, "Simulation speed", Corner,0.0f,2.0f).y * -1.0f - step;
	}
		break;
	
	default:
		break;
	}
	float constcolorwhiteaddition = 0.008f;
	iw->backgroundColor = { EditorColor.r * 0.3f + constcolorwhiteaddition,EditorColor.g * 0.3f + constcolorwhiteaddition,EditorColor.b * 0.3f + constcolorwhiteaddition,1.0f };
	pw->backgroundColor = { EditorColor.r * 0.3f + constcolorwhiteaddition,EditorColor.g * 0.3f + constcolorwhiteaddition,EditorColor.b * 0.3f + constcolorwhiteaddition,1.0f };
	cw->backgroundColor = { EditorColor.r * 0.3f + constcolorwhiteaddition,EditorColor.g * 0.3f + constcolorwhiteaddition,EditorColor.b * 0.3f + constcolorwhiteaddition,1.0f };
	w->backgroundColor = SceneBackgroundColor;
	GetWindow(0)->backgroundColor = EditorColor;
	//Nodes of Scene



	b = false;

	

	if (HEIGHT * 0.5f < abs(Corner.y + CameraPosition.y))
	{
		if (ProjectWindowScroll > 0.0f)
			ProjectWindowScroll = 0.0f;
		else if (ProjectWindowScroll < (Corner.y + CameraPosition.y))
			ProjectWindowScroll = (Corner.y + CameraPosition.y);
	}
	else
		ProjectWindowScroll = 0.0f;

	GetWindow(ProjectWindowID)->End();


	Corner.y = w->Position.y + (w->ViewportSize.y*0.5f) * w->Scale.y + 50.0f*0.5f;
	Corner.x =  w->Position.x - (w->ViewportSize.x*0.5f) * w->Scale.x;
	xstep = 0.0f;
	int Scenebuttoncount = 8;
	if(Running)
		Scenebuttoncount+=1;
	bSizeX = (cw->ViewportSize.x - step * (Scenebuttoncount+1)) / (float)Scenebuttoncount;// 4 buttons = 5 steps  s bbb s bbb s bbb s
	if(bSizeX>150.0f) bSizeX = 150.0f;
	if(bSizeX<50.0f) bSizeX = 50.0f;
	b=SceneWindowSelection == 0;
	xstep += UI_button(&b,"Viewport", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		SceneWindowSelection =0;
	b=SceneWindowSelection == 1;
	xstep += UI_button(&b,"NormalMaps", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		SceneWindowSelection =1;		
	b=SceneWindowSelection == 2;
	xstep += UI_button(&b,"LightMap", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		SceneWindowSelection =2;
	b=SceneWindowSelection == 4;
	xstep += UI_button(&b,"Height", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		SceneWindowSelection =4;
	b=SceneWindowSelection == 3;
	xstep += UI_button(&b,"Collisionmap", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
	if (b)
		SceneWindowSelection =3;
	
	b=false;
	if(!Running)
	{
		b=false;
		xstep += UI_button(&b,"Run", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
		if(b)
		{
			Running = true;
			Paused = false;
			Map.SaveAs("PreRunSave.sav");
			//Ready();

		}
	}
	else
	{
		b=false;
		xstep += UI_button(&b,"Stop", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
		if(b)
		{
			Running = false;
			Paused = false;
			cleanSelection =true;
			SceneEnd();
			Map.LoadFrom("PreRunSave.sav");
		}
		b=false;

		xstep += UI_button(&b,"StopWithSave", Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).x * 1.0f + step;
		if(b)
		{
			Running = false;
			Paused = false;
		}
	}
	
	std::string pauseunpause = "Pause";
	if(Paused)	pauseunpause = "Continue";

	b=false;
	Corner.y += UI_button(&b,pauseunpause.c_str(), Corner + glm::vec2(xstep,0.0f),glm::vec2(bSizeX,35.0f)).y * -1.0f - step;
	if(b)
		Paused = !Paused;
		
	
	Map.DrawCollisions = SceneWindowSelection == 3;
	Map.DrawRegularScene = !Map.DrawCollisions;

	GetWindow(SceneWindowID)->Use();
	if(cleanSelection)
	{
		SelectedNode = NULL;
		SelectedNodeID = -1;
		SelectedAsset =NULL;
		SelectedAssetID=-1;
	}
	//DrawCube(MousePosition, { 100.0f,100.0f }, 0.0f, {1.0f,1.0f,1.0f,1.0f}, true, NegativeNormalMapTexture);

	AqueredCameraScale *= 1.0f + scrollmovement * 0.1f;
	CameraScale += (AqueredCameraScale - CameraScale) * 0.25f * 0.017f * 60.0f;
	

	dif = glm::vec2(0.0f);
	if (buttons[GLFW_MOUSE_BUTTON_MIDDLE]&& !JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE] && MMBJustPressedWindow[3])
		dif = PrevMousePos - MousePosition;
	CameraPosition += dif;
	MousePosition += dif;
	
	if(w->active)
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_MIDDLE])
		{
			PrevMousePos = MousePosition;
			MMBJustPressedWindow[0]=false;
			MMBJustPressedWindow[1]=false;
			MMBJustPressedWindow[2]=false;
			MMBJustPressedWindow[3]=true;
		}

	

	// grab/select tool
	if (w->active)
	{
		if (JustPressedLMB)
		{
			PrevDifference *=0.0f;
			PrevMousePosition = MousePosition;
			grabbed = false;
			
			for (int i = 0; i < Map.Nodes.size(); i++)
			{
				if(Map.Nodes[i]->Delete ||  (GameScene->filter_object != Map.Nodes[i]->type && GameScene->filter))
					continue;
				bool mousetouched = Map.Nodes[i]->SelectionCheck(MousePosition);
				

				if (mousetouched)
				{
					if(GrabbedNode == NULL || GrabbedNode->Z_Index < Map.Nodes[i]->Z_Index)
					{
						GrabbedNode = Map.Nodes[i]; 
						GrabbedNodeID = i;
					}
					grabbed = true;
				}
			}

		}
		if (ReleasedLMB && sqrlength(PrevMousePosition - MousePosition)<2.0f && !keys[GLFW_KEY_LEFT_ALT])
		{
			SelectedNode = GrabbedNode;
			SelectedNodeID = GrabbedNodeID; 
			GrabbedNode= NULL;
			GrabbedNodeID = -1;
			grabbed = false;
			

		}
		
		if(GrabbedNode != NULL && grabbed && PrevMousePosition != MousePosition && keys[GLFW_KEY_LEFT_CONTROL])
		{
			GrabbedNode->position -=PrevDifference;
			
			PrevDifference = (MousePosition - PrevMousePosition);

			if(grid)
			{
				glm::vec2 steppedpos = GrabbedNode->position / grid_step;
				steppedpos.x = roundf(steppedpos.x);
				steppedpos.y = roundf(steppedpos.y);
				GrabbedNode->position = steppedpos * grid_step;

				glm::vec2 steppedPrevDifference = PrevDifference/ grid_step;
				steppedPrevDifference.x = roundf(steppedPrevDifference.x);
				steppedPrevDifference.y = roundf(steppedPrevDifference.y);
				PrevDifference = steppedPrevDifference * grid_step;
			}

			GrabbedNode->position +=PrevDifference;

		}
		if(SelectedNode!=NULL && SelectedNodeID>=0)
		{
			//Resize
			if(buttons[GLFW_MOUSE_BUTTON_1] && keys[GLFW_KEY_LEFT_ALT])
			{
				SelectedNode->OnResize(PrevDifference,MousePosition, PrevMousePosition);
				PrevDifference = (MousePosition - PrevMousePosition);
			}

			//Duplicate
			if(JustPressedkey[GLFW_KEY_D] && keys[GLFW_KEY_LEFT_CONTROL])
			{
				Node* NewNode = NodeConstructors[SelectedNode->type]();
				if(NewNode!=NULL)
				{
					//for subtypes to sync
					for(int i=0;i<2;i++)
					{
						std::vector<UI_DataPack> uidp = NewNode->GetUIData();
						std::vector<UI_DataPack> uidp2 = SelectedNode->GetUIData();
						for(int i=0;i<uidp.size();i++)
						{
							uidp2[i].CopyInto(&uidp[i]);
						}
					}
					Map.Nodes.push_back(NewNode);
				}
				
			}
		}

		if(ReleasedLMB)
		{
			GrabbedNode= NULL;
			GrabbedNodeID = -1;
			grabbed = false;
		}
	}
	else
	{
		GrabbedNode= NULL;
		GrabbedNodeID = -1;
		grabbed = false;		
	}
	
	if(!Paused && Running)
	{
		SelectedNode = NULL;
		SelectedNodeID = -1;
		SelectedAsset =NULL;
		SelectedAssetID=-1;
	}
	ProcessScene(&Map,true,true);


	GetWindow(SceneWindowID)->End();
	if(JustPressedkey[GLFW_KEY_DELETE] && SelectedNode !=NULL)
	{
		SelectedNode->Delete = true;
		SelectedNode = NULL;
		SelectedNodeID = -1;
		GrabbedNode= NULL;
		GrabbedNodeID = -1;
	}
	if(cleanSelection)
	{
		SelectedNode = NULL;
		SelectedNodeID = -1;
		SelectedAsset =NULL;
		SelectedAssetID=-1;
	}
	if(!Running)
	{
		if(keys[GLFW_KEY_LEFT_CONTROL] && bJustPressedkey[GLFW_KEY_Z] && UndoLine.size()>1)
		{
			UndoLine.pop_back();
			Map.LoadFromds(UndoLine.back());
			SelectedNode = NULL;
			SelectedNodeID = -1;
			SelectedAsset =NULL;
			SelectedAssetID=-1;
			ActionDoneThisFrame = false;
		}
		else
		{
			bool actiondone = false;
			for(int i=0;i<1024;i++)
			{
				if(keys[i] && i !=GLFW_KEY_LEFT_CONTROL && i !=GLFW_KEY_Z)
				{
					actiondone = true;
					break;
				}
			}
			if(!actiondone)
				for(int i=0;i<64;i++)
				{
					if(buttons[i])
					{
						actiondone = true;
						break;
					}
				}


			if(actiondone)
			{
				DataStorage tmpds = Map.SaveAsds();
				if(UndoLine.size()==0)
					UndoLine.push_back(tmpds);
				else
				{
					if(!buttons[GLFW_MOUSE_BUTTON_1])
					if(UndoLine.back().ToString() != tmpds.ToString())
						UndoLine.push_back(tmpds);
				}
			}
		}
	}
	
	//for (int i = 0; i < Windows.size(); i++)
	//	std::cout<<"\ni "<<i<<" ls = "<<Windows[i].w_LightSources.size();
}

int main()
{
	//initEngine();
	//initEngine("Redactor", 1920,1000,false);
	PreReady();
	initEngine("Redactor",s_Resolution.x,s_Resolution.y,s_Fullscreen);
	std::cout<<"\nexited, saving...";
	Map.SaveAs(MapFileName + ".back");
	std::cout<<"\nsaved, sound...";
	AL_Destroy();
	std::cout<<"\ndeleted, scene...";
	Destroy();
	std::cout<<"\ndeleted, threads...";
	//_DeleteScenethreads();
	std::cout<<"\ndeleted";
	return 0;
}

