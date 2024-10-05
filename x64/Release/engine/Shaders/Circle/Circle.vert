#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aPosScale;
layout (location = 3) in float rotation;

out vec4 fColor;
out vec2 UV;


uniform float aspect;
vec4 pos;

void main()
{
    pos = vec4(aPos.xy,0.0f,1.0f);

    pos.xy = pos.xy * aPosScale.zw;

    
    float Sin = sin(rotation);
    float Cos = cos(rotation);
    pos.xy = vec2(Cos * pos.x - Sin * pos.y, Sin * pos.x + Cos * pos.y);
    
    pos.xy += aPosScale.xy;

    pos.x *=aspect;

    gl_Position =  pos ;
    fColor = aColor;
    UV = aPos.zw;
}  