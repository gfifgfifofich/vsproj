#pragma once

/*Tree

	Scene has vector of nodes.

	Node
		Object
			CollisionObject
				Ball
				Cube
				Polygon
			LightSource
			ParticleEmiterObject
				PointEmiter
				AttractionSphere
				MovementSphere - TODO (Probably connected to movement through AnimationGraph?)
				CaptureShapes
				...
		SoundSource
		
	
	All assets are in one map, referenced by pointers.
	when asset is marked for deletion, all pointers = NULL, next frame, asset is deleted
	
	Asset
		Texture
			Albedo
			NormalMap
			...
		Sound
		Material
		ParticleEmmiter
		|AnimationGraph? (asset to manipulate some properties of items over time)
        |InterpolationGraph? (asset to add to a node and control its property over time) || (asset to add to a property and control it over time)

		Shader --Todo
		PolygonMesh --Todo
*/
/*UI's
ObjName
	Node
		pos
		...
	Object
		texture
		normal map
		...
	
Slelection of nodes
				buttons
{ProjectWin } {NewNode	}
{CreatNewNod} {Node		}
{			} {Object	}
{			} {CO_Ball	}
{			} {LightSphe}
{			}

*/
/*Drawing
	ObjectDraw
	{
		Material m;
		m.texture = tex;
		...
		drawmaterial(m);
	}

*/
/*
*/



// single-use object to retrive information for ui, or get all properties with labels
class UI_DataPack
{
public:
	std::string name = "Data_Name";

	std::vector<glm::ivec3*> iSliderdata;
	std::vector<std::string> iSliderdatanames;
	std::vector<glm::vec3*> fSliderdata;
	std::vector<std::string> fSliderdatanames;
	
	std::vector<int*> idata;
	std::vector<std::string> idatanames;
	std::vector<bool*> bdata;
	std::vector<std::string> bdatanames;
	std::vector<float*> fdata;
	std::vector<std::string> fdatanames;
	std::vector<std::string*> textdata;
	std::vector<std::string> textdatanames;
	std::vector<std::string*> NoUitextdata;
	std::vector<std::string> NoUitextdatanames;
	std::vector<glm::vec2*> v2data;
	std::vector<std::string> v2datanames;
	std::vector<glm::vec3*> v3data;
	std::vector<std::string> v3datanames;
	std::vector<glm::vec4*> v4data;
	std::vector<std::string> v4datanames;
	std::vector<glm::vec4*> colordata;
	std::vector<std::string> colordatanames;

	std::vector<std::string*> texturedata;
	std::vector<std::string> texturedatanames;

	std::vector<std::string*> materialdata;
	std::vector<std::string> materialdatanames;

	std::vector<std::string*> shaderdata;
	std::vector<std::string> shaderdatanames;

	std::vector<std::string*> ParticleAssetdata;
	std::vector<std::string> ParticleAssetdatanames;

	std::vector<std::string*> polygonDatadata;
	std::vector<std::string> polygonDatadatanames;

	std::vector<polygonData*> p_polygonDatadata;
	std::vector<std::string> p_polygonDatadatanames;

	std::vector<Shader*> s_ShaderDatadata;
	std::vector<std::string> s_ShaderDatadatanames;

	std::vector<Texture*> t_texturedata;
	std::vector<std::string> t_texturedatanames;

	std::vector<Material*> m_materialdata;
	std::vector<std::string> m_materialdatanames;

