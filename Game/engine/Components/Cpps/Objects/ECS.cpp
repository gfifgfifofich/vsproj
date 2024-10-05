

#include "../../Include/Helper.h"
#include "../../Include/Drawing.h"
#include "../../Include/Objects.h"
#include "../../Include/Objects/Particle.h"
#include "../../Include/UI.h"
#include "../../Include/Collisions.h"
#include "../../Include/sounds.h"
#include "../../Include/Objects/ECS.h"
#include "../../Include/SaveToFile.h";
#include "../../Include/Objects/Scene.h"


Asset::Asset()
{
	type =AssetType::ASSET;
	Name = "Asset";
}
Asset::~Asset() {};
TextureObject::TextureObject()
{
	type =AssetType::TEXTUREOBJECT;
	Name = "TextureObject";
}
TextureObject::~TextureObject() 
{
	UnLoad();
}
MaterialObject::MaterialObject()
{
	type =AssetType::MATERIALOBJECT;
	Name = "MaterialObject";
	UsedAssets.push_back((Asset**)&Texture);
	UsedAssets.push_back((Asset**)&NormalMap);
}
MaterialObject::~MaterialObject() 
{}
ParticleAsset::ParticleAsset()
{
	type = AssetType::PARTICLEASSET;
	Name = "ParticleAsset";
	UsedAssets.push_back((Asset**)&Mater);
}
AnimationGraph::AnimationGraph()
{
	type =AssetType::ANIMATIONGRAPH;
	Name = "AnimationGraph";
}
SoundAsset::SoundAsset()
{
	type =AssetType::SOUNDASSET;
	Name = "SoundAsset";
}
SoundAsset::~SoundAsset()
{
	UnLoad();
}

UI_DataPack Asset::GetUIDataAsset()
{
	UI_DataPack uidp;
	uidp.name = "Asset";
	
	uidp.idata.push_back(&id);
	uidp.idatanames.push_back("ID");
	
	uidp.textdata.push_back(&Name);
	uidp.textdatanames.push_back("Name");
	
	return uidp;
}
std::vector<UI_DataPack> Asset::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataAsset());
	return data;
}
void Asset::CustomUIDraw(glm::vec2* Corner, float Xstep,float step) {};
void Asset::Load() {};
void Asset::UnLoad() {};
void Asset::Ready() {};
void Asset::Process(float dt) {};
void Asset::DebugProcess(float dt) {};
void Asset::DrawPreview(glm::vec2 ui_position, glm::vec2 size)
{
	UI_DrawCircle(ui_position,size.x,glm::vec4(1.0f),false,NULL,-100);
};
void Asset::DrawProcess(float dt)
{
};



UI_DataPack TextureObject::GetUIDataTextureObject()
{
	UI_DataPack uidp;
	uidp.name = "Texture";
	uidp.t_texturedata.push_back(&texture);
	uidp.t_texturedatanames.push_back("texture");		

	//uidp.idata.push_back(&textid);
	//uidp.idatanames.push_back("textureID");		
	return uidp;
}
std::vector<UI_DataPack> TextureObject::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataAsset());
	data.push_back(GetUIDataTextureObject());
	return data;
}
void TextureObject::Load() 
{
	texture.Load();
}
void TextureObject::UnLoad() 
{
	texture.Delete();
};
void TextureObject::DrawPreview(glm::vec2 ui_position, glm::vec2 size) 
{
	if(texture.texture !=NULL && glIsTexture(texture.texture))
		UI_DrawTexturedQuad(ui_position,size,texture.texture,0.0f,glm::vec4(1.0f),-100);
};



UI_DataPack MaterialObject::GetUIDataMaterialObject()
{
	UI_DataPack uidp;
	uidp.name = "Material";
	uidp.texturedata.push_back(&TextureName);
	uidp.texturedatanames.push_back("Texture");		
	uidp.texturedata.push_back(&NormalMapName);
	uidp.texturedatanames.push_back("Normal_Map");
	uidp.texturedata.push_back(&HeightMapName);
	uidp.texturedatanames.push_back("Height_Map");
			
	return uidp;
}
std::vector<UI_DataPack> MaterialObject::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataAsset());
	data.push_back(GetUIDataMaterialObject());
	return data;
}
void MaterialObject::DrawProcess(float dt)
{
	bool texexists = false;
	bool normexists = false;
	bool heightexists = false;
	for(auto tex : AvailableTextures)
	{
		
		if(tex->Name == TextureName && !texexists && !tex->Delete)
		{
			Texture = tex;
			texexists=true;
		}
		if(tex->Name == NormalMapName && !normexists && !tex->Delete)
		{
			NormalMap = tex;
			normexists=true;
		}
		if(tex->Name == HeightMapName && !heightexists && !tex->Delete)
		{
			HeightMap = tex;
			heightexists=true;
		}
	}
	if(!texexists)
		Texture = NULL;
	if(!normexists)
		NormalMap = NULL;
	if(!heightexists)
		HeightMap = NULL;

	if(Texture!=NULL)
		mater.Texture = Texture->texture.texture;
	else
		mater.Texture = NULL;
		
	if(NormalMap!=NULL)
		mater.NormalMap = NormalMap->texture.texture;
	else
		mater.NormalMap = NULL;
		
	if(HeightMap!=NULL)
		mater.HeightMap = HeightMap->texture.texture;
	else
		mater.HeightMap = NULL;
}
void MaterialObject::DrawPreview(glm::vec2 ui_position, glm::vec2 size)
{
	UI_DrawQuadWithMaterial(ui_position,size,mater,0.0f,glm::vec4(1.0f),false,-100);
};
void MaterialObject::Ready()
{};



