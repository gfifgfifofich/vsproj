#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int Type =0;

void main()
{             
	vec2 TC = TexCoords*2.0f - 1.0f;

	if(Type==0)
	{
		float dist = TC.x*TC.x+TC.y*TC.y;


		if(dist<=1.0f)
		{
			vec2 NTC = TC/sqrt(dist);
			vec4 NM = vec4(NTC*dist,2.0f,1.0f);
			FragColor = NM;
		}
		else discard;
	}
	if(Type==1)
	{
		bool l1 = TC.y>TC.x;
		bool l2 = TC.y>-TC.x;

		if(l1&&l2)FragColor = vec4(0.0f,1.0f,2.0f,1.0f);
		else if(l1&&!l2)FragColor = vec4(-1.0f,0.0f,2.0f,1.0f);
		else if(!l1&&l2)FragColor = vec4(1.0f,0.0f,2.0f,1.0f);
		else if(!l1&&!l2)FragColor = vec4(0.0f,-1.0f,2.0f,1.0f);
	}
}