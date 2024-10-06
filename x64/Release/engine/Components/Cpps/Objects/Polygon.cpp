#pragma once

#include "../../Include/Helper.h"
#include "../../Include/Objects/Polygon.h"
#include "../../Include/Drawing.h"


void Triangle::Update_Shape()
	{
		SubTriangles.clear();

		

		midlePosition = { 0.0f,0.0f };
		midlePosition = points[0] + points[1] + points[2];
		midlePosition /= 3;


		lines[0] = line(points[0], points[1]);
		lines[1] = line(points[1], points[2]);
		lines[2] = line(points[2], points[0]);

		if (!(PointToLineSideCheck(midlePosition, lines[0]) &&
			PointToLineSideCheck(midlePosition, lines[1]) &&
			PointToLineSideCheck(midlePosition, lines[2])))
		{
			glm::vec2 swaper = points[1];
			points[1] = points[2];
			points[2] = swaper;
			lines[0] = line(points[0], points[1]);
			lines[1] = line(points[1], points[2]);
			lines[2] = line(points[2], points[0]);
		}




		for (int i = 0; i < 3; i++)
		{
			SubTriangle temp_tri;
			glm::vec2 p1 = lines[i].P1;
			glm::vec2 p2 = lines[0].P1;
			glm::vec2 p3 = midlePosition;
			if (i + 1 < 3) p2 = lines[i + 1].P1;


			temp_tri.l1 = line(p1, p2);
			temp_tri.l1.width = 1.0f;
			temp_tri.l2 = line(p2, p3);
			temp_tri.l2.width = 0.0f;
			temp_tri.l3 = line(p3, p1);
			temp_tri.l3.width = 0.0f;





			SubTriangles.push_back(temp_tri);

		}

	}

Triangle::Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	lines[0] = line(p1, p2);
	lines[1] = line(p2, p3);
	lines[2] = line(p3, p1);
	Update_Shape();
}