UI_DataPack ParticleAsset::GetUIDataParticleAsset()
{
	UI_DataPack uidp;
	uidp.name = "ParticleObject";
	uidp.materialdata.push_back(&MaterialName);
	uidp.materialdatanames.push_back("Material");
	uidp.bdata.push_back(&pe.Additive);
	uidp.bdatanames.push_back("Additive");
	uidp.bdata.push_back(&pe.DrawToNormalMap);
	uidp.bdatanames.push_back("DrawToNormalMap");
	uidp.bdata.push_back(&pe.influenced);
	uidp.bdatanames.push_back("influenced");
	uidp.bdata.push_back(&pe.lighted);
	uidp.bdatanames.push_back("lighted");
	
	uidp.idata.push_back(&pe.Z_Index);
	uidp.idatanames.push_back("Z_Index");
	uidp.fdata.push_back(&pe.VelocityDamper);
	uidp.fdatanames.push_back("VelocityDamper");
	uidp.fdata.push_back(&pe.InitialRotation);
	uidp.fdatanames.push_back("InitialRotation");
	uidp.fdata.push_back(&pe.RotationRandomness);
	uidp.fdatanames.push_back("RotationRandomness");
	uidp.fdata.push_back(&pe.RotationVelocity);
	uidp.fdatanames.push_back("RotationVelocity");
	uidp.fdata.push_back(&pe.RotationAcceleration);
	uidp.fdatanames.push_back("RotationAcceleration");
	uidp.fdata.push_back(&pe.RotationDamper);
	uidp.fdatanames.push_back("RotationDamper");
	uidp.fdata.push_back(&pe.InitialOrbitalVelocity);
	uidp.fdatanames.push_back("InitialOrbitalVelocity");
	uidp.fdata.push_back(&pe.OrbitalVelocityRandomness);
	uidp.fdatanames.push_back("OrbitalVelocityRandomness");
	uidp.fdata.push_back(&pe.lifetime);
	uidp.fdatanames.push_back("lifetime");
	uidp.fdata.push_back(&pe.lifetimeRandomness);
	uidp.fdatanames.push_back("lifetimeRandomness");
	uidp.v2data.push_back(&pe.acceleration);
	uidp.v2datanames.push_back("Acceleration");
	uidp.v2data.push_back(&pe.InitialVelocity);
	uidp.v2datanames.push_back("InitialVelocity");
	uidp.v2data.push_back(&pe.StartSize);
	uidp.v2datanames.push_back("StartSize");
	uidp.v2data.push_back(&pe.EndSize);
	uidp.v2datanames.push_back("EndSize");
	uidp.v4data.push_back(&pe.VelocityRandomness);
	uidp.v4datanames.push_back("VelocityRandomness");
	uidp.colordata.push_back(&pe.StartColor);
	uidp.colordatanames.push_back("StartColor");
	uidp.colordata.push_back(&pe.EndColor);
	uidp.colordatanames.push_back("EndColor");
	
	uidp.NoUitextdata.push_back(&pe.Type);
	uidp.NoUitextdatanames.push_back("ParticleEmiterType");
	return uidp;
}
std::vector<UI_DataPack> ParticleAsset::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataAsset());
	data.push_back(GetUIDataParticleAsset());
	
	return data;
}
void ParticleAsset::CustomUIDraw(glm::vec2* Corner, float Xstep,float step) 
{
	int tip = 0;
	if(pe.Type == "QUAD") tip = 0;
	if(pe.Type == "LINE") tip = 1;
	if(pe.Type == "CIRCLE") tip = 2;
	if(pe.Type == "TEXTURED") tip = 3;
	if(pe.Type == "TEXTUREDLINE") tip = 4;
	Corner->y += UI_SliderInt(&tip ,(std::string("DrawingType: ") + pe.Type).c_str(), *Corner + glm::vec2(Xstep,0.0f),0,4).y * -1.0f - step;
	if(tip == 0 )  pe.Type = "QUAD";
	if(tip == 1 )  pe.Type = "LINE";
	if(tip == 2 )  pe.Type = "CIRCLE";
	if(tip == 3 )  pe.Type = "TEXTURED";
	if(tip == 4 )  pe.Type = "TEXTUREDLINE";
}
void ParticleAsset::ObjectUpdateMaterial()
{
	bool texexists = false;
	for(auto tex : AvailableMaterials)
	{
		
		if(tex->Name == MaterialName && !texexists && !tex->Delete)
		{
			Mater = tex;
			texexists=true;
		}
	}
	if(!texexists)
		Mater = NULL;
}
void ParticleAsset::DrawProcess(float dt)
{
	ObjectUpdateMaterial();
	if(Mater!=NULL)
		pe.material = Mater->mater;
	else
	{
		pe.material.Texture=NULL;
		pe.material.NormalMap=NULL;
	}
	
	pe.Process(dt);

	pe.CaptureSpheres.clear();
	pe.SpheresOfInfluence.clear();
	pe.CubesOfInfluence.clear();
	pe.EmitionPoints.clear();
	pe.EmitionCircles.clear();
	pe.EmitionCubes.clear();
	pe.LightSpheres.clear();
	pe.LightCubes.clear();
};
void ParticleAsset::DrawPreview(glm::vec2 ui_position, glm::vec2 size)
{
	UI_DrawCircle(ui_position,size.x,glm::vec4(1.0f),false,NULL,-100);
}

void SoundAsset::Load()
{
	if(Sound>=0)
		UnLoad();
	Sound = LoadSound(SoundName.c_str());
}
void SoundAsset::UnLoad()
{
	if(Sound<0)
		DeleteSound(&Sound);
}
void SoundAsset::DrawProcess(float dt)
{
	if(PrevSoundName!=SoundName)
	{
		Load();
		PrevSoundName = SoundName;
	}
}
UI_DataPack SoundAsset::GetUIDataSoundAsset()
{
	UI_DataPack uidp;
	uidp.name = "Sound";
	uidp.textdata.push_back(&SoundName);
	uidp.textdatanames.push_back("FileName");	


	return uidp;
}
std::vector<UI_DataPack> SoundAsset::GetUIData()
{

	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataAsset());
	data.push_back(GetUIDataSoundAsset());
	
	return data;
}
void SoundAsset::DrawPreview(glm::vec2 ui_position, glm::vec2 size)
{
	UI_DrawCircle(ui_position,size.x,glm::vec4(1.0f),false,NULL,-100);
}



