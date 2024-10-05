#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 Color;

uniform float BaldnessX = 0.05f;
uniform float BaldnessY = 0.2f;

void main()
{
	
	if(TexCoords.x-BaldnessX >0.0f && TexCoords.x <1.0f-BaldnessX || TexCoords.y-BaldnessY >0.0f && TexCoords.y <1.0f-BaldnessY )
		FragColor = Color; 
	else
	{
		vec2 v = vec2(1.0f);

		if(TexCoords.x>0.5f && TexCoords.y>0.5f)
		{
			v = TexCoords - vec2(1.0f - BaldnessX,1.0f - BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		else if(TexCoords.x<0.5f && TexCoords.y>0.5f)
		{
			v = TexCoords - vec2(BaldnessX,1.0f - BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		else if(TexCoords.x>0.5f && TexCoords.y<0.5f)
		{
			v = TexCoords - vec2(1.0f - BaldnessX,BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		else if(TexCoords.x<0.5f && TexCoords.y<0.5f)
		{
			v = TexCoords - vec2(BaldnessX,BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		
		float d = v.x*v.x+v.y*v.y;
		if(d<1.0f)
			FragColor = Color; 
		else
			FragColor = vec4(0.0f); 
			

	}

}