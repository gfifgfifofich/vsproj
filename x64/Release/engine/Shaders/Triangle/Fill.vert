#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 scr;
void main()
{

    vec4 pos = vec4(aPos,0.0f,1.0f);
    pos.x /=scr.x/scr.y;
    gl_Position =  pos;
}  