UI_DataPack AnimationGraph::GetUIDataAnimationGraph()
{
	UI_DataPack uidp;
	uidp.name = "AnimationGraph";
    uidp.bdata.push_back(&backforth);
    uidp.bdatanames.push_back("backforth");	
	uidp.fdata.push_back(&t);
	uidp.fdatanames.push_back("t");
	uidp.fdata.push_back(&f1);
	uidp.fdatanames.push_back("FirstFloat");
	uidp.fdata.push_back(&f2);
	uidp.fdatanames.push_back("SecondFloat");
	uidp.fdata.push_back(&maxt);
	uidp.fdatanames.push_back("Maxt");
	uidp.v2data.push_back(&v21);
	uidp.v2datanames.push_back("FirstVec");
	uidp.v2data.push_back(&v22);
	uidp.v2datanames.push_back("SecondVec");
	uidp.v4data.push_back(&v41);
	uidp.v4datanames.push_back("FirstVecF");
	uidp.v4data.push_back(&v42);
	uidp.v4datanames.push_back("SecondVecF");
	
	uidp.textdata.push_back(&paramName);
	uidp.textdatanames.push_back("paramName");
	
	uidp.textdata.push_back(&NodeName);
	uidp.textdatanames.push_back("NodeName");
	
	return uidp;
}
std::vector<UI_DataPack> AnimationGraph::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataAsset());
	data.push_back(GetUIDataAnimationGraph());
	return data;
}
void AnimationGraph::UpdateData()
{
	if(ControlledNode==NULL)
		return;
	
	std::vector<UI_DataPack> data = ControlledNode->GetUIData();
	for(int i=0;i<data.size();i++)
	{
		for(int a = 0;a<data[i].fdata.size();a++)
			if(data[i].fdatanames[a] == paramName)
				{
					paramCategID=i;
					paramId=a;
					is_fl = true;
					is_v2 = false;
					is_v4 = false;
					fparam = data[i].fdata[a];
					v2param = NULL;
					v4param = NULL;
				}
		for(int a = 0;a<data[i].v2data.size();a++)
			if(data[i].v2datanames[a] == paramName)
				{
					paramCategID=i;
					paramId=a;
					is_fl = false;
					is_v2 = true;
					is_v4 = false;
					fparam = NULL;
					v2param = data[i].v2data[a];
					v4param = NULL;
				}
		for(int a = 0;a<data[i].v4data.size();a++)
			if(data[i].v4datanames[a] == paramName)
				{
					paramCategID=i;
					paramId=a;
					is_fl = false;
					is_v2 = false;
					is_v4 = true;
					fparam = NULL;
					v2param = NULL;
					v4param = data[i].v4data[a];
				}
	}
}
void AnimationGraph::Process(float dt)
{
	if(SceneInProcess!=NULL)
    {
        for(int i=0;i<SceneInProcess->Nodes.size();i++)
        {
            if(SceneInProcess->Nodes[i]->Name == NodeName)
                {
                    ControlledNode = SceneInProcess->Nodes[i];
                    break;
                }
        }
    }
	if(ControlledNode!=NULL)
	{	if(ControlledNode->Delete)
		{
			ControlledNode = NULL;	
			fparam = NULL;
			v2param = NULL;
			v4param = NULL;
			is_fl = false;
			is_v2 = false;
			is_v4 = false;
			return;
		}}
	else return;
	
	UpdateData();
	if(!forward)
        dt*=-1.0f;

    if(is_fl && fparam!=NULL)
	{
		*fparam -= LinearInterpolation(f1,f2,t);
		t+=dt;
		*fparam += LinearInterpolation(f1,f2,t);
	} 
	if(is_v2 && v2param!=NULL)
	{
		v2param->x -= LinearInterpolation(v21.x,v22.x,t);
		v2param->y -= LinearInterpolation(v21.y,v22.y,t);
		t+=dt;
		v2param->x += LinearInterpolation(v21.x,v22.x,t);
		v2param->y += LinearInterpolation(v21.y,v22.y,t);
	} 
	if(is_v4 && v4param!=NULL)
	{
		v4param->x -= LinearInterpolation(v41.x,v42.x,t);
		v4param->y -= LinearInterpolation(v41.y,v42.y,t);
		v4param->z -= LinearInterpolation(v41.z,v42.z,t);
		v4param->w -= LinearInterpolation(v41.w,v42.w,t);
		t+=dt;
		v4param->x += LinearInterpolation(v41.x,v42.x,t);
		v4param->y += LinearInterpolation(v41.y,v42.y,t);
		v4param->z += LinearInterpolation(v41.z,v42.z,t);
		v4param->w += LinearInterpolation(v41.w,v42.w,t);
	} 

    if(backforth)
    {
        if(forward)
        {
            if(t>=maxt)
                forward = false;
        }
        else
        {
            if(t<=0.0f)
                forward = true;
        }
    }
    else
    {
        forward = true;

        if(t>=maxt)
            {
                if(is_fl && fparam!=NULL)
                   {
                       *fparam -= LinearInterpolation(f1,f2,t);
                   } 
                   if(is_v2 && v2param!=NULL)
                   {
                       v2param->x -= LinearInterpolation(v21.x,v22.x,t);
                       v2param->y -= LinearInterpolation(v21.y,v22.y,t);
                   } 
                   if(is_v4 && v4param!=NULL)
                   {
                       v4param->x -= LinearInterpolation(v41.x,v42.x,t);
                       v4param->y -= LinearInterpolation(v41.y,v42.y,t);
                       v4param->z -= LinearInterpolation(v41.z,v42.z,t);
                       v4param->w -= LinearInterpolation(v41.w,v42.w,t);
                   } 


                t=0.0f;
            }
    }
} 