	// Yep,full manual
	void CopyInto(UI_DataPack* dp)
	{
		for(int i=0;i<iSliderdata.size();i++)
			if(i<dp->iSliderdata.size())
				*dp->iSliderdata[i] = *iSliderdata[i];

		for(int i=0;i<fSliderdata.size();i++)
			if(i<dp->fSliderdata.size())
				*dp->fSliderdata[i] = *fSliderdata[i];

		for(int i=0;i<idata.size();i++)
			if(i<dp->idata.size())
				*dp->idata[i] = *idata[i];

		for(int i=0;i<bdata.size();i++)
			if(i<dp->bdata.size())
				*dp->bdata[i] = *bdata[i];

		for(int i=0;i<fdata.size();i++)
			if(i<dp->fdata.size())
				*dp->fdata[i] = *fdata[i];

		for(int i=0;i<textdata.size();i++)
			if(i<dp->textdata.size())
				*dp->textdata[i] = *textdata[i];

		for(int i=0;i<NoUitextdata.size();i++)
			if(i<dp->NoUitextdata.size())
				*dp->NoUitextdata[i] = *NoUitextdata[i];

		for(int i=0;i<v2data.size();i++)
			if(i<dp->v2data.size())
				*dp->v2data[i] = *v2data[i];

		for(int i=0;i<v3data.size();i++)
			if(i<dp->v3data.size())
				*dp->v3data[i] = *v3data[i];

		for(int i=0;i<v4data.size();i++)
			if(i<dp->v4data.size())
				*dp->v4data[i] = *v4data[i];

		for(int i=0;i<colordata.size();i++)
			if(i<dp->colordata.size())
				*dp->colordata[i] = *colordata[i];

		for(int i=0;i<texturedata.size();i++)
			if(i<dp->texturedata.size())
				*dp->texturedata[i] = *texturedata[i];

		for(int i=0;i<materialdata.size();i++)
			if(i<dp->materialdata.size())
				*dp->materialdata[i] = *materialdata[i];

		for(int i=0;i<shaderdata.size();i++)
			if(i<dp->shaderdata.size())
				*dp->shaderdata[i] = *shaderdata[i];

		for(int i=0;i<ParticleAssetdata.size();i++)
			if(i<dp->ParticleAssetdata.size())
				*dp->ParticleAssetdata[i] = *ParticleAssetdata[i];

		for(int i=0;i<polygonDatadata.size();i++)
			if(i<dp->polygonDatadata.size())
				*dp->polygonDatadata[i] = *polygonDatadata[i];

		for(int i=0;i<p_polygonDatadata.size();i++)
			if(i<dp->p_polygonDatadata.size())
				*dp->p_polygonDatadata[i] = *p_polygonDatadata[i];

		for(int i=0;i<s_ShaderDatadata.size();i++)
			if(i<dp->s_ShaderDatadata.size())
				*dp->s_ShaderDatadata[i] = *s_ShaderDatadata[i];

		for(int i=0;i<t_texturedata.size();i++)
			if(i<dp->t_texturedata.size())
				*dp->t_texturedata[i] = *t_texturedata[i];

		for(int i=0;i<m_materialdata.size();i++)
			if(i<dp->m_materialdata.size())
				*dp->m_materialdata[i] = *m_materialdata[i];

	}

};

enum ECSType
{
	ECSASSET = 0,
	ECSNODE = 1,
};
enum NodeType
{
	NODE = 0,
	OBJECT = 1,
	COLLISIONOBJECT = 2,
	CO_BALL = 3,
	CO_CUBE = 4,
	CO_POLYGON = 5,
	LIGHTSOURCEOBJECT = 6,
	PARTICLEOBJECT = 7,
	SOUNDSOURCE = 8,
	LASTNODE=10000
};
enum AssetType
{
	ASSET = 0,
	TEXTUREOBJECT = 1,
	MATERIALOBJECT = 2,
	PARTICLEASSET = 3,
	ANIMATIONGRAPH = 4,
	SOUNDASSET = 5,
	LASTASSET =10000
};



class Asset
{
public:
	int framesUntillDeletion = 5;
	std::vector<Asset**> UsedAssets;
	int id =0;
	std::string Name = "Asset";
	int type = AssetType::ASSET;

	bool Delete = false;

	Asset();
	UI_DataPack GetUIDataAsset();
	virtual std::vector<UI_DataPack> GetUIData();
	virtual void CustomUIDraw(glm::vec2* Corner, float Xstep,float step);
	virtual void Ready();
	virtual void Load();
	virtual void UnLoad();
	virtual void Process(float dt);
	virtual void DebugProcess(float dt);
	virtual void DrawPreview(glm::vec2 ui_position, glm::vec2 size);
	virtual void DrawProcess(float dt);

	virtual ~Asset();
	
};

class TextureObject : public Asset
{
public:
	int textid = -1;
	TextureObject();

	Texture texture;

	UI_DataPack GetUIDataTextureObject();
	virtual std::vector<UI_DataPack> GetUIData() override;


	virtual void Load() override;
	virtual void UnLoad() override;
	virtual void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override;
	virtual ~TextureObject() override;
};

inline std::vector<TextureObject*> AvailableTextures;

class MaterialObject : public Asset
{
public:


	Material mater;
	TextureObject* Texture = NULL;
	TextureObject* NormalMap = NULL;
	TextureObject* HeightMap = NULL;
	std::string TextureName = "No_texture";
	std::string NormalMapName = "No_texture";
	std::string HeightMapName = "No_texture";
	MaterialObject();

