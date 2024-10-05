#version 330 core
in vec4 fColor;
in vec2 UV;

out vec4 color;


void main()
{
	

	vec2 uv = UV *2.0f - vec2(1.0f);
	float dist = uv.x*uv.x+uv.y*uv.y;
	
	if(dist>1.0f)
		color= vec4(0.0f,0.0f,0.0f,0.0f);
	else 
		color= fColor;

}