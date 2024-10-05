#version 330 core
layout (location = 0) in vec4 aPos;


out vec2 TexCoords;

uniform float angle;
uniform vec2 Aposition;
uniform vec2 scale;
uniform float aspect;


uniform bool flipY = false;
void main()
{

	vec4 pos = vec4(aPos.xy,0.0f,1.0f);

    pos.xy = pos.xy * scale;


    float Sin = sin(angle);
    float Cos = cos(angle);
    pos.xy = vec2(Cos * pos.x - Sin * pos.y, Sin * pos.x + Cos * pos.y);

    pos.xy += Aposition.xy;

    pos.x *=aspect;

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