	UI_DataPack GetUIDataMaterialObject();
	virtual std::vector<UI_DataPack> GetUIData();
	virtual void DrawProcess(float dt) override;
	
	virtual void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override;

	virtual void Ready() override;
	virtual ~MaterialObject() override;
};

inline std::vector<MaterialObject*> AvailableMaterials;

class ParticleAsset : public Asset
{
public:
	ParticleEmiter pe;
	MaterialObject* Mater = NULL;
	std::string MaterialName = "No_Material";
	ParticleAsset();

	UI_DataPack GetUIDataParticleAsset();
	
	virtual std::vector<UI_DataPack> GetUIData() override;
	virtual void CustomUIDraw(glm::vec2* Corner, float Xstep,float step);
	void ObjectUpdateMaterial();
	virtual void DrawProcess(float dt) override;
	virtual void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override;

};

inline std::vector<ParticleAsset*> AvailableParticleAssets;

class SoundAsset : public Asset
{
	std::string PrevSoundName="";
public:
	unsigned int Sound = -1;
	std::string SoundName = "";

	SoundAsset();
	virtual ~SoundAsset() override;
	UI_DataPack GetUIDataSoundAsset();
	
	virtual void Load() override;
	virtual void UnLoad() override;
	virtual void DrawProcess(float dt) override;
	virtual std::vector<UI_DataPack> GetUIData() override;
	virtual void DrawPreview(glm::vec2 ui_position, glm::vec2 size) override;

};

inline std::vector<SoundAsset*> AvailableSoundAssets;


class Node
{

public:
	
	int framesUntillDeletion = 5;
	bool Delete = false;
	// array of pointers to pointers of assets
	std::vector<Asset**> UsedAssets;

	int Z_Index = 0;

	int id = 0;
	int type = NodeType::NODE;
	std::string Name = "Node";
	glm::vec2 position = glm::vec2(0.0f);


	Node();
	virtual ~Node();
	UI_DataPack GetUIDataNode();
	virtual std::vector<UI_DataPack> GetUIData();
	virtual void CustomUIDraw(glm::vec2* Corner, float Xstep,float step);
	virtual void Ready();
	virtual void MTPreProcess();
	virtual void MTProcess(float dt);
	virtual void PreProcess();
	virtual void Process(float dt);
	virtual void DrawProcess(float dt);
	virtual void DebugProcess(float dt);
	virtual void Draw();
	virtual void DebugDraw();
	virtual void DrawPreview(glm::vec2 ui_position, glm::vec2 size);
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp);
	virtual bool SelectionCheck(glm::vec2 point);
};

class Object : public Node
{
public:
	glm::vec2 Scale = glm::vec2(1.0f);
	glm::vec4 Color = glm::vec4(1.0f);
	float rotation = 0.0f;
	float depth = 0.0f;
	bool invertX = false;
	bool invertY = false;
	bool Additive = false;
	MaterialObject* Mater = NULL;
	std::string MaterialName = "No_Material";
	bool ForceMaterialCheck = true;
	Object();
	void ObjectPreconstructor();
	virtual void Ready() override;
	void ObjectUpdateMaterial();

	UI_DataPack GetUIDataObject();
	virtual std::vector<UI_DataPack> GetUIData() override;
	virtual void MTPreProcess() override;
	virtual void Draw() override;
	virtual bool SelectionCheck(glm::vec2 point) override;
	virtual void DebugDraw() override;
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp) override;

};

class LightSourceObject : public Object
{
public:
	float volume = 0.0f;
	float sizeZ = 1.0f;
	TextureObject* Texture = NULL;
	std::string TextureName = "No_texture";

	LightSourceObject();
	virtual void Ready() override;
	virtual void MTPreProcess() override;

	UI_DataPack GetUIDataLightSourceObject();
	virtual std::vector<UI_DataPack> GetUIData() override;

	virtual void Draw() override;
	virtual void DebugDraw() override;
};

class CollisionObject : public Object
{
public:
	int CollisionLayer = 0;
	int CO_type = NodeType::CO_BALL; 
	float roughness = 0;
	float bounciness = 0;
	CollisionObject();
	virtual void Ready() override;

	UI_DataPack GetUIDataCollisionObject();
	virtual std::vector<UI_DataPack> GetUIData() override;
};

