#version 330 core
layout (location = 0) in vec4 aPos;

out vec2 UV;

uniform vec2 position;
uniform vec2 scale;
uniform float rotation;
uniform float aspect;
void main()
{
	vec4 pos = vec4(aPos.xy,0.0f,1.0f);

    pos.xy = pos.xy * scale;


    float Sin = sin(rotation);
    float Cos = cos(rotation);
    pos.xy = vec2(Cos * pos.x - Sin * pos.y, Sin * pos.x + Cos * pos.y);

    pos.xy += position.xy;

    pos.x *=aspect;
    gl_Position =  pos;
	UV.x = aPos.z;
	UV.y = 1.0f - aPos.w;
}  