// NODES --------------------------------------------------------------------------------------------------

Node::Node()
{
	type = NodeType::NODE;
	Name = "Node";
}
Node::~Node()
{
}
void Object::ObjectPreconstructor()
{
	UsedAssets.push_back((Asset**)&Mater);
}
Object::Object()
{
	type = NodeType::OBJECT;
	Name = "Object";
	ObjectPreconstructor();
}
LightSourceObject::LightSourceObject()
{
	type = NodeType::LIGHTSOURCEOBJECT;
	Name = "LightSource";
	UsedAssets.push_back((Asset**)&Texture);
}
CollisionObject::CollisionObject()
{
	type = NodeType::COLLISIONOBJECT;
	Name = "CollisionObject";
	ObjectPreconstructor();
}
CO_Ball::CO_Ball()
{
	type = NodeType::CO_BALL;
	Name = "CO_Ball";
	ObjectPreconstructor();
	b.r = 1.0f;
}
CO_Cube::CO_Cube()
{
	type = NodeType::CO_CUBE;
	Name = "CO_Cube";
	ObjectPreconstructor();
	c.width = 1.0f;
	c.height = 1.0f;
}
CO_Polygon::CO_Polygon()
{
	type = NodeType::CO_POLYGON;
	Name = "CO_Polygon";
	ObjectPreconstructor();
}
ParticleObject::ParticleObject()
{
	type = NodeType::PARTICLEOBJECT;
	Name = "ParticleObject";
	UsedAssets.push_back((Asset**)&partAsset);
}
SoundSource::SoundSource()
{
	type = NodeType::SOUNDSOURCE;
	Name = "SoundSource";
	UsedAssets.push_back((Asset**)&sound);
}
SoundSource::~SoundSource()
{
	if(soundSource>=0)
	{
		StopSource(&soundSource);
		DeleteSource(&soundSource);
	} 
}





UI_DataPack Node::GetUIDataNode()
{
	UI_DataPack uidp;
	uidp.name = "Node";
	uidp.textdata.push_back(&Name);
	uidp.textdatanames.push_back("Name");
	uidp.idata.push_back(&id);
	uidp.idatanames.push_back("ID");
	uidp.v2data.push_back(&position);
	uidp.v2datanames.push_back("Position");
	uidp.idata.push_back(&Z_Index);
	uidp.idatanames.push_back("Z_Index");
	return uidp;
}
std::vector<UI_DataPack> Node::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	return data;
};
void Node::CustomUIDraw(glm::vec2* Corner, float Xstep,float step) {};
void Node::Ready() {};
void Node::PreProcess() {};
void Node::Process(float dt) {};
void Node::MTPreProcess() {};
void Node::MTProcess(float dt) {};
void Node::DrawProcess(float dt) {};
void Node::DebugProcess(float dt) {};
void Node::Draw() {};
void Node::DebugDraw() 
{
	DrawCircle(position,1.0f,{1.0f,1.0f,1.0f,1.0f});
};
void Node::DrawPreview(glm::vec2 ui_position, glm::vec2 size) 
{
	UI_DrawCircle(ui_position,size.x,glm::vec4(1.0f),false,0,-100);
};
void Node::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{

}
bool Node::SelectionCheck(glm::vec2 point) 
{
	ball b;
	b.r = 1.0f;
	b.position = position;
	return BalltoPointCollisionCheck(b,point);
};



void Object::Ready()
{
}
void Object::ObjectUpdateMaterial()
{
	if(!ForceMaterialCheck)
		return;
	bool texexists = false;
	for(auto tex : AvailableMaterials)
	{
		
		if(tex->Name == MaterialName && !texexists && !tex->Delete)
		{
			Mater = tex;
			texexists=true;
		}
	}
	
	if(!texexists)
		Mater = NULL;
}
void Object::MTPreProcess()
{
	ObjectUpdateMaterial();
}
UI_DataPack Object::GetUIDataObject()
{
	UI_DataPack uidp;
	uidp.name = "Object";
	uidp.v2data.push_back(&Scale);
	uidp.v2datanames.push_back("Scale");
	uidp.fdata.push_back(&rotation);
	uidp.fdatanames.push_back("Rotation");
	uidp.fdata.push_back(&depth);
	uidp.fdatanames.push_back("Depth");
	
	uidp.colordata.push_back(&Color);
	uidp.colordatanames.push_back("Color");
	
	

	uidp.bdata.push_back(&invertX);
	uidp.bdatanames.push_back("invertX");
	uidp.bdata.push_back(&invertY);
	uidp.bdatanames.push_back("invertY");
	uidp.bdata.push_back(&Additive);
	uidp.bdatanames.push_back("Additive");
	uidp.materialdata.push_back(&MaterialName);
	uidp.materialdatanames.push_back("Material");
	
	
	return uidp;
}
std::vector<UI_DataPack> Object::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	return data;
};
void Object::Draw()
{
	Material m;
	if(Mater!=NULL)
		m = Mater->mater;
	m.flipX = invertX;
	m.flipY = invertY;
	if(m.Texture == NULL)
		m.Texture = FlatColorTexture;
	if(m.NormalMap == NULL)
		m.NormalMap = NULL;
	DrawQuadWithMaterial(position,Scale,m,rotation,Color,Z_Index,Additive,depth);
};
bool Object::SelectionCheck(glm::vec2 point) 
{
	cube NodeCube;
	NodeCube.position =position;
	NodeCube.width = Scale.x;
	NodeCube.height = Scale.y;
	return PointToQuadCollisionCheck(NodeCube, point);
};
void Object::DebugDraw() 
{
	DrawCube(position,Scale,0.0f,{Color.r,Color.g,Color.b,0.6f});
};
void Object::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{
	glm::vec2 dif = mp - prevmp;
	Scale -= prevdif;
	Scale += dif;
}



