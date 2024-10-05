#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 AngleDepth;

uniform sampler2D Texture;
uniform sampler2D Texture2;
uniform sampler2D HeightMap;

uniform bool generated = false;
uniform bool HeightMapped = false;
uniform bool AlphaTexture = true;

void main()
{

	vec4 SurfaceNormal =  vec4(texture(Texture, TexCoords.xy).xyz, AngleDepth.y);

	if(SurfaceNormal.x<=0.1f && SurfaceNormal.y<=0.1f && SurfaceNormal.z<=0.1f)
		discard;

	if(HeightMapped)
		SurfaceNormal.a += ( texture(HeightMap, TexCoords.xy).x);
	
	if(!generated)
	{
		SurfaceNormal.x = SurfaceNormal.x*2.0f-1.0f;
		SurfaceNormal.y = SurfaceNormal.y*2.0f-1.0f;
	}
	float Cos = cos(AngleDepth.x);
	float Sin = sin(AngleDepth.x);

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