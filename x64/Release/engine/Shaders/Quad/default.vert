#version 330 core
layout (location = 0) in vec4 aPos;

out vec2 UV;

uniform vec2 position = vec2(0.0f);
uniform vec2 scale = vec2(1.0f);
uniform float rotation = 0.0f;
uniform float aspect = 1.0f;
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