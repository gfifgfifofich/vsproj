#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float Angle;

uniform sampler2D Texture;
uniform sampler2D Texture2;

uniform bool generated = false;
uniform bool AlphaTexture = true;

void main()
{

	vec4 SurfaceNormal =  texture(Texture, TexCoords.xy);
	
		if(!generated)
		{
			SurfaceNormal.x = SurfaceNormal.x*2.0f-1.0f;
			SurfaceNormal.y = SurfaceNormal.y*2.0f-1.0f;
		}
		float Cos = cos(Angle);
		float Sin = sin(Angle);
	
		if(!AlphaTexture)
		{
			if(Cos!=0.0f||Sin!=0.0f)
			{
				vec4 col = SurfaceNormal;
				SurfaceNormal.xy = vec2(Cos*col.x - Sin*col.y, Sin*col.x + Cos*col.y);
				FragColor = SurfaceNormal;
			}
			else
				FragColor = SurfaceNormal;
		}
		else
		{
			float alpha = texture(Texture2, TexCoords.xy).w;
			if(Cos!=0.0f||Sin!=0.0f)
			{
				if(alpha > 0.0f)
				{
					vec4 col = SurfaceNormal;
					SurfaceNormal.xy = vec2(Cos*col.x - Sin*col.y, Sin*col.x + Cos*col.y);
					FragColor = SurfaceNormal;
				}	
				else
					discard;
			}
			else if(alpha > 0.0f)
				FragColor = SurfaceNormal;
			else
				discard;

		}

}