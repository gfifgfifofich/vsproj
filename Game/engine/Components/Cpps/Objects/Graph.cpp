
#include "../../Include/Helper.h"
#include "../../Include/Objects/Graph.h"

#include "../../Include/Text.h"
#include "../../Include/Drawing.h"
void Graph::Inspect(glm::vec2 Coord)
{
	if (Coord.x > position.x && Coord.x < (position.x + scale.x) && Coord.y>position.y && Coord.y < (position.y + scale.y))
	{
		float relpos = (Coord.x - position.x) / scale.x;

		float* therealdata = new float[data.size()];
		int i = 0;
		for (float n : data)
		{
			therealdata[i] = n;
			i++;
		}
		float X = originX + relpos * data.size() * stepX;
		double junk;
		float Y = LinearInterpolation(therealdata[(int)(relpos * data.size())], therealdata[(int)(relpos * data.size()) + 1], modf(relpos * data.size(), &junk));




		std::string pos = "";

		pos += "(";
		pos += std::to_string(X);
		pos.pop_back();
		pos.pop_back();
		pos.pop_back();
		pos.pop_back();
		pos += "; ";
		pos += std::to_string(Y);
		pos.pop_back();
		pos.pop_back();
		pos.pop_back();
		pos.pop_back();
		pos += ")";

		DrawText(pos.c_str(), Coord, 0.5f);
		DrawCircle(position + glm::vec2(relpos * scale.x, originY + (Y / DataScale.y) * scale.y * stepY), 5.0f, { 10.0f,0.0f,0.0f,1.0f });
		delete[] therealdata;
	}
}

void Graph::Inspect(float x)
{
	Inspect(glm::vec2((x)*scale.x * 0.1f - 100, 0.0f));
}

void Graph::Draw(bool NullLines)
{


	float* therealdata = new float[data.size()];
	int i = 0;
	for (float n : data)
	{
		therealdata[i] = n;
		i++;
	}
	if (NullLines)
	{
		DrawLine(position, position + glm::vec2(scale.x, 0.0f));
		DrawLine(position, position + glm::vec2(0.0f, scale.y));
	}

	if(data.size()>0)
	for (int i = 0; i < data.size() - 1; i++)
	{
		float X1 = ((float)i / data.size()) * scale.x;
		float Y1 = originY + (therealdata[i] / DataScale.y) * scale.y * stepY;

		float X2 = ((float)(i + 1) / data.size()) * scale.x;
		float Y2 = originY + (therealdata[i + 1] / DataScale.y) * scale.y * stepY;
		if (X1 >= 0.0f && X1 <= scale.x && Y1 >= 0.0f && Y1 <= scale.y && X2 >= 0.0f && X2 <= scale.x && Y2 >= 0.0f && Y2 <= scale.y)
			DrawLine(position + glm::vec2(X1, Y1), position + glm::vec2(X2, Y2), width, color);
	}

	delete[] therealdata;
}

Graph::Graph(int DataSize, float stepX , float stepY, float originX, float originY)
{
	data.clear();
	data.resize(DataSize);

	this->stepX = stepX;
	this->stepY = stepY;
	this->originX = originX;
	this->originY = originY;

}
