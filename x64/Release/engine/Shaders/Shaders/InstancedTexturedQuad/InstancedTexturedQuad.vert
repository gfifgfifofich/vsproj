#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in float rotation;
layout (location = 2) in vec4 aPosScale;
layout (location = 3) in vec4 aColor;


out vec2 TexCoords;
out vec4 Color;

uniform bool flipY = false;
uniform float aspect;
uniform bool flipX = false;
void main()
{
	vec4 pos = vec4(aPos.xy,0.0f,1.0f);

    pos.xy = pos.xy * aPosScale.zw;


    float Sin = sin(rotation);
    float Cos = cos(rotation);
    pos.xy = vec2(Cos * pos.x - Sin * pos.y, Sin * pos.x + Cos * pos.y);

    pos.xy += aPosScale.xy;

    pos.x *=aspect;

	gl_Position = pos;
	Color = aColor;

    if(!flipX)
	    TexCoords.x = aPos.z;
	else
        TexCoords.x = 1.0f - aPos.z;

     if(flipY)
	    TexCoords.y = aPos.w;
	else
        TexCoords.y = 1.0f - aPos.w;
}  