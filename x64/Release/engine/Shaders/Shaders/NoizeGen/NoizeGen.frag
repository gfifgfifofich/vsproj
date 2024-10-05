#version 330 core
out vec4 Color;

in vec2 TexCoords;

uniform sampler2D PrevTexture;

uniform float sizex = 1.0f;
uniform float sizey = 1.0f;

uniform float weight = 1.0f;

uniform int shape = 0;

uniform vec2 offset = vec2(0.0f);



float rand(vec2 co)
{
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 randomGradient(int ix, int iy) 
{
	float random = rand(vec2(ix,iy));
	return vec2(cos(random+offset.x), sin(random+offset.y));

}

float dotGridGradient(int ix, int iy, float x, float y) 
{
    vec2 gradient = randomGradient(ix, iy);

    float dx = x - float(ix);
    float dy = y - float(iy);

    return (dx*gradient.x + dy*gradient.y);
}

float perlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = int(floor(x));
    int x1 = x0 + 1;
    int y0 = int(floor(y));
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - float(x0);
    float sy = y - float(y0);

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = mix(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = mix(n0, n1, sx);

    value = mix(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

void main()
{
	

	float col = texture(PrevTexture,TexCoords).a;

	float r = perlin(TexCoords.x*sizex,TexCoords.y*sizey);
	r = (r+1)*0.5f-0.25f;
	col += r*weight;

	float mx = 1.0f;
	float my = 1.0f;
	
	if(shape == 1)
	{
		if(TexCoords.x<0.2f)
			mx = smoothstep(0.0f,1.0f,TexCoords.x*5.0f);
		else if(TexCoords.x>0.8f)
			mx = smoothstep(0.0f,1.0f,(1.0f - TexCoords.x)*5.0f);


		if(TexCoords.y<0.2f)
			my = smoothstep(0.0f,1.0f,TexCoords.y*5.0f);
		else if(TexCoords.y>0.8f)
			my = smoothstep(0.0f,1.0f,(1.0f - TexCoords.y)*5.0f);
	}
	if(shape==2)
	{
			
		vec2 uv = TexCoords *2.0f - vec2(1.0f);
		float dist = uv.x*uv.x+uv.y*uv.y;
		mx  = smoothstep(1.0f,0.0f,dist);
		
	
	}
	Color = vec4(1.0f,1.0f,1.0f,col*mx*my);
}