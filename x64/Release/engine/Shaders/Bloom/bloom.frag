#version 330 core
out vec4 Color;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{           
	vec4 FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
	if(FragColor.r>1.0f || FragColor.g>1.0f||FragColor.b>1.0f)
	{
		FragColor.rgb-=1.0f;
		if(FragColor.r < 0.0f)
		   FragColor.r=0.0f;
		   
		if(FragColor.g < 0.0f)
		   FragColor.g=0.0f;
		   
		if(FragColor.b < 0.0f)
		   FragColor.b=0.0f;

		FragColor.r += FragColor.g*0.05f + FragColor.b*0.05f;
		FragColor.g += FragColor.r*0.05f + FragColor.b*0.05f;
		FragColor.b += FragColor.g*0.05f + FragColor.r*0.05f;
		
		Color = vec4(FragColor.rgb, 1.0);
	}
	else Color = vec4(0.0f,0.0f,0.0f,1.0f);
	
	
}