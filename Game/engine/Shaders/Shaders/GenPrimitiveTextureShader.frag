#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform int Type;
vec4 fColor = vec4(1.0f);

void main()
{
	
	if(Type ==0)
	{
		vec2 uv = TexCoords *2.0f - vec2(1.0f);
		float dist = uv.x*uv.x+uv.y*uv.y;
	
		if(dist>1.0f)
			discard;
		else 
			color= fColor;
	}
	else 
	{
		color= fColor;
	}
}