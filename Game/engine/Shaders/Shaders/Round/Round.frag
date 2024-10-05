#version 330 core

out vec4 Color;

in vec2 TexCoords;

uniform sampler2D Texture;

uniform float size = 1.0f;




void main()
{
	
	
	vec2 uv = TexCoords *2.0f - vec2(1.0f);
	float dist = uv.x*uv.x+uv.y*uv.y;
	float col  = smoothstep(1.0f,0.0f,dist*size);
		
	

	Color =texture(Texture,TexCoords)*col;
}