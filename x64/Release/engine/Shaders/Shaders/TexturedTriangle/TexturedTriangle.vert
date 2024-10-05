#version 330 core
layout (location = 0) in vec4 aPos;


out vec2 TexCoords;

uniform vec2 scr;
uniform bool flipY = false;
void main()
{
	vec4 pos = vec4(aPos.xy,0.0f,1.0f);
	pos.x /=scr.x/scr.y;
	gl_Position =  pos;
	if(!flipY)
	{
		TexCoords.x = aPos.z;
		TexCoords.y = aPos.w;
	}
	else
	{
		TexCoords.x = aPos.z;
		TexCoords.y = 1.0f-aPos.w;
	}
}  