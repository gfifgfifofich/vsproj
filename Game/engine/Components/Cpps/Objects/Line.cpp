#include "../../Include/Helper.h"
#include "../../Include/Objects/Line.h"

line::line(glm::vec2 p1, glm::vec2 p2, bool Calculate_normal)
{
	P1 = p1;
	P2 = p2;
	glm::vec2 d = P2 - P1;
	if (Calculate_normal) normal = glm::vec2(-d.y, d.x) / length(d);
}

void line::Calculate_normal()
{
	glm::vec2 d = P2 - P1;
	normal = glm::vec2(-d.y, d.x) / length(d);
}