void LightSourceObject::Ready()
{
}
void LightSourceObject::MTPreProcess()
{
	bool texexists = false;
	for(auto tex : AvailableTextures)
	{
		
		if(tex->Name == TextureName && !texexists && !tex->Delete)
		{
			Texture = tex;
			texexists=true;
		}
	}
	if(!texexists)
		Texture = NULL;
}
UI_DataPack LightSourceObject::GetUIDataLightSourceObject()
{
	UI_DataPack uidp;
	uidp.name = "LightSource";
	uidp.fdata.push_back(&volume);
	uidp.fdatanames.push_back("Volume");
	uidp.fdata.push_back(&sizeZ);
	uidp.fdatanames.push_back("sizeZ");
	
	
	uidp.texturedata.push_back(&TextureName);
	uidp.texturedatanames.push_back("Texture");
	return uidp;
}
std::vector<UI_DataPack> LightSourceObject::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data[data.size()-1].materialdata.pop_back(); // Additive option
	data[data.size()-1].materialdatanames.pop_back();
	data[data.size()-1].bdata.pop_back(); // Additive option
	data[data.size()-1].bdatanames.pop_back();
	data.push_back(GetUIDataLightSourceObject());
	return data;
};
void LightSourceObject::Draw()
{
	if(Texture != NULL)
		DrawLight(glm::vec3(position,depth),glm::vec3(Scale,sizeZ),Color,volume,rotation,Texture->texture.texture);
	else 
		DrawLight(glm::vec3(position,depth),glm::vec3(Scale,sizeZ),Color,volume,rotation);
};
void LightSourceObject::DebugDraw()
{
	DrawCube(position,Scale,rotation,{Color.r,Color.g,Color.b,0.4f});
};



void CollisionObject::Ready()
{
}
UI_DataPack CollisionObject::GetUIDataCollisionObject()
{
	UI_DataPack uidp;
	uidp.name = "Collision object";
	uidp.idata.push_back(&CollisionLayer);
	uidp.idatanames.push_back("Collision_layer");
	uidp.fdata.push_back(&roughness);
	uidp.fdatanames.push_back("Roughness");
	uidp.fdata.push_back(&bounciness);
	uidp.fdatanames.push_back("Bounciness");
	return uidp;
}
std::vector<UI_DataPack> CollisionObject::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataCollisionObject());
	return data;
};



void CO_Ball::Ready()
{
}
void CO_Ball::MTPreProcess()
{
	ObjectUpdateMaterial();
	b.position = position;
	b.roughness = roughness;
	b.bounciness = bounciness;
}
void CO_Ball::PreProcess()
{
	SceneInProcess->Collision_balls.push_back(&b);	
}
UI_DataPack CO_Ball::GetUIDataCO_Ball()
{
	UI_DataPack uidp;
	uidp.name = "Ball";
	uidp.fdata.push_back(&b.r);
	uidp.fdatanames.push_back("Radius");	
	return uidp;
}
std::vector<UI_DataPack> CO_Ball::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataCollisionObject());
	data.push_back(GetUIDataCO_Ball());
	return data;
};
void CO_Ball::DebugDraw()
{
	DrawCircle(position,b.r,{Color.r,Color.g,Color.b,0.6f});
};
bool CO_Ball::SelectionCheck(glm::vec2 point)
{
	return BalltoPointCollisionCheck(b,point);
};
void CO_Ball::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{
	glm::vec2 dif = mp - prevmp;
	Scale -= prevdif;
	Scale += dif;
	b.r -= length(prevdif);
	b.r += length(dif);
}



void CO_Cube::Ready()
{
}
void CO_Cube::MTPreProcess()
{
	ObjectUpdateMaterial();
	c.position = position;
}
void CO_Cube::PreProcess()
{
	SceneInProcess->Collision_cubes.push_back(&c);	
}
UI_DataPack CO_Cube::GetUIDataCO_Cube()
{
	UI_DataPack uidp;
	uidp.name = "Cube";
	uidp.fdata.push_back(&c.width);
	uidp.fdatanames.push_back("Width");
	uidp.fdata.push_back(&c.height);
	uidp.fdatanames.push_back("Height");	
	return uidp;
}
std::vector<UI_DataPack> CO_Cube::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataCollisionObject());
	data.push_back(GetUIDataCO_Cube());
	return data;
};
bool CO_Cube::SelectionCheck(glm::vec2 point)
{
	cube NodeCube;
	NodeCube.position =position;
	NodeCube.width = c.width;
	NodeCube.height = c.height;
	return PointToQuadCollisionCheck(NodeCube, point);
};
void CO_Cube::DebugDraw()
{
	DrawCube(c,{Color.r,Color.g,Color.b,0.6f});		
};
void CO_Cube::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{
	glm::vec2 dif = mp - prevmp;
	Scale -= prevdif;
	Scale += dif;
	c.width -= prevdif.x;
	c.height -= prevdif.y;
	c.width += dif.x;
	c.height += dif.y;
}