void Triangle::DrawOutline(glm::vec4 color )
	{
		DrawLine(points[0], points[1], 2.0f, color);
		DrawLine(points[1], points[2], 2.0f, color);
		DrawLine(points[2], points[0], 2.0f, color);
}
void Triangle::DrawNormals()
{
	glm::vec2 mid1 = points[0] + points[1];
	mid1 *= 0.5f;
	glm::vec2 mid2 = points[1] + points[2];
	mid2 *= 0.5f;
	glm::vec2 mid3 = points[2] + points[0];
		mid3 *= 0.5f;


		DrawLine(mid1, mid1 + lines[0].normal * 50.0f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		DrawLine(mid2, mid2 + lines[1].normal * 50.0f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		DrawLine(mid3, mid3 + lines[2].normal * 50.0f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}




void polygon::Update_MidlePos()
{
	if (Data == NULL)
		return;
	Data->RawmidlePosition = { 0.0f,0.0f };
	for (int i = 0; i < Data->Rawpoints.size(); i++)
		Data->RawmidlePosition += Data->Rawpoints[i];
	Data->RawmidlePosition /= Data->Rawpoints.size();


	RotationMass = 0.0f;
	for (int i = 0; i < Data->Rawpoints.size(); i++)
		RotationMass += length(Data->RawmidlePosition - Data->Rawpoints[i]);

	midlePosition = { 0.0f,0.0f };
	for (int i = 0; i < Transofromedpoints.size(); i++)
	{
		midlePosition += (Data->RawmidlePosition + Rotate((Data->Rawpoints[i] - Data->RawmidlePosition) * Scale, Rotation)) + Position;
	}
	midlePosition /= Data->Rawpoints.size();
}
void polygon::Update_Shape()
{

	if (Data == NULL)
		return;
	
	Transofromedpoints.clear();
	Transofromedpoints.resize(Data->Rawpoints.size());

	midlePosition = {0.0f,0.0f};
	for (int i = 0; i < Transofromedpoints.size(); i++)
	{
		Transofromedpoints[i] = (Data->RawmidlePosition + Rotate((Data->Rawpoints[i] - Data->RawmidlePosition) * Scale, Rotation)) + Position;
		midlePosition += Transofromedpoints[i];
	}
	midlePosition /= Transofromedpoints.size();

	triangles.clear();

	for (int i = 0; i < Data->indexes.size(); i++)
	{
		glm::vec2 p1 = Transofromedpoints[Data->indexes[i].x];
		glm::vec2 p2 = Transofromedpoints[Data->indexes[i].y];
		glm::vec2 p3 = Transofromedpoints[Data->indexes[i].z];

		Triangle t(p1, p2, p3);


		triangles.push_back(t);

	}

}

void polygonData::SaveAs(std::string name)
	{
		std::ofstream SaveFile(name);



		for (int i = 0; i < Rawpoints.size(); i++)
		{
			SaveFile << "p";
			std::string str = std::to_string(Rawpoints[i].x);
			SaveFile << str;
			SaveFile << " ";
			str = std::to_string(Rawpoints[i].y);
			SaveFile << str;
			SaveFile << "\n";

			SaveFile << "t";
			str = std::to_string(TexturePoints[i].x);
			SaveFile << str;
			SaveFile << " ";
			str = std::to_string(TexturePoints[i].y);
			SaveFile << str;
			SaveFile << "\n";
		}
		for (int i = 0; i < indexes.size(); i++)
		{
			SaveFile << "i";
			std::string str = std::to_string(indexes[i].x);
			SaveFile << str;
			SaveFile << " ";
			str = std::to_string(indexes[i].y);
			SaveFile << str;
			SaveFile << " ";
			str = std::to_string(indexes[i].z);
			SaveFile << str;
			SaveFile << "\n";
		}
		
		SaveFile.close();
	}


void polygonData::Load(std::string name)
{
	Rawpoints.clear();
	indexes.clear();
	TexturePoints.clear();
	std::ifstream f(name);
	if (!f.is_open())
		return;
	int i = 0;
	while (!f.eof())
	{
		i++;
		char line[128];
		f.getline(line, 128);
		std::strstream s;
		s << line;

		char junk;
		if (line[0] == 'p')
		{
			glm::vec2 point;
			s >> junk >> point.x >> point.y;
			Rawpoints.push_back(point);
		}
		if (line[0] == 't')
		{
			glm::vec2 point;
			s >> junk >> point.x >> point.y;
			TexturePoints.push_back(point);
		}
		if (line[0] == 'i')
		{
			glm::ivec3 index;
			s >> junk >> index.x >> index.y >> index.z;
			indexes.push_back(index);
		}
		
	}
	Update();
}

void polygonData::Update()
{//VAO
	if(VAO!=NULL)
		glDeleteVertexArrays(1,&VAO);
	glGenVertexArrays(1, &VAO);

	if (VBO != NULL)
		glDeleteBuffers(1, &VBO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);

	float aspx = 1.0f;
	float aspy = 1.0f;

	data.clear();

	for (int i = 0; i < indexes.size(); i++)
	{
	
		data.push_back(Rawpoints[indexes[i].x].x * aspx);
		data.push_back(Rawpoints[indexes[i].x].y * aspy);
		data.push_back(TexturePoints[indexes[i].x].x);
		data.push_back(TexturePoints[indexes[i].x].y);

		data.push_back(Rawpoints[indexes[i].y].x * aspx);
		data.push_back(Rawpoints[indexes[i].y].y * aspy);
		data.push_back(TexturePoints[indexes[i].y].x);
		data.push_back(TexturePoints[indexes[i].y].y);

		data.push_back(Rawpoints[indexes[i].z].x * aspx);
		data.push_back(Rawpoints[indexes[i].z].y * aspy);
		data.push_back(TexturePoints[indexes[i].z].x);
		data.push_back(TexturePoints[indexes[i].z].y);
	}
	


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);



	
	glBindVertexArray(0);


}


void polygonData::add_Point(glm::vec2 point, bool addindex)
{
	Rawpoints.push_back(point);


	float aspx = 1.0f * ScreenDivisorX * CameraScale.x;
	float aspy = 1.0f * ScreenDivisorY * CameraScale.y;

	point.x = (point.x - CameraPosition.x) * aspx;
	point.y = (point.y - CameraPosition.y) * aspy;

	TexturePoints.push_back(point);
	if (addindex)
	{
		if (state == 0)
		{
			glm::ivec3 d;
			d.x = Rawpoints.size() - 1;
			d.y = Rawpoints.size() - 1;
			d.z = Rawpoints.size() - 1;
			indexes.push_back(d);
			state++;
		}
		else if (state == 1)
		{
			indexes[indexes.size() - 1].y = Rawpoints.size() - 1;
			state++;
		}
		else
		{
			indexes[indexes.size() - 1].z = Rawpoints.size() - 1;

			glm::ivec3 d;
			d.x = Rawpoints.size() - 2;
			d.y = Rawpoints.size() - 1;
			d.z = Rawpoints.size() - 2;

			indexes.push_back(d);
		}
	}
	Update();



}


void polygon::DrawOutline(glm::vec4 color)
	{
		for (int i = 0; i < Transofromedpoints.size(); i++)
		{
			if (i + 1 < Transofromedpoints.size())DrawLine(Transofromedpoints[i], Transofromedpoints[i + 1], 2.0f, color);
			else DrawLine(Transofromedpoints[i], Transofromedpoints[0], 2.0f, color);
		}

	}
void polygon::DrawTrianglesOutlines(glm::vec4 color )
	{
		for (int i = 0; i < triangles.size(); i++)
		{
			triangles[i].DrawOutline(color);
		}
	}

void polygon::DrawTriangles()
{
	if (Data == NULL)
		return;
	if (Texture == NULL)
	{
		for (int i = 0; i < Data->indexes.size(); i++)
		{
			DrawTriangle(Transofromedpoints[Data->indexes[i].x], Transofromedpoints[Data->indexes[i].y], Transofromedpoints[Data->indexes[i].z], color);
		}
	}
	else
	{
		for (int i = 0; i < Data->indexes.size(); i++)
		{
			DrawTexturedTriangle(Transofromedpoints[Data->indexes[i].x], Transofromedpoints[Data->indexes[i].y], Transofromedpoints[Data->indexes[i].z], Texture, color, Data->TexturePoints[Data->indexes[i].x], Data->TexturePoints[Data->indexes[i].y], Data->TexturePoints[Data->indexes[i].z]);
		}
	}
}

void polygon::Process(float dt)
{

	Position += Velocity * dt;
	Velocity += Force / Mass * dt;
	Rotation += RotationVelocity * dt;
	RotationVelocity += RotationForce / Mass * dt;
	Update_Shape();
}