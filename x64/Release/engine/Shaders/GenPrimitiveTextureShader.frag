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
	else if (Type == 1)
	{
		color= fColor;
	}
	else if (Type == 2)
	{
		vec2 uv = TexCoords *2.0f - vec2(1.0f);
		float dist = uv.x*uv.x+uv.y*uv.y;
	
		if(dist>1.0f)
			discard;
		else 
			color= cos(dist) * fColor *0.25f;
	}
	else if (Type == 3)
	{
		vec2 uv = TexCoords *2.0f - 1.0f;
		bool l1 = uv.y>uv.x;
		bool l2 = uv.y>-uv.x;
		float mult = 0.25f;
		if(l1&&l2)color = mult * vec4((1.0f-uv.y));

		else if(l1 && !l2)color = mult * vec4((TexCoords.x*2.0f)); // left

		else if(!l1 && l2)color = mult * vec4((1.0f-uv.x));

		else if(!l1&&!l2)color = mult * vec4((TexCoords.y*2.0f)); // up (flipped down)

	}
}