void CO_Polygon::Ready()
{
}
UI_DataPack CO_Polygon::GetUIDataCO_Polygon()
{
	UI_DataPack uidp;
	uidp.name = "Polygon";
	uidp.polygonDatadata.push_back(&PolygonMeshName);
	uidp.polygonDatadatanames.push_back("Mesh");	
	return uidp;
}
std::vector<UI_DataPack> CO_Polygon::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataObject());
	data.push_back(GetUIDataCollisionObject());
	data.push_back(GetUIDataCO_Polygon());
	return data;
};
void CO_Polygon::Draw()
{
	// if(Mater!=NULL)
	// {
	// 	p.Texture = Mater->mater.Texture;
	// 	p.NormalMap = Mater->mater.NormalMap;
	// }
	// p.color = Color;
	// p.Scale = Scale;
	// p.Rotation = rotation;
	// p.Update_Shape();
	// DrawPolygon(&p);
};
void CO_Polygon::DebugDraw()
{
	// p.color = Color;
	// p.Scale = Scale;
	// p.Rotation = rotation;
	// p.color.a = 0.2f;
	// p.Texture = NULL;
	// p.NormalMap = NULL;
	// DrawPolygon(&p);
};



UI_DataPack ParticleObject::GetUIDataParticleObject()
{
	UI_DataPack uidp;
	uidp.name = "ParticleEmiterObject";
	uidp.ParticleAssetdata.push_back(&partAssetName);
	uidp.ParticleAssetdatanames.push_back("ParticleAsset");
	uidp.iSliderdata.push_back(&po_typeData);
	uidp.iSliderdatanames.push_back("ParticleObjectType");
	switch (po_typeData.x)
	{
	case 0:
		uidp.v2data.push_back(&velocity);
		uidp.v2datanames.push_back("velocity");
		
		uidp.fdata.push_back(&r);
		uidp.fdatanames.push_back("Radius");
		uidp.fdata.push_back(&attractionStrength);
		uidp.fdatanames.push_back("AttractionStrength");
		uidp.bdata.push_back(&attractive);
		uidp.bdatanames.push_back("Attractive");
		
		break;
	case 1:
		uidp.v2data.push_back(&scale);
		uidp.v2datanames.push_back("scale");
		uidp.v2data.push_back(&velocity);
		uidp.v2datanames.push_back("velocity");
		
		uidp.fdata.push_back(&attractionStrength);
		uidp.fdatanames.push_back("AttractionStrength");
		uidp.bdata.push_back(&attractive);
		uidp.bdatanames.push_back("Attractive");
		break;
	case 2:
		uidp.v2data.push_back(&velocity);
		uidp.v2datanames.push_back("velocity");
		
		uidp.idata.push_back(&amount);
		uidp.idatanames.push_back("amount");
		uidp.fdata.push_back(&tick);
		uidp.fdatanames.push_back("tick");
		break;
	case 3:
		uidp.v2data.push_back(&velocity);
		uidp.v2datanames.push_back("velocity");
		
		uidp.fdata.push_back(&r);
		uidp.fdatanames.push_back("Radius");
		uidp.idata.push_back(&amount);
		uidp.idatanames.push_back("amount");
		uidp.fdata.push_back(&tick);
		uidp.fdatanames.push_back("tick");
		break;
	case 4:
		uidp.v2data.push_back(&scale);
		uidp.v2datanames.push_back("scale");
		uidp.v2data.push_back(&velocity);
		uidp.v2datanames.push_back("velocity");
		
		uidp.idata.push_back(&amount);
		uidp.idatanames.push_back("amount");
		uidp.fdata.push_back(&tick);
		uidp.fdatanames.push_back("tick");
		break;
	case 5:
		uidp.v4data.push_back(&Color);
		uidp.v4datanames.push_back("Color");
		
		uidp.fdata.push_back(&r);
		uidp.fdatanames.push_back("Radius");
		break;
	case 6:
		uidp.v2data.push_back(&scale);
		uidp.v2datanames.push_back("scale");
		uidp.v4data.push_back(&Color);
		uidp.v4datanames.push_back("Color");
		
		break;
	case 7:
		uidp.v2data.push_back(&velocity);
		uidp.v2datanames.push_back("velocity");
		
		uidp.fdata.push_back(&r);
		uidp.fdatanames.push_back("Radius");
		uidp.fdata.push_back(&attractionStrength);
		uidp.fdatanames.push_back("AttractionStrength");
		uidp.bdata.push_back(&attractive);
		uidp.bdatanames.push_back("Attractive");
		
		break;
	}
	return uidp;
}
std::vector<UI_DataPack> ParticleObject::GetUIData() 
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data[data.size()-1].idata.pop_back();//Z_Index
	data[data.size()-1].idatanames.pop_back();
	data.push_back(GetUIDataParticleObject());
	return data;
}
void ParticleObject::DrawProcess(float dt)
{
	if (t > tick)
		t = 0.0f;
	t+=dt;
	bool texexists = false;
	for(auto tex : AvailableParticleAssets)
	{
		
		if(tex->Name == partAssetName && !texexists && !tex->Delete)
		{
			partAsset = tex;
			texexists=true;
		}
	}
	if(!texexists)
		partAsset = NULL;
	if(partAsset!=NULL)
	{
		switch (po_typeData.x)
		{
		case 0:
			po_influenceSphere.position=position;
			po_influenceSphere.velocity=velocity;
			po_influenceSphere.r = r;
			po_influenceSphere.attractive = attractive;
			po_influenceSphere.attractionStrength = attractionStrength;
			
			partAsset->pe.SpheresOfInfluence.push_back(po_influenceSphere);
			break;
		case 1:
			po_influenceCube.position = position;
			po_influenceCube.scale = scale;
			po_influenceCube.velocity = velocity;
			po_influenceCube.attractive = attractive;
			po_influenceCube.attractionStrength = attractionStrength;
			partAsset->pe.CubesOfInfluence.push_back(po_influenceCube);
			break;
		case 2:
			po_EmitionPoint.position = position;
			po_EmitionPoint.velocity = velocity;
			po_EmitionPoint.amount = amount;
			po_EmitionPoint.tick = tick;
			po_EmitionPoint.t = t;
			partAsset->pe.EmitionPoints.push_back(po_EmitionPoint);
			break;
		case 3:
			po_EmitionCircle.position = position;
			po_EmitionCircle.velocity = velocity;
			po_EmitionCircle.r = r;
			po_EmitionCircle.amount = amount;
			po_EmitionCircle.tick = tick;
			po_EmitionCircle.t = t;
			partAsset->pe.EmitionCircles.push_back(po_EmitionCircle);
			break;
		case 4:
			po_EmitionCube.position = position;
			po_EmitionCube.scale = scale;
			po_EmitionCube.velocity = velocity;
			po_EmitionCube.t = t;
			po_EmitionCube.amount = amount;	
			po_EmitionCube.tick = tick;
			partAsset->pe.EmitionCubes.push_back(po_EmitionCube);
			break;
		case 5:
			po_LightSphere.r = r;
			po_LightSphere.position = position;
			po_LightSphere.Color = Color;
			partAsset->pe.LightSpheres.push_back(po_LightSphere);
			break;
		case 6:
			po_LightCube.position = position;
			po_LightCube.scale = scale;
			po_LightCube.Color = Color;
			partAsset->pe.LightCubes.push_back(po_LightCube);
			break;
		
		case 7:
			po_influenceSphere.position=position;
			po_influenceSphere.velocity=velocity;
			po_influenceSphere.r = r;
			po_influenceSphere.attractive = attractive;
			po_influenceSphere.attractionStrength = attractionStrength;
			
			partAsset->pe.CaptureSpheres.push_back(po_influenceSphere);
		default:
			break;
		}
	}
}
bool ParticleObject::SelectionCheck(glm::vec2 point)
{
	ball b;
	cube NodeCube;
	switch (po_typeData.x)
	{	
	case 0:
		b.r = r;
		b.position = position;
		return BalltoPointCollisionCheck(b,point);
	case 1:
		NodeCube.position =position;
		NodeCube.width = scale.x;
		NodeCube.height = scale.y;
		return PointToQuadCollisionCheck(NodeCube, point);
	case 2:
			
		b.r = 1.0f;
		b.position = position;
		return BalltoPointCollisionCheck(b,point);
	case 3:
		b.r = r;
		b.position = position;
		return BalltoPointCollisionCheck(b,point);
	case 4:
		NodeCube.position =position;
		NodeCube.width = scale.x;
		NodeCube.height = scale.y;
		return PointToQuadCollisionCheck(NodeCube, point);
	case 5:
		b.r = r;
		b.position = position;
		return BalltoPointCollisionCheck(b,point);
	case 6:
		NodeCube.position =position;
		NodeCube.width = scale.x;
		NodeCube.height = scale.y;
		return PointToQuadCollisionCheck(NodeCube, point);
	case 7:
		b.r = r;
		b.position = position;
		return BalltoPointCollisionCheck(b,point);
	default:
		return false;
	}
};
void ParticleObject::OnResize(glm::vec2 prevdif,glm::vec2 mp, glm::vec2 prevmp)
{
	glm::vec2 dif = mp - prevmp;
	r -= length(prevdif);
	r += length(dif);
	scale -= prevdif;
	scale += dif;
}
void ParticleObject::DebugDraw() 
{
	switch (po_typeData.x)
	{	
	case 0:
		DrawCircle(position,r,glm::vec4(1.0f,1.0f,2.0f,0.4f));
		break;
	case 1:
		DrawCube(position,scale,0.0f,glm::vec4(1.0f,1.0f,2.0f,0.4f));
		break;
	case 2:
		DrawCircle(position,1.0f,{1.0f,2.0f,1.0f,1.4f});
		break;
	case 3:
		DrawCircle(position,r,glm::vec4(1.0f,2.0f,1.0f,0.4f));
		break;
	case 4:
		DrawCube(position,scale,0.0f,glm::vec4(1.0f,2.0f,1.0f,0.4f));
		break;
	case 5:
		DrawCircle(position,r,glm::vec4(2.0f,2.0f,1.0f,0.4f));
		break;
	case 6:
		DrawCube(position,scale,0.0f,glm::vec4(2.0f,2.0f,1.0f,0.4f));
		break;
	case 7:
		DrawCircle(position,r,glm::vec4(1.0f,1.0f,2.0f,0.4f));
		break;
	default:
		break;
	}
}


