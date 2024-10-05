#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec4 Color1 = vec4(1.0f);
uniform vec4 Color2 = vec4(1.0f,1.0f,1.0f,0.0f);

void main()
{   
    FragColor = Color1 * (1.0f-TexCoords.x) + Color2 * (TexCoords.x);
}