#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture;
uniform sampler2D Texture2;
uniform vec4 color = vec4(1.0f);

void main()
{             
	vec2 coord =  vec2(TexCoords.x,1.0f - TexCoords.y);
	
	vec4 Color = vec4(mix(texture(Texture2, coord).rgb, texture(Texture, coord).rgb, 1.5),1.0f) * color;    
	
	




	FragColor = Color;
}