UI_DataPack SoundSource::GetUIDataSoundSource()
{
	UI_DataPack uidp;
	uidp.name = "SoundSource";
	uidp.textdata.push_back(&SoundAssetName);
	uidp.textdatanames.push_back("SoundAsset");	

	uidp.bdata.push_back(&Looping);
	uidp.bdatanames.push_back("Looping");

	uidp.bdata.push_back(&CamRelative);
	uidp.bdatanames.push_back("CamRelative");

	uidp.bdata.push_back(&Temporary);
	uidp.bdatanames.push_back("Temporary");


	uidp.fdata.push_back(&gain);
	uidp.fdatanames.push_back("Gain");

	uidp.fdata.push_back(&pitch);
	uidp.fdatanames.push_back("Pitch");
	
	uidp.v2data.push_back(&velocity);
	uidp.v2datanames.push_back("Velocity");

	uidp.fdata.push_back(&HalfVolumeDist);
	uidp.fdatanames.push_back("HalfVolumeDist");

	uidp.fdata.push_back(&RollOff);
	uidp.fdatanames.push_back("RollOff");

	uidp.fdata.push_back(&MaxDist);
	uidp.fdatanames.push_back("MaxDist");

	

	return uidp;
}
std::vector<UI_DataPack> SoundSource::GetUIData()
{
	std::vector<UI_DataPack> data;
	data.push_back(GetUIDataNode());
	data.push_back(GetUIDataSoundSource());
	return data;
}
void SoundSource::CustomUIDraw(glm::vec2* Corner, float Xstep,float step)
{
	bool b = false;
	Corner->y += UI_button(&b ,"PlayOnce", *Corner + glm::vec2(Xstep,0.0f)).y * -1.0f - step;
	if(b)
	{
		if(soundSource>=0)
		{
			PlaySource(&soundSource);
			//PlaySound(&sound->Sound,{0.0f,0.0f});
		}
	}
	b = false;
	Corner->y += UI_button(&b ,"Reload", *Corner + glm::vec2(Xstep,0.0f)).y * -1.0f - step;
	if(b)
	{
		if(soundSource>=0)
		{
			DeleteSource(&soundSource);
			GenSource(&soundSource);
			SetSourceSound(&soundSource,&sound->Sound);
		}
	}
}
void SoundSource::MTPreProcess()
{	
	bool texexists = false;
	for(auto tex : AvailableSoundAssets)
	{
		
		if(tex->Name == SoundAssetName && !texexists && !tex->Delete)
		{
			sound = tex;
			texexists=true;
			break;
		}
	}

	if(!texexists)
	{
		sound = NULL;
		soundid = -1;
		prevsound = -1;
		changedSound = true;
		return;
	}

	changedSound = prevsound != sound->Sound;
	prevsound =sound->Sound; 
	soundid = sound->Sound;
	if(prevposition != position)
	{
		prevposition = position;
		SetSourcePosition(&soundSource,position);
	}
	if(prevgain != gain)
	{
		prevgain = gain;
		SetSourceGain(&soundSource,gain);
	}
	if(prevpitch != pitch)
	{
		prevpitch = pitch;
		SetSourcePitch(&soundSource,pitch);
	}
	if(prevLooping != Looping)
	{
		prevLooping = Looping;
		SetSourceLooping(&soundSource,Looping);
	}
	if(prevvelocity != velocity)
	{
		prevvelocity = velocity;
		SetSourceVelocity(&soundSource,{velocity,0.0f});
	}
	alGetSourcef(soundSource,AL_SEC_OFFSET,&Time);

	if(HalfVolumeDist != prevHalfVolumeDist)
	{
		prevHalfVolumeDist = HalfVolumeDist;
		SetSourceRefDist(&soundSource, HalfVolumeDist);
	}
	if(RollOff != prevRollOff)
	{
		prevRollOff = RollOff;
		SetSourceRollOff(&soundSource, RollOff);
	}
	if(MaxDist != prevMaxDist)
	{
		prevMaxDist = MaxDist;
		SetSourceMaxDistance(&soundSource, MaxDist);
	}
	if(CamRelative != prevCamRelative)
	{
		prevCamRelative = CamRelative;
		SetSourceRelative(&soundSource,CamRelative);
	}

}
void SoundSource::Play()
{
	if(soundSource>0)
	{
		StopSource(&soundSource);
		DeleteSource(&soundSource);
	}

	GenSource(&soundSource);
	if(NoAsset && noAssetSound!=NULL)
		SetSourceSound(&soundSource,&noAssetSound);
	else if(!NoAsset && sound!=NULL)
		SetSourceSound(&soundSource,&sound->Sound);


	prevposition = position;
	SetSourcePosition(&soundSource,position);
	prevgain = gain;
	SetSourceGain(&soundSource,gain);
	prevpitch = pitch;
	SetSourcePitch(&soundSource,pitch);
	prevLooping = Looping;
	SetSourceLooping(&soundSource,Looping);
	prevvelocity = velocity;
	SetSourceVelocity(&soundSource,{velocity,0.0f});
	prevHalfVolumeDist = HalfVolumeDist;
	SetSourceRefDist(&soundSource, HalfVolumeDist);
	prevRollOff = RollOff;
	SetSourceMaxDistance(&soundSource, RollOff);
	prevMaxDist = MaxDist;
	SetSourceRollOff(&soundSource, MaxDist);
	prevCamRelative = CamRelative;
	SetSourceRelative(&soundSource,CamRelative);

	PlaySource(&soundSource);


}

