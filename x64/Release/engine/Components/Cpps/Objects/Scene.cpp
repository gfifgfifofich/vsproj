
#include "../../Include/Helper.h"
#include "../../Include/Drawing.h"
#include "../../Include/Objects.h"
#include "../../Include/Objects/Particle.h"
#include "../../Include/UI.h"
#include "../../Include/Collisions.h"
#include "../../Include/Objects/ECS.h"
#include "../../Include/SaveToFile.h"
#include "../../Include/Objects/Scene.h"

void Scene::SaveAs(std::string filename)
{
	DataStorage ds;
	for(int N =0;N<Nodes.size();N++)
	{
		// total size of "Hash string" attachment = 11;
		std::string HashIshString = "";
		std::string Istr = std::to_string(N);
		
		HashIshString += "_";
		for(int i=0;i<10 - Istr.size();i++)
			HashIshString += "0";
		HashIshString += Istr;

		std::string HashedNodeName = Nodes[N]->Name + HashIshString;
		std::vector<UI_DataPack> datapacks;
		datapacks = Nodes[N]->GetUIData();

		ds.SetProperty(HashedNodeName,"Type",ECSType::ECSNODE);
		ds.SetProperty(HashedNodeName,"Class",Nodes[N]->type);

		for(int i=0;i<datapacks.size();i++)
		{
			for(int a = 0; a<datapacks[i].bdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].bdatanames[a],*datapacks[i].bdata[a]);
				
			for(int a = 0; a<datapacks[i].idata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].idatanames[a],*datapacks[i].idata[a]);

			for(int a = 0; a<datapacks[i].fdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].fdatanames[a],*datapacks[i].fdata[a]);

			for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].iSliderdatanames[a],datapacks[i].iSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].fSliderdatanames[a],datapacks[i].fSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].v2data.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].v2datanames[a],*datapacks[i].v2data[a]);

			for(int a = 0; a<datapacks[i].v3data.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].v3datanames[a],*datapacks[i].v3data[a]);

			for(int a = 0; a<datapacks[i].v4data.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].v4datanames[a],*datapacks[i].v4data[a]);

			for(int a = 0; a<datapacks[i].colordata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].colordatanames[a],*datapacks[i].colordata[a]);

			for(int a = 0; a<datapacks[i].textdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].textdatanames[a],*datapacks[i].textdata[a]);

			for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].NoUitextdatanames[a],*datapacks[i].NoUitextdata[a]);

			for(int a = 0; a<datapacks[i].texturedata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].texturedatanames[a],*datapacks[i].texturedata[a]);

			for(int a = 0; a<datapacks[i].materialdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].materialdatanames[a],*datapacks[i].materialdata[a]);

			for(int a = 0; a<datapacks[i].shaderdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].shaderdatanames[a],*datapacks[i].shaderdata[a]);

			for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].ParticleAssetdatanames[a],*datapacks[i].ParticleAssetdata[a]);

			for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].polygonDatadatanames[a],*datapacks[i].polygonDatadata[a]);
			
			for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
			{
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"),datapacks[i].t_texturedata[a]->FileName);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"),datapacks[i].t_texturedata[a]->filter);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"),datapacks[i].t_texturedata[a]->Gradient_Color1);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"),datapacks[i].t_texturedata[a]->Gradient_Color2);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"),datapacks[i].t_texturedata[a]->Noize_Frequency);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"),datapacks[i].t_texturedata[a]->Noize_Layers);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"),datapacks[i].t_texturedata[a]->Size);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"),datapacks[i].t_texturedata[a]->Type);
			}
		}
	}	
	for(int i =0;i<Assets.size();i++)
	{
		// total size of "Hash string" attachment = 11;
		std::string HashIshString = "";
		std::string Istr = std::to_string(i);
		
		HashIshString += "_";
		for(int i=0;i<10 - Istr.size();i++)
			HashIshString += "0";
		HashIshString += Istr;

		std::string HashedAssetName = Assets[i]->Name + HashIshString;
		std::vector<UI_DataPack> datapacks;
		datapacks = Assets[i]->GetUIData();

		ds.SetProperty(HashedAssetName,"Type",ECSType::ECSASSET);
		ds.SetProperty(HashedAssetName,"Class",Assets[i]->type);
		

		for(int i=0;i<datapacks.size();i++)
		{
			for(int a = 0; a<datapacks[i].bdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].bdatanames[a],*datapacks[i].bdata[a]);
				
			for(int a = 0; a<datapacks[i].idata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].idatanames[a],*datapacks[i].idata[a]);

			for(int a = 0; a<datapacks[i].fdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].fdatanames[a],*datapacks[i].fdata[a]);

			for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].iSliderdatanames[a],datapacks[i].iSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].fSliderdatanames[a],datapacks[i].fSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].v2data.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].v2datanames[a],*datapacks[i].v2data[a]);

			for(int a = 0; a<datapacks[i].v3data.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].v3datanames[a],*datapacks[i].v3data[a]);

			for(int a = 0; a<datapacks[i].v4data.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].v4datanames[a],*datapacks[i].v4data[a]);

			for(int a = 0; a<datapacks[i].colordata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].colordatanames[a],*datapacks[i].colordata[a]);

			for(int a = 0; a<datapacks[i].textdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].textdatanames[a],*datapacks[i].textdata[a]);
				
			for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].NoUitextdatanames[a],*datapacks[i].NoUitextdata[a]);

			for(int a = 0; a<datapacks[i].texturedata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].texturedatanames[a],*datapacks[i].texturedata[a]);

			for(int a = 0; a<datapacks[i].materialdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].materialdatanames[a],*datapacks[i].materialdata[a]);

			for(int a = 0; a<datapacks[i].shaderdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].shaderdatanames[a],*datapacks[i].shaderdata[a]);

			for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].ParticleAssetdatanames[a],*datapacks[i].ParticleAssetdata[a]);

			for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].polygonDatadatanames[a],*datapacks[i].polygonDatadata[a]);
			
			for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
			{
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"),datapacks[i].t_texturedata[a]->FileName);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"),datapacks[i].t_texturedata[a]->filter);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"),datapacks[i].t_texturedata[a]->Gradient_Color1);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"),datapacks[i].t_texturedata[a]->Gradient_Color2);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"),datapacks[i].t_texturedata[a]->Noize_Frequency);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"),datapacks[i].t_texturedata[a]->Noize_Layers);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"),datapacks[i].t_texturedata[a]->Size);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"),datapacks[i].t_texturedata[a]->Type);
			}
		}
	}	
	ds.Save(filename);
}

