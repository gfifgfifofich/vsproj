#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 Color;

uniform sampler2D Texture;
uniform vec4 color = vec4(1.0f);

void main()
{             
	vec2 coord =  vec2(TexCoords.x,TexCoords.y);
	
	FragColor = texture(Texture, coord).rgba * Color;    
	
}