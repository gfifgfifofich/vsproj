#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec4 aPosScale;
layout (location = 4) in float rotation;

out vec4 fColor;

vec4 pos;

uniform float aspect;

void main()
{
    // scale -> rotate -> transform


    pos = vec4(aPos,0.0f,1.0f);

    pos.xy = pos.xy * aPosScale.zw;


    float Sin = sin(rotation);
    float Cos = cos(rotation);
    pos.xy = vec2(Cos * pos.x - Sin * pos.y, Sin * pos.x + Cos * pos.y);

    pos.xy += aPosScale.xy;

    pos.x *=aspect;

    gl_Position =  pos;
    fColor = aColor;
}  