class CO_Ball : public CollisionObject
{
public:
	ball b;
	CO_Ball();
	virtual void Ready() override;
	virtual void MTPreProcess() override;
	virtual void PreProcess() override;

	UI_DataPack GetUIDataCO_Ball();
	virtual std::vector<UI_DataPack> GetUIData() override;
	
	virtual void DebugDraw() override;
	virtual bool SelectionCheck(glm::vec2 point) override;
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp) override;
};

class CO_Cube : public CollisionObject
{
public:
	cube c;

	CO_Cube();
	virtual void Ready() override;
	virtual void MTPreProcess() override;
	virtual void PreProcess() override;
	
	UI_DataPack GetUIDataCO_Cube();
	virtual std::vector<UI_DataPack> GetUIData() override;
	virtual bool SelectionCheck(glm::vec2 point) override;
	virtual void DebugDraw() override;
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp) override;
};

class CO_Polygon : public CollisionObject
{
public:
	polygon p;
	std::string PolygonMeshName = "No_mesh";
	CO_Polygon();
	virtual void Ready() override;

	UI_DataPack GetUIDataCO_Polygon();
	virtual std::vector<UI_DataPack> GetUIData() override;
	virtual void Draw() override;
	virtual void DebugDraw() override;
};




class ParticleObject : public Node
{
public:

	glm::ivec3 po_typeData = glm::ivec3(0,0,7); 

	ParticleAsset* partAsset = NULL;
	std::string partAssetName = "None";

	glm::vec2 velocity = glm::vec2(0.0f);
	glm::vec2 scale = glm::vec2(1.0f);
	glm::vec4 Color = glm::vec4(1.0f);
	int amount = 1;
	float tick = 0.017f;
	float r = 1.0f;
	bool attractive = false;
	float attractionStrength = 1.0f;

	float t = 0.0f;

	ParticleEmiter::influenceSphere po_influenceSphere;
	ParticleEmiter::influenceCube po_influenceCube;
	ParticleEmiter::EmitionPoint po_EmitionPoint;
	ParticleEmiter::EmitionCircle po_EmitionCircle;
	ParticleEmiter::EmitionCube po_EmitionCube;
	ParticleEmiter::LightSphere po_LightSphere;
	ParticleEmiter::LightCube po_LightCube;



	ParticleObject();
	UI_DataPack GetUIDataParticleObject();
	virtual std::vector<UI_DataPack> GetUIData();
	virtual bool SelectionCheck(glm::vec2 point) override;
	virtual void DrawProcess(float dt) override;
	virtual void DebugDraw();
	virtual void OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp) override;
};

class AnimationGraph : public Asset
{
    bool forward = true;
public:
	Node* ControlledNode = NULL;
	std::string NodeName = "";
	float* fparam = NULL;
	glm::vec2* v2param = NULL;
	glm::vec4* v4param = NULL;
	bool is_fl = false;
	bool is_v2 = false;
	bool is_v4 = false;

    bool backforth = false;



	int paramCategID = -1;
	int paramId = -1;
	std::string paramName = "";

	float f1 = 0.0f;
	float f2 = 1.0f;

	glm::vec2 v21 = glm::vec2(0.0f);
	glm::vec2 v22 = glm::vec2(1.0f);

	glm::vec4 v41 = glm::vec4(0.0f);
	glm::vec4 v42 = glm::vec4(1.0f);

	float t = 0.0f;
	float maxt = 1.0f;
	AnimationGraph();

	UI_DataPack GetUIDataAnimationGraph();
	virtual std::vector<UI_DataPack> GetUIData();
	void UpdateData();
	virtual void Process(float dt) override;

};

class SoundSource : public Node
{
	unsigned int prevsound = 0;
	unsigned int soundid = 0;
	bool changedSound = false;
	bool prevLooping = false;
	glm::vec2 prevvelocity = {0.0f,0.0f};
	glm::vec2 prevposition = {0.0f,0.0f};
	float prevgain = 1.0f;
	float prevpitch = 1.0f;

	float prevTime = 0.0;
	float prevHalfVolumeDist = 1.0f; 
	float prevRollOff = 1.0f;
	float prevMaxDist = 1.0f;
	bool prevCamRelative = false;
	bool wasplaying = false;
	/*
	AL_SEC_OFFSET
	AL_REFERENCE_DISTANCE
	AL_ROLLOFF_FACTOR
	AL_MAX_DISTANCE
	AL_SOURCE_RELATIVE
	*/
public:
	float Time = 0.0;
	float HalfVolumeDist = 1.0f; 
	float RollOff = 1.0f;
	float MaxDist = 1.0f;
	bool CamRelative = false;
	bool ContiniusPlay = false;
	bool Looping = false;
	bool Temporary = false;
	bool NoAsset = true;
	unsigned int noAssetSound = NULL; 

