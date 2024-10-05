#version 330 core
out float FragValue;
// will process one layer of nn, 

uniform int inSize = 10;// X
uniform int outSize = 10;// X
uniform int InstanceCount = 1;// X
uniform int Func = 0; //Activation function of layer 

uniform sampler2D weights; // [inSize][outSize];
uniform sampler2D biases; // [outSize][1];
uniform sampler2D data_in; // [inSize][InstanceCount]
//uniform sampler2D data_out;// [outSize][InstanceCount] // this will be outputed wia FragValue




float sigmoid(float x)
{
	return 1 / (1 + pow(2.71828, -x));
}
float sigmoidApprox(float x)
{



	if (x > -2.05f && x < 2.05f)
		return x * 0.2f + 0.5f;

	if (x < -5)
		return 0.0f;

	if (x > 5)
		return 1.0f;

	if (x < -2.05f)
		return x * 0.03f + 0.15f;

	if (x > 2.05f)
		return x * 0.03f + 0.85f;
}
float ReLu(float x)
{
	return x >= 0.0f ? x : 0.0f;
}

float GetBias(int i)
{
	return texture(biases,vec2( (float(i+0.5f) / inSize),0.0f)).r;
}

float GetWeight(int i)
{
	return texture(weights,vec2( (float(i+0.5f) / inSize),(gl_FragCoord.x / outSize) )).r;
}

float GetIn(int i)
{
	return texture(data_in,vec2( (float(i+0.5f) / inSize), (gl_FragCoord.y / InstanceCount))).r;
}

float Run()
{
	float sum = GetBias(int(gl_FragCoord.x));

	for(int i=0;i<inSize;i++)
	{
		sum += GetWeight(i) * GetIn(i);
	}

	switch(Func)
	{
		case 0:
			return sigmoidApprox(sum);
		case 1:
			return ReLu(sum);
		case 2:
			return sigmoid(sum);
		case 3:
			return sum;
			
	}
}

void main()
{
	//FragValue = texture(weights,vec2( (gl_FragCoord.y / inSize),(gl_FragCoord.x / outSize) )).r;
	//FragValue = GetIn(int(gl_FragCoord.x));
	//FragValue = GetWeight(int(gl_FragCoord.x));
	//FragValue = GetBias(int(gl_FragCoord.x));
	FragValue = Run();
}