#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture;
uniform vec4 color = vec4(1.0f);

void main()
{             
	vec2 coord =  vec2(TexCoords.x,1.0f - TexCoords.y);
	FragColor =texture(Texture, coord).rgba * color;
}