void Scene::LoadFrom(std::string filename)
{
	for(int i =0;i<Nodes.size();i++)
	{
		delete Nodes[i];
	}
	for(int i =0;i<Assets.size();i++)
	{
		delete Assets[i];
	}
	Nodes.clear();
	Assets.clear();

	Collision_polygons.clear();
	Collision_cubes.clear();
	Collision_balls.clear();
	AvailableTextures.clear();
	AvailableMaterials.clear();
	AvailableParticleAssets.clear();
	firstframe = true;
	DataStorage ds;
	ds.Load(filename);
	for(auto Item : ds.data)
	{

		if(ds.GetPropertyAsInt(Item.first,"Type") == ECSType::ECSNODE)
		{
			Node* NewNode = NULL;
			
			// total size of "Hash string" attachment = 11;
			std::string NewNodeName = Item.first;
			for(int i=0;i<11;i++)
				NewNodeName.pop_back();

			int type =-1;

			NewNode = NodeConstructors[ds.GetPropertyAsInt(Item.first,"Class")]();
			if(NewNode == NULL)
			{
				std::cout<<"Error loading Node: " << Item.first;
				continue;
			}
			Nodes.push_back(NewNode);
			for(int i=0;i<2;i++)
			{
				std::vector<UI_DataPack> datapacks;
				datapacks = NewNode->GetUIData();
				
				for(int i=0;i<datapacks.size();i++)
			{
				for(int a = 0; a<datapacks[i].bdata.size();a++)
					*datapacks[i].bdata[a] = ds.GetPropertyAsBool(Item.first,datapacks[i].bdatanames[a]);
					
				for(int a = 0; a<datapacks[i].idata.size();a++)
					*datapacks[i].idata[a] = ds.GetPropertyAsInt(Item.first,datapacks[i].idatanames[a]);

				for(int a = 0; a<datapacks[i].fdata.size();a++)
					*datapacks[i].fdata[a] = ds.GetPropertyAsFloat(Item.first,datapacks[i].fdatanames[a]);

				for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
					datapacks[i].iSliderdata[a]->x = ds.GetPropertyAsInt(Item.first,datapacks[i].iSliderdatanames[a]);

				for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
					datapacks[i].fSliderdata[a]->x = ds.GetPropertyAsFloat(Item.first,datapacks[i].fSliderdatanames[a]);

				for(int a = 0; a<datapacks[i].v2data.size();a++)
					*datapacks[i].v2data[a] = ds.GetPropertyAsVec2(Item.first,datapacks[i].v2datanames[a]);

				for(int a = 0; a<datapacks[i].v3data.size();a++)
					*datapacks[i].v3data[a] = ds.GetPropertyAsVec3(Item.first,datapacks[i].v3datanames[a]);

				for(int a = 0; a<datapacks[i].v4data.size();a++)
					*datapacks[i].v4data[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].v4datanames[a]);

				for(int a = 0; a<datapacks[i].colordata.size();a++)
					*datapacks[i].colordata[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].colordatanames[a]);

				for(int a = 0; a<datapacks[i].textdata.size();a++)
					*datapacks[i].textdata[a] = ds.GetProperty(Item.first,datapacks[i].textdatanames[a]);

				for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
					*datapacks[i].NoUitextdata[a] = ds.GetProperty(Item.first,datapacks[i].NoUitextdatanames[a]);

				for(int a = 0; a<datapacks[i].texturedata.size();a++)
					*datapacks[i].texturedata[a] = ds.GetProperty(Item.first,datapacks[i].texturedatanames[a]);
				
				for(int a = 0; a<datapacks[i].materialdata.size();a++)
					*datapacks[i].materialdata[a] = ds.GetProperty(Item.first,datapacks[i].materialdatanames[a]);

				for(int a = 0; a<datapacks[i].shaderdata.size();a++)
					*datapacks[i].shaderdata[a] = ds.GetProperty(Item.first,datapacks[i].shaderdatanames[a]);

				for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
					*datapacks[i].ParticleAssetdata[a] = ds.GetProperty(Item.first,datapacks[i].ParticleAssetdatanames[a]);

				for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
					*datapacks[i].polygonDatadata[a] = ds.GetProperty(Item.first,datapacks[i].polygonDatadatanames[a]);

				for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
				{
					datapacks[i].t_texturedata[a]->FileName = ds.GetProperty(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"));
					datapacks[i].t_texturedata[a]->filter = ds.GetPropertyAsBool(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"));
					datapacks[i].t_texturedata[a]->Gradient_Color1 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"));
					datapacks[i].t_texturedata[a]->Gradient_Color2 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"));
					datapacks[i].t_texturedata[a]->Noize_Frequency = ds.GetPropertyAsFloat(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"));
					datapacks[i].t_texturedata[a]->Noize_Layers = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"));
					datapacks[i].t_texturedata[a]->Size = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"));
					datapacks[i].t_texturedata[a]->Type = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"));
				}
			}
				NewNode->Name = NewNodeName;
			}
			NewNode->Ready();
		}
		else if(ds.GetPropertyAsInt(Item.first,"Type") == ECSType::ECSASSET)
		{
			Asset* NewAsset = NULL;
			
			// total size of "Hash string" attachment = 11;
			std::string NewAssetName = Item.first;
			for(int i=0;i<11;i++)
				NewAssetName.pop_back();

			int type =-1;

			NewAsset = AssetConstructors[ds.GetPropertyAsInt(Item.first,"Class")]();
			if(NewAsset == NULL)
			{
				std::cout<<"Error loading Asset: " << Item.first;
				continue;
			}
			Assets.push_back(NewAsset);
			for(int i=0;i<2;i++)
			{
				std::vector<UI_DataPack> datapacks;
				datapacks = NewAsset->GetUIData();
				for(int i=0;i<datapacks.size();i++)
				{
					for(int a = 0; a<datapacks[i].bdata.size();a++)
						*datapacks[i].bdata[a] = ds.GetPropertyAsBool(Item.first,datapacks[i].bdatanames[a]);

					for(int a = 0; a<datapacks[i].idata.size();a++)
						*datapacks[i].idata[a] = ds.GetPropertyAsInt(Item.first,datapacks[i].idatanames[a]);

					for(int a = 0; a<datapacks[i].fdata.size();a++)
						*datapacks[i].fdata[a] = ds.GetPropertyAsFloat(Item.first,datapacks[i].fdatanames[a]);

					for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
						datapacks[i].iSliderdata[a]->x = ds.GetPropertyAsInt(Item.first,datapacks[i].iSliderdatanames[a]);

					for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
						datapacks[i].fSliderdata[a]->x = ds.GetPropertyAsFloat(Item.first,datapacks[i].fSliderdatanames[a]);

					for(int a = 0; a<datapacks[i].v2data.size();a++)
						*datapacks[i].v2data[a] = ds.GetPropertyAsVec2(Item.first,datapacks[i].v2datanames[a]);

					for(int a = 0; a<datapacks[i].v3data.size();a++)
						*datapacks[i].v3data[a] = ds.GetPropertyAsVec3(Item.first,datapacks[i].v3datanames[a]);

					for(int a = 0; a<datapacks[i].v4data.size();a++)
						*datapacks[i].v4data[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].v4datanames[a]);

					for(int a = 0; a<datapacks[i].colordata.size();a++)
						*datapacks[i].colordata[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].colordatanames[a]);

					for(int a = 0; a<datapacks[i].textdata.size();a++)
						*datapacks[i].textdata[a] = ds.GetProperty(Item.first,datapacks[i].textdatanames[a]);

					for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
						*datapacks[i].NoUitextdata[a] = ds.GetProperty(Item.first,datapacks[i].NoUitextdatanames[a]);

					for(int a = 0; a<datapacks[i].texturedata.size();a++)
						*datapacks[i].texturedata[a] = ds.GetProperty(Item.first,datapacks[i].texturedatanames[a]);

					for(int a = 0; a<datapacks[i].materialdata.size();a++)
						*datapacks[i].materialdata[a] = ds.GetProperty(Item.first,datapacks[i].materialdatanames[a]);

					for(int a = 0; a<datapacks[i].shaderdata.size();a++)
						*datapacks[i].shaderdata[a] = ds.GetProperty(Item.first,datapacks[i].shaderdatanames[a]);

					for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
						*datapacks[i].ParticleAssetdata[a] = ds.GetProperty(Item.first,datapacks[i].ParticleAssetdatanames[a]);

					for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
						*datapacks[i].polygonDatadata[a] = ds.GetProperty(Item.first,datapacks[i].polygonDatadatanames[a]);

					for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
					{
						datapacks[i].t_texturedata[a]->FileName = ds.GetProperty(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"));
						datapacks[i].t_texturedata[a]->filter = ds.GetPropertyAsBool(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"));
						datapacks[i].t_texturedata[a]->Gradient_Color1 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"));
						datapacks[i].t_texturedata[a]->Gradient_Color2 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"));
						datapacks[i].t_texturedata[a]->Noize_Frequency = ds.GetPropertyAsFloat(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"));
						datapacks[i].t_texturedata[a]->Noize_Layers = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"));
						datapacks[i].t_texturedata[a]->Size = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"));
						datapacks[i].t_texturedata[a]->Type = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"));
					}
				}
				NewAsset->Name = NewAssetName;
			}
			if(LoadAssets)
				NewAsset->Load();
			NewAsset->Ready();
		}
	}	
}

DataStorage Scene::SaveAsds()
{
	DataStorage ds;
	for(int N =0;N<Nodes.size();N++)
	{
		// total size of "Hash string" attachment = 11;
		std::string HashIshString = "";
		std::string Istr = std::to_string(N);
		
		HashIshString += "_";
		for(int i=0;i<10 - Istr.size();i++)
			HashIshString += "0";
		HashIshString += Istr;

		std::string HashedNodeName = Nodes[N]->Name + HashIshString;
		std::vector<UI_DataPack> datapacks;
		datapacks = Nodes[N]->GetUIData();

		ds.SetProperty(HashedNodeName,"Type",ECSType::ECSNODE);
		ds.SetProperty(HashedNodeName,"Class",Nodes[N]->type);

		for(int i=0;i<datapacks.size();i++)
		{
			for(int a = 0; a<datapacks[i].bdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].bdatanames[a],*datapacks[i].bdata[a]);
				
			for(int a = 0; a<datapacks[i].idata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].idatanames[a],*datapacks[i].idata[a]);

			for(int a = 0; a<datapacks[i].fdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].fdatanames[a],*datapacks[i].fdata[a]);

			for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].iSliderdatanames[a],datapacks[i].iSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].fSliderdatanames[a],datapacks[i].fSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].v2data.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].v2datanames[a],*datapacks[i].v2data[a]);

			for(int a = 0; a<datapacks[i].v3data.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].v3datanames[a],*datapacks[i].v3data[a]);

			for(int a = 0; a<datapacks[i].v4data.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].v4datanames[a],*datapacks[i].v4data[a]);

			for(int a = 0; a<datapacks[i].colordata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].colordatanames[a],*datapacks[i].colordata[a]);

			for(int a = 0; a<datapacks[i].textdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].textdatanames[a],*datapacks[i].textdata[a]);

			for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].NoUitextdatanames[a],*datapacks[i].NoUitextdata[a]);

			for(int a = 0; a<datapacks[i].texturedata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].texturedatanames[a],*datapacks[i].texturedata[a]);

			for(int a = 0; a<datapacks[i].materialdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].materialdatanames[a],*datapacks[i].materialdata[a]);

			for(int a = 0; a<datapacks[i].shaderdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].shaderdatanames[a],*datapacks[i].shaderdata[a]);

			for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].ParticleAssetdatanames[a],*datapacks[i].ParticleAssetdata[a]);

			for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
				ds.SetProperty(HashedNodeName,datapacks[i].polygonDatadatanames[a],*datapacks[i].polygonDatadata[a]);
			
			for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
			{
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"),datapacks[i].t_texturedata[a]->FileName);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"),datapacks[i].t_texturedata[a]->filter);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"),datapacks[i].t_texturedata[a]->Gradient_Color1);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"),datapacks[i].t_texturedata[a]->Gradient_Color2);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"),datapacks[i].t_texturedata[a]->Noize_Frequency);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"),datapacks[i].t_texturedata[a]->Noize_Layers);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"),datapacks[i].t_texturedata[a]->Size);
				ds.SetProperty(HashedNodeName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"),datapacks[i].t_texturedata[a]->Type);
			}
		}
	}	
	for(int i =0;i<Assets.size();i++)
	{
		// total size of "Hash string" attachment = 11;
		std::string HashIshString = "";
		std::string Istr = std::to_string(i);
		
		HashIshString += "_";
		for(int i=0;i<10 - Istr.size();i++)
			HashIshString += "0";
		HashIshString += Istr;

		std::string HashedAssetName = Assets[i]->Name + HashIshString;
		std::vector<UI_DataPack> datapacks;
		datapacks = Assets[i]->GetUIData();

		ds.SetProperty(HashedAssetName,"Type",ECSType::ECSASSET);
		ds.SetProperty(HashedAssetName,"Class",Assets[i]->type);
		

		for(int i=0;i<datapacks.size();i++)
		{
			for(int a = 0; a<datapacks[i].bdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].bdatanames[a],*datapacks[i].bdata[a]);
				
			for(int a = 0; a<datapacks[i].idata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].idatanames[a],*datapacks[i].idata[a]);

			for(int a = 0; a<datapacks[i].fdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].fdatanames[a],*datapacks[i].fdata[a]);

			for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].iSliderdatanames[a],datapacks[i].iSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].fSliderdatanames[a],datapacks[i].fSliderdata[a]->x);

			for(int a = 0; a<datapacks[i].v2data.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].v2datanames[a],*datapacks[i].v2data[a]);

			for(int a = 0; a<datapacks[i].v3data.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].v3datanames[a],*datapacks[i].v3data[a]);

			for(int a = 0; a<datapacks[i].v4data.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].v4datanames[a],*datapacks[i].v4data[a]);

			for(int a = 0; a<datapacks[i].colordata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].colordatanames[a],*datapacks[i].colordata[a]);

			for(int a = 0; a<datapacks[i].textdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].textdatanames[a],*datapacks[i].textdata[a]);
				
			for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].NoUitextdatanames[a],*datapacks[i].NoUitextdata[a]);

			for(int a = 0; a<datapacks[i].texturedata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].texturedatanames[a],*datapacks[i].texturedata[a]);

			for(int a = 0; a<datapacks[i].materialdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].materialdatanames[a],*datapacks[i].materialdata[a]);

			for(int a = 0; a<datapacks[i].shaderdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].shaderdatanames[a],*datapacks[i].shaderdata[a]);

			for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].ParticleAssetdatanames[a],*datapacks[i].ParticleAssetdata[a]);

			for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
				ds.SetProperty(HashedAssetName,datapacks[i].polygonDatadatanames[a],*datapacks[i].polygonDatadata[a]);
			
			for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
			{
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"),datapacks[i].t_texturedata[a]->FileName);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"),datapacks[i].t_texturedata[a]->filter);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"),datapacks[i].t_texturedata[a]->Gradient_Color1);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"),datapacks[i].t_texturedata[a]->Gradient_Color2);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"),datapacks[i].t_texturedata[a]->Noize_Frequency);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"),datapacks[i].t_texturedata[a]->Noize_Layers);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"),datapacks[i].t_texturedata[a]->Size);
				ds.SetProperty(HashedAssetName,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"),datapacks[i].t_texturedata[a]->Type);
			}
		}
	}	
	return ds;
}

