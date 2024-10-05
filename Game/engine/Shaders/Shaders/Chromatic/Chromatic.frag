#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform vec2 point = vec2(0.5f); 
uniform float strength = 1.0f; 

void main()
{             
	
	vec2 offset = point - TexCoords ;
	//offset *=2000;
	//offset = offset;
	//offset *=0.000000000525f;
	offset *=strength*10.0f;
	
	vec3 result = texture(image, TexCoords).rgb;
	result.r = texture(image, TexCoords+offset*strength*0.00f).r;
	result.g = texture(image, TexCoords+offset*strength*0.01f).g;
	result.b = texture(image, TexCoords+offset*strength*0.02f).b;


	FragColor = vec4(result, 1.0);
}