void SoundSource::PreProcess()
{
	if(!NoAsset && sound==NULL)
	{
		if(soundSource>=0)
		{
			DeleteSource(&soundSource);
			soundSource = -1;
			soundid = -1;
		}
		return;
	}
	else
	{

		if(noAssetSound == NULL)
		{
			if(soundSource>=0)
			{
				DeleteSource(&soundSource);
				soundSource = -1;
				soundid = -1;
			}
			return;
		}		

	}

	if(soundSource ==0)
		GenSource(&soundSource);

	if(changedSound)
	{
		if(soundSource>0)
			DeleteSource(&soundSource);
		GenSource(&soundSource);
		if(NoAsset && noAssetSound!=NULL)
			SetSourceSound(&soundSource,&noAssetSound);
		else if(!NoAsset && sound!=NULL)
			SetSourceSound(&soundSource,&sound->Sound);
		prevposition = position;
		SetSourcePosition(&soundSource,position);
		prevgain = gain;
		SetSourceGain(&soundSource,gain);
		prevpitch = pitch;
		SetSourcePitch(&soundSource,pitch);
		prevLooping = Looping;
		SetSourceLooping(&soundSource,Looping);
		prevvelocity = velocity;
		SetSourceVelocity(&soundSource,{velocity,0.0f});
		prevHalfVolumeDist = HalfVolumeDist;
		SetSourceRefDist(&soundSource, HalfVolumeDist);
		prevRollOff = RollOff;
		SetSourceMaxDistance(&soundSource, RollOff);
		prevMaxDist = MaxDist;
		SetSourceRollOff(&soundSource, MaxDist);
		prevCamRelative = CamRelative;
		SetSourceRelative(&soundSource,CamRelative);
	}
	
	if(soundSource >0)
	{
		if(SourcePlaying(&soundSource))
			wasplaying = true;

		if(Temporary && !SourcePlaying(&soundSource) && wasplaying)
			Delete=true;
	}
}
void SoundSource::DebugDraw()
{

	DrawCircle(position,1.0f,{0.0f,2.0f,0.0f,1.0f});
}
