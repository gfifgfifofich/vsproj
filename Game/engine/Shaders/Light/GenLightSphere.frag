#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


void main()
{             
	vec2 TC = TexCoords*2.0f - 1.0f;

	float dist = TC.x*TC.x+TC.y*TC.y;
	if(dist<1.0f)
		{
			FragColor = vec4(1.0f-sqrt(dist));
		}
	else discard;
}