	glm::vec2 velocity = {0.0f,0.0f};
	float gain = 1.0f;
	float pitch = 1.0f;
	unsigned int soundSource = -1;
	SoundAsset* sound = NULL;
	std::string SoundAssetName = "";
	
	SoundSource();
	virtual ~SoundSource() override; 
	UI_DataPack GetUIDataSoundSource();
	virtual std::vector<UI_DataPack> GetUIData() override;
	virtual void CustomUIDraw(glm::vec2* Corner, float Xstep,float step) override;
	virtual void MTPreProcess() override;
	virtual void PreProcess() override;
	virtual void Play();
	virtual void DebugDraw() override;
};



inline std::map<int,Node*(*)()> NodeConstructors;
inline std::map<int,std::string> NodeConstructorNames;

inline std::map<int,Asset*(*)()> AssetConstructors;
inline std::map<int,std::string> AssetConstructorNames;

inline void _FillVectorsOfNodesAndAssets()
{
	NodeConstructors.insert({NodeType::NODE,[](){return new Node();}});
	NodeConstructorNames.insert({NodeType::NODE,"Node"});

	NodeConstructors.insert({NodeType::OBJECT,[](){return (Node*)new Object();}});
	NodeConstructorNames.insert({NodeType::OBJECT,"Object"});

	NodeConstructors.insert({NodeType::COLLISIONOBJECT,[](){return (Node*)new CollisionObject();}});
	NodeConstructorNames.insert({NodeType::COLLISIONOBJECT,"CollisionObject"});

	NodeConstructors.insert({NodeType::CO_BALL,[](){return (Node*)new CO_Ball();}});
	NodeConstructorNames.insert({NodeType::CO_BALL,"CO_Ball"});

	NodeConstructors.insert({NodeType::CO_CUBE,[](){return (Node*)new CO_Cube();}});
	NodeConstructorNames.insert({NodeType::CO_CUBE,"CO_Cube"});

	NodeConstructors.insert({NodeType::CO_POLYGON,[](){return (Node*)new CO_Polygon();}});
	NodeConstructorNames.insert({NodeType::CO_POLYGON,"CO_Polygon"});

	NodeConstructors.insert({NodeType::LIGHTSOURCEOBJECT,[](){return (Node*)new LightSourceObject();}});
	NodeConstructorNames.insert({NodeType::LIGHTSOURCEOBJECT,"LightSourceObject"});

	NodeConstructors.insert({NodeType::PARTICLEOBJECT,[](){return (Node*)new ParticleObject();}});
	NodeConstructorNames.insert({NodeType::PARTICLEOBJECT,"ParticleObject"});

	NodeConstructors.insert({NodeType::SOUNDSOURCE,[](){return (Node*)new SoundSource();}});
	NodeConstructorNames.insert({NodeType::SOUNDSOURCE,"SoundSource"});



	AssetConstructors.insert({AssetType::ASSET,[](){return new Asset();}});
	AssetConstructorNames.insert({AssetType::ASSET,"Asset"});

	AssetConstructors.insert({AssetType::TEXTUREOBJECT,[](){return (Asset*)new TextureObject();}});
	AssetConstructorNames.insert({AssetType::TEXTUREOBJECT,"TextureObject"});

	AssetConstructors.insert({AssetType::MATERIALOBJECT,[](){return (Asset*)new MaterialObject();}});
	AssetConstructorNames.insert({AssetType::MATERIALOBJECT,"MaterialObject"});

	AssetConstructors.insert({AssetType::PARTICLEASSET,[](){return (Asset*)new ParticleAsset();}});
	AssetConstructorNames.insert({AssetType::PARTICLEASSET,"ParticleAsset"});
	
	AssetConstructors.insert({AssetType::ANIMATIONGRAPH,[](){return (Asset*)new AnimationGraph();}});
	AssetConstructorNames.insert({AssetType::ANIMATIONGRAPH,"AnimationGraph"});

	AssetConstructors.insert({AssetType::SOUNDASSET,[](){return (Asset*)new SoundAsset();}});
	AssetConstructorNames.insert({AssetType::SOUNDASSET,"SoundAsset"});
}
