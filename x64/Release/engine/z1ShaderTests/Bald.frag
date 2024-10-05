#version 330 core
out vec4 FragColor;

in vec2 UV;


uniform float BaldnessX = 0.2f;
uniform float BaldnessY = 0.2f;

void main()
{
	
	if(UV.x-BaldnessX >0.0f && UV.x <1.0f-BaldnessX || UV.y-BaldnessY >0.0f && UV.y <1.0f-BaldnessY )
		FragColor = vec4(1.0f); 
	else
	{
		vec2 v = vec2(1.0f);

		if(UV.x>0.5f && UV.y>0.5f)
		{
			v = UV - vec2(1.0f - BaldnessX,1.0f - BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		else if(UV.x<0.5f && UV.y>0.5f)
		{
			v = UV - vec2(BaldnessX,1.0f - BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		else if(UV.x>0.5f && UV.y<0.5f)
		{
			v = UV - vec2(1.0f - BaldnessX,BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		else if(UV.x<0.5f && UV.y<0.5f)
		{
			v = UV - vec2(BaldnessX,BaldnessY);
			v.x/=BaldnessX;
			v.y/=BaldnessY;
		}
		
		float d = v.x*v.x+v.y*v.y;
		if(d<1.0f)
			FragColor = vec4(1.0f,1.0f,1.0f,1.0f); 
		else
			FragColor = vec4(0.0f); 
			

	}

}