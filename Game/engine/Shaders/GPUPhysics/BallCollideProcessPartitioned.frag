#version 330 core
in vec2 TexCoords;
out vec4 FragValue;
uniform int Cellcount = 1;
uniform int CellYcount = 1;
uniform int Cellsize = 1;
uniform int BallsCount = 1;
uniform int TexturePositionsCount = 1;
uniform float delta = 0.017f;
uniform float Radius = 5.0f;
uniform vec2 force = vec2(0.0f);

uniform sampler2D Balls; 
uniform sampler2D PackedBalls; 
uniform sampler2D TexturePositions; 
float invbc = 0.1f; 
float DOT(vec2 v1, vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
float sqrlength(vec2 p)
{
	return (p.x*p.x + p.y*p.y);
}

vec4 GetBall(int i)
{
	return texture(Balls,vec2( (float(i+0.2f) * invbc),0.0f));
}

vec4 GetBallCell(int i,float cellid)
{
	return texture(PackedBalls,vec2( (float(i+0.2f) / Cellsize),   float(cellid+0.2f) / float(Cellcount)) );
}



vec4 Run()
{
	
	invbc = 1.0f / BallsCount;
	vec4 b1 = texture(Balls, TexCoords);
	vec3 CellPos = texture(TexturePositions, TexCoords).xyz;
	float CellId = CellPos.x + CellPos.y * CellYcount;
	//b1 = GetBallCell(int(CellPos.z+0.4f),CellId);

	vec4 buff = b1;
	float cellids[5];
	cellids[0] = CellId;
	cellids[1] = CellId-1.0f;
	cellids[2] = CellId+1.0f;
	cellids[3] = CellId-CellYcount;
	cellids[4] = CellId+CellYcount;
	for(int c = 0; c < 5 ; c++)
	{
		for(int i=0;i<Cellsize;i++)
		{
			vec4 b2 = GetBallCell(i,cellids[c]);
			if(b2.x+b2.y+b2.z+b2.w <0.0001f)// noball
				break;
			vec2 dif = vec2(b2.x - buff.x,b2.y - buff.y);
			if ((dif.x * dif.x + dif.y * dif.y) < (Radius + Radius) * (Radius + Radius))
			{


				float dist = length(dif);
				float distancedifference = (Radius + Radius) - dist;
				if(sqrlength(buff.xy - b2.xy)< 0.001f)
					continue;
				if(dist<0.00000001f)
				{
					//b1.xy += vec2(TexCoords); 
					continue;
				}

				vec2 n2 = dif/dist;
				vec2 n1 = -n2;
				vec2 CollisionLine = vec2(-n1.y, n1.x);



				float energy1 = DOT(buff.zw, n2);
				float energy2 = DOT(b2.zw, n2);


				b1.xy = b1.xy + n1.xy * distancedifference * 0.5f;
				b1.zw -= n1 * energy2 * 0.7f + n2 * energy1;
			}
		}
	}
	b1.z += force.x * delta;  // velocity += force
	b1.w += force.y * delta;  // velocity += force
	b1.x += b1.z * delta;//pos+=velocity
	b1.y += b1.w * delta;//pos+=velocity
	float roughness = 0.5f;
	if(b1.x>2000.0f)
	{
		b1.x = 2000.0f;
		b1.z *= -roughness;
		b1.w *= roughness;
	}
	if(b1.x<20.0f)
	{
		b1.x = 20.0f;
		b1.z *= -roughness;
		b1.w *= roughness;
	}
	if(b1.y>2000.0f)
	{
		b1.y = 2000.0f;
		b1.w *= -roughness;
		b1.z *= roughness;
	}
	if(b1.y<20.0f)
	{
		b1.y = 20.0f;
		b1.w *= -roughness;
		b1.z *= roughness;
	}
	return b1;
	
}

void main()
{
	//FragValue = texture(weights,vec2( (gl_FragCoord.y / inSize),(gl_FragCoord.x / outSize) )).r;
	//FragValue = GetIn(int(gl_FragCoord.x));
	//FragValue = GetWeight(int(gl_FragCoord.x));
	//FragValue = GetBias(int(gl_FragCoord.x));
	FragValue = Run();
}