void Scene::LoadFromds(DataStorage ds)
{
	for(int i =0;i<Nodes.size();i++)
	{
		delete Nodes[i];
	}
	for(int i =0;i<Assets.size();i++)
	{
		delete Assets[i];
	}
	Nodes.clear();
	Assets.clear();

	Collision_polygons.clear();
	Collision_cubes.clear();
	Collision_balls.clear();
	AvailableTextures.clear();
	AvailableMaterials.clear();
	AvailableParticleAssets.clear();
	firstframe = true;
	for(auto Item : ds.data)
	{

		if(ds.GetPropertyAsInt(Item.first,"Type") == ECSType::ECSNODE)
		{
			Node* NewNode = NULL;
			
			// total size of "Hash string" attachment = 11;
			std::string NewNodeName = Item.first;
			for(int i=0;i<11;i++)
				NewNodeName.pop_back();

			int type =-1;

			NewNode = NodeConstructors[ds.GetPropertyAsInt(Item.first,"Class")]();
			if(NewNode == NULL)
			{
				std::cout<<"Error loading Node: " << Item.first;
				continue;
			}
			Nodes.push_back(NewNode);
			for(int i=0;i<2;i++)
			{
				std::vector<UI_DataPack> datapacks;
				datapacks = NewNode->GetUIData();
				
				for(int i=0;i<datapacks.size();i++)
			{
				for(int a = 0; a<datapacks[i].bdata.size();a++)
					*datapacks[i].bdata[a] = ds.GetPropertyAsBool(Item.first,datapacks[i].bdatanames[a]);
					
				for(int a = 0; a<datapacks[i].idata.size();a++)
					*datapacks[i].idata[a] = ds.GetPropertyAsInt(Item.first,datapacks[i].idatanames[a]);

				for(int a = 0; a<datapacks[i].fdata.size();a++)
					*datapacks[i].fdata[a] = ds.GetPropertyAsFloat(Item.first,datapacks[i].fdatanames[a]);

				for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
					datapacks[i].iSliderdata[a]->x = ds.GetPropertyAsInt(Item.first,datapacks[i].iSliderdatanames[a]);

				for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
					datapacks[i].fSliderdata[a]->x = ds.GetPropertyAsFloat(Item.first,datapacks[i].fSliderdatanames[a]);

				for(int a = 0; a<datapacks[i].v2data.size();a++)
					*datapacks[i].v2data[a] = ds.GetPropertyAsVec2(Item.first,datapacks[i].v2datanames[a]);

				for(int a = 0; a<datapacks[i].v3data.size();a++)
					*datapacks[i].v3data[a] = ds.GetPropertyAsVec3(Item.first,datapacks[i].v3datanames[a]);

				for(int a = 0; a<datapacks[i].v4data.size();a++)
					*datapacks[i].v4data[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].v4datanames[a]);

				for(int a = 0; a<datapacks[i].colordata.size();a++)
					*datapacks[i].colordata[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].colordatanames[a]);

				for(int a = 0; a<datapacks[i].textdata.size();a++)
					*datapacks[i].textdata[a] = ds.GetProperty(Item.first,datapacks[i].textdatanames[a]);

				for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
					*datapacks[i].NoUitextdata[a] = ds.GetProperty(Item.first,datapacks[i].NoUitextdatanames[a]);

				for(int a = 0; a<datapacks[i].texturedata.size();a++)
					*datapacks[i].texturedata[a] = ds.GetProperty(Item.first,datapacks[i].texturedatanames[a]);
				
				for(int a = 0; a<datapacks[i].materialdata.size();a++)
					*datapacks[i].materialdata[a] = ds.GetProperty(Item.first,datapacks[i].materialdatanames[a]);

				for(int a = 0; a<datapacks[i].shaderdata.size();a++)
					*datapacks[i].shaderdata[a] = ds.GetProperty(Item.first,datapacks[i].shaderdatanames[a]);

				for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
					*datapacks[i].ParticleAssetdata[a] = ds.GetProperty(Item.first,datapacks[i].ParticleAssetdatanames[a]);

				for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
					*datapacks[i].polygonDatadata[a] = ds.GetProperty(Item.first,datapacks[i].polygonDatadatanames[a]);

				for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
				{
					datapacks[i].t_texturedata[a]->FileName = ds.GetProperty(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"));
					datapacks[i].t_texturedata[a]->filter = ds.GetPropertyAsBool(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"));
					datapacks[i].t_texturedata[a]->Gradient_Color1 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"));
					datapacks[i].t_texturedata[a]->Gradient_Color2 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"));
					datapacks[i].t_texturedata[a]->Noize_Frequency = ds.GetPropertyAsFloat(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"));
					datapacks[i].t_texturedata[a]->Noize_Layers = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"));
					datapacks[i].t_texturedata[a]->Size = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"));
					datapacks[i].t_texturedata[a]->Type = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"));
				}
			}
				NewNode->Name = NewNodeName;
			}
			NewNode->Ready();
		}
		else if(ds.GetPropertyAsInt(Item.first,"Type") == ECSType::ECSASSET)
		{
			Asset* NewAsset = NULL;
			
			// total size of "Hash string" attachment = 11;
			std::string NewAssetName = Item.first;
			for(int i=0;i<11;i++)
				NewAssetName.pop_back();

			int type =-1;

			NewAsset = AssetConstructors[ds.GetPropertyAsInt(Item.first,"Class")]();
			if(NewAsset == NULL)
			{
				std::cout<<"Error loading Asset: " << Item.first;
				continue;
			}
			Assets.push_back(NewAsset);
			for(int i=0;i<2;i++)
			{
				std::vector<UI_DataPack> datapacks;
				datapacks = NewAsset->GetUIData();
				for(int i=0;i<datapacks.size();i++)
				{
					for(int a = 0; a<datapacks[i].bdata.size();a++)
						*datapacks[i].bdata[a] = ds.GetPropertyAsBool(Item.first,datapacks[i].bdatanames[a]);

					for(int a = 0; a<datapacks[i].idata.size();a++)
						*datapacks[i].idata[a] = ds.GetPropertyAsInt(Item.first,datapacks[i].idatanames[a]);

					for(int a = 0; a<datapacks[i].fdata.size();a++)
						*datapacks[i].fdata[a] = ds.GetPropertyAsFloat(Item.first,datapacks[i].fdatanames[a]);

					for(int a = 0; a<datapacks[i].iSliderdata.size();a++)
						datapacks[i].iSliderdata[a]->x = ds.GetPropertyAsInt(Item.first,datapacks[i].iSliderdatanames[a]);

					for(int a = 0; a<datapacks[i].fSliderdata.size();a++)
						datapacks[i].fSliderdata[a]->x = ds.GetPropertyAsFloat(Item.first,datapacks[i].fSliderdatanames[a]);

					for(int a = 0; a<datapacks[i].v2data.size();a++)
						*datapacks[i].v2data[a] = ds.GetPropertyAsVec2(Item.first,datapacks[i].v2datanames[a]);

					for(int a = 0; a<datapacks[i].v3data.size();a++)
						*datapacks[i].v3data[a] = ds.GetPropertyAsVec3(Item.first,datapacks[i].v3datanames[a]);

					for(int a = 0; a<datapacks[i].v4data.size();a++)
						*datapacks[i].v4data[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].v4datanames[a]);

					for(int a = 0; a<datapacks[i].colordata.size();a++)
						*datapacks[i].colordata[a] = ds.GetPropertyAsVec4(Item.first,datapacks[i].colordatanames[a]);

					for(int a = 0; a<datapacks[i].textdata.size();a++)
						*datapacks[i].textdata[a] = ds.GetProperty(Item.first,datapacks[i].textdatanames[a]);

					for(int a = 0; a<datapacks[i].NoUitextdata.size();a++)
						*datapacks[i].NoUitextdata[a] = ds.GetProperty(Item.first,datapacks[i].NoUitextdatanames[a]);

					for(int a = 0; a<datapacks[i].texturedata.size();a++)
						*datapacks[i].texturedata[a] = ds.GetProperty(Item.first,datapacks[i].texturedatanames[a]);

					for(int a = 0; a<datapacks[i].materialdata.size();a++)
						*datapacks[i].materialdata[a] = ds.GetProperty(Item.first,datapacks[i].materialdatanames[a]);

					for(int a = 0; a<datapacks[i].shaderdata.size();a++)
						*datapacks[i].shaderdata[a] = ds.GetProperty(Item.first,datapacks[i].shaderdatanames[a]);

					for(int a = 0; a<datapacks[i].ParticleAssetdata.size();a++)
						*datapacks[i].ParticleAssetdata[a] = ds.GetProperty(Item.first,datapacks[i].ParticleAssetdatanames[a]);

					for(int a = 0; a<datapacks[i].polygonDatadata.size();a++)
						*datapacks[i].polygonDatadata[a] = ds.GetProperty(Item.first,datapacks[i].polygonDatadatanames[a]);

					for(int a = 0; a<datapacks[i].t_texturedata.size();a++)
					{
						datapacks[i].t_texturedata[a]->FileName = ds.GetProperty(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("FileName"));
						datapacks[i].t_texturedata[a]->filter = ds.GetPropertyAsBool(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("filter"));
						datapacks[i].t_texturedata[a]->Gradient_Color1 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color1"));
						datapacks[i].t_texturedata[a]->Gradient_Color2 = ds.GetPropertyAsVec4(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Gradient_Color2"));
						datapacks[i].t_texturedata[a]->Noize_Frequency = ds.GetPropertyAsFloat(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Frequency"));
						datapacks[i].t_texturedata[a]->Noize_Layers = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Noize_Layers"));
						datapacks[i].t_texturedata[a]->Size = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Size"));
						datapacks[i].t_texturedata[a]->Type = ds.GetPropertyAsInt(Item.first,datapacks[i].t_texturedatanames[a] + "_" + std::string("Type"));
					}
				}
				NewAsset->Name = NewAssetName;
			}
			if(LoadAssets)
				NewAsset->Load();
			NewAsset->Ready();
		}
	}	

}



void Scene::Rescale(glm::vec2 scale, int Z_Index)
{
	//for (int i = 0; i < points.size(); i++)
		// {
		// 	points[i].position.x *= scale.x;
		// 	points[i].position.y *= scale.y;
		// }
		// for (int i = 0; i < balls.size(); i++)
		// {
		// 	balls[i].position *= scale;
		// 	balls[i].r *= scale.x;
		// 	balls[i].Z_Index += Z_Index;
		// }
		// for (int i = 0; i < cubes.size(); i++)
		// {
		// 	cubes[i].position *= scale;
		// 	cubes[i].width *= scale.x;
		// 	cubes[i].height *= scale.x;
		// 	cubes[i].Z_Index += Z_Index;
		// }
		// for (int i = 0; i < polygons.size(); i++)
		// {/*
		// 	for (int a = 0; a < polygons[i].Rawpoints.size(); a++)
		// 		polygons[i].Rawpoints[a] *= scale;*/
		// 	polygons[i].Scale += scale;
		// 	polygons[i].Position *= scale;
		// 	polygons[i].Z_Index += Z_Index;
		// 	polygons[i].Update_MidlePos();
		// 	polygons[i].Update_Shape();


		// }
		// for (int i = 0; i < ParticleEmiters.size(); i++)
		// {/*
		// 	ParticleEmiters[i].StartSize *= scale;
		// 	ParticleEmiters[i].EndSize *= scale;
		// 	ParticleEmiters[i].InitialVelocity *= scale;
		// 	ParticleEmiters[i].VelocityRandomness.x *= scale.x;
		// 	ParticleEmiters[i].VelocityRandomness.y *= scale.x;
		// 	ParticleEmiters[i].VelocityRandomness.z *= scale.y;
		// 	ParticleEmiters[i].VelocityRandomness.w *= scale.y;*/
		// 	ParticleEmiters[i].Z_Index += Z_Index;

		// 	for (int a = 0; a < ParticleEmiters[i].CubesOfInfluence.size(); a++)
		// 	{
		// 		ParticleEmiters[i].CubesOfInfluence[a].position *= scale;
		// 		ParticleEmiters[i].CubesOfInfluence[a].scale *= scale;
		// 		ParticleEmiters[i].CubesOfInfluence[a].velocity *= scale;
		// 	}
		// 	for (int a = 0; a < ParticleEmiters[i].EmitionCircles.size(); a++)
		// 	{
		// 		ParticleEmiters[i].EmitionCircles[a].position *= scale;
		// 		ParticleEmiters[i].EmitionCircles[a].r *= scale.x;
		// 		ParticleEmiters[i].EmitionCircles[a].velocity *= scale;
		// 	}
		// 	for (int a = 0; a < ParticleEmiters[i].EmitionCubes.size(); a++)
		// 	{
		// 		ParticleEmiters[i].EmitionCubes[a].position *= scale;
		// 		ParticleEmiters[i].EmitionCubes[a].scale *= scale;
		// 		ParticleEmiters[i].EmitionCubes[a].velocity *= scale;
		// 	}
		// 	for (int a = 0; a < ParticleEmiters[i].EmitionPoints.size(); a++)
		// 	{
		// 		ParticleEmiters[i].EmitionPoints[a].position *= scale;
		// 		ParticleEmiters[i].EmitionPoints[a].velocity *= scale;
		// 	}
		// 	for (int a = 0; a < ParticleEmiters[i].LightCubes.size(); a++)
		// 	{
		// 		ParticleEmiters[i].LightCubes[a].position *= scale;
		// 		ParticleEmiters[i].LightCubes[a].scale *= scale;
		// 	}
		// 	for (int a = 0; a < ParticleEmiters[i].LightSpheres.size(); a++)
		// 	{
		// 		ParticleEmiters[i].LightSpheres[a].position *= scale;
		// 		ParticleEmiters[i].LightSpheres[a].r *= scale.x;
		// 	}
		// 	for (int a = 0; a < ParticleEmiters[i].SpheresOfInfluence.size(); a++)
		// 	{
		// 		ParticleEmiters[i].SpheresOfInfluence[a].position *= scale;
		// 		ParticleEmiters[i].SpheresOfInfluence[a].r *= scale.x;
		// 		ParticleEmiters[i].SpheresOfInfluence[a].velocity *= scale;
		// 	}
		// }


		// for (int i = 0; i < LightSources.size(); i++)
		// {
		// 	LightSources[i].position.x *= scale.x;
		// 	LightSources[i].position.y *= scale.y;
		// 	LightSources[i].scale *= scale;

		// }
}

void Scene::Update()
{	
	SceneInProcess = this;
	if(firstframe)
	{
		firstframe = false;
		for(int i=0;i<Nodes.size();i++)
			Nodes[i]->Ready();
		for(int i=0;i<Assets.size();i++)
			Assets[i]->Ready();
	}
	int ii =0;
	AvailableTextures.clear();
	AvailableMaterials.clear();
	AvailableParticleAssets.clear();
	AvailableSoundAssets.clear();
	while(ii<Assets.size())
	{
		if(Assets[ii]->framesUntillDeletion <=0)
		{
			if(Assets.size()>0)
				Assets[ii] = Assets[Assets.size()-1];
			Assets.pop_back();
		}
		else
		{

			for(int a =0;a<Assets[ii]->UsedAssets.size();a++)
			{
				if((*Assets[ii]->UsedAssets[a])!=NULL)
					if((*Assets[ii]->UsedAssets[a])->Delete)
						(*Assets[ii]->UsedAssets[a]) = NULL;
			}
			if(Assets[ii]->Delete)
				Assets[ii]->framesUntillDeletion--;

			if(Assets[ii]->type == AssetType::TEXTUREOBJECT)
				AvailableTextures.push_back((TextureObject*)Assets[ii]);

			if(Assets[ii]->type == AssetType::MATERIALOBJECT)
				AvailableMaterials.push_back((MaterialObject*)Assets[ii]);

			if(Assets[ii]->type == AssetType::PARTICLEASSET)
				AvailableParticleAssets.push_back((ParticleAsset*)Assets[ii]);

			if(Assets[ii]->type == AssetType::SOUNDASSET)
				AvailableSoundAssets.push_back((SoundAsset*)Assets[ii]);

			ii++;
		}
	}
	


	Collision_balls.clear();
	Collision_cubes.clear();
	Collision_polygons.clear();
	ii =0;
	while(ii<Nodes.size())
	{

		if(filter && filter_object != Nodes[ii]->type)
			continue;
		if(Nodes[ii]->framesUntillDeletion <=0)
		{
			if(Nodes.size()>0)
				Nodes[ii] = Nodes[Nodes.size()-1];
			Nodes.pop_back();
		}
		else
		{

			if(Nodes[ii]->Delete)
				Nodes[ii]->framesUntillDeletion--;
			for(int a =0;a<Nodes[ii]->UsedAssets.size();a++)
			{
				if((*Nodes[ii]->UsedAssets[a])!=NULL)
					if((*Nodes[ii]->UsedAssets[a])->Delete)
						(*Nodes[ii]->UsedAssets[a]) = NULL;
			}
			//if(Nodes[ii]->type == NodeType::CO_BALL)
			//	Collision_balls.push_back(&((CO_Ball*)Nodes[ii])->b);
//
			//if(Nodes[ii]->type == NodeType::CO_CUBE)
			//	Collision_cubes.push_back(&((CO_Cube*)Nodes[ii])->c);
//
			//if(Nodes[ii]->type == NodeType::CO_POLYGON)
			//	Collision_polygons.push_back(&((CO_Polygon*)Nodes[ii])->p);
		
			Nodes[ii]->PreProcess();
			ii++;
		}
	}
}

void Scene::Draw(float dt)
{
	for(int i=0;i<Nodes.size();i++)
	{
		if(filter && filter_object != Nodes[i]->type)
			continue;
		if(DrawRegularScene)
		{
			Nodes[i]->DrawProcess(dt);	
			Nodes[i]->Draw();
		}
		if(DrawCollisions)
		{
			Nodes[i]->DebugDraw();
		}
	}
	for(int i=0;i<Assets.size();i++)
	{
		if(DrawRegularScene)
			Assets[i]->DrawProcess(dt);	
	}
}
void Scene::Process(float dt)
{
	SceneInProcess = this;
	

	for(int i=0;i<Assets.size();i++)
	{
		Assets[i]->Process(dt);
		if(DrawCollisions)
			Assets[i]->DebugProcess(dt);
	}
	
	for(int i=0;i<Nodes.size();i++)
	{
		if(filter && filter_object != Nodes[i]->type)
			continue;

		for(int a =0;a<Nodes[i]->UsedAssets.size();a++)
		{
			
			if((*Nodes[i]->UsedAssets[a])!=NULL)
				if((*Nodes[i]->UsedAssets[a])->Delete)
					(*Nodes[i]->UsedAssets[a]) = NULL;
		}



		Nodes[i]->Process(dt);
		
		if(DrawCollisions)
			Nodes[i]->DebugProcess(dt);
		
	}

	SceneInProcess = NULL;
}
void Scene::DeleteNormalMaps()
{
	
}
void Scene::DeleteTextures()
{
	
}
void Scene::ReloadTextures()
{
	
}