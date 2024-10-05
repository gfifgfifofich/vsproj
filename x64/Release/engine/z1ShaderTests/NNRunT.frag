#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


// [Arch[0]..... , Weights[0]..... Biases[0]......Biases[biassize]]  ;
// unifs: NNTextureSize, outsize, thrirdparam, NNTextureSize archsize Weightsize Biassize  ArchStart WeightsStart BiasesStart


uniform int archsize;
uniform int Weightsize;
uniform int Biassize;


uniform sampler1D weights;
uniform sampler1D biases;
uniform sampler1D arch;
uniform sampler1D NodesStep;
uniform sampler1D WeightsStep;




//uniform vec2 TextureSize;

//uniform sampler2D Texture;


float sigmoid(float x)
{
	return 1.0f / (1.0f + pow(2.71828f, -x));
}
float NNTStep ;
int sizein;
int acrchi;
int prevacrchi;

float inputData[10];

float PrevNodes[100];
float NewNodes[100];

/*float GetFromTeture(float pos)
{
	float t = texture(NNTexture,pos).r;
	if(t>800.0f)
		t-=1000.0f;
	return t;
}*/

float Run(float third)
{

	PrevNodes[0] = TexCoords.x;
	PrevNodes[1] = TexCoords.y;
	PrevNodes[2] = third;
	for (int i = 1; i < archsize; i++)
	{
		int size = int(texelFetch(arch,i,0).r);
		int weightsPerNode =  int(texelFetch(arch,i-1,0).r);
		int node = 0;

		for (int n = int(texelFetch(NodesStep,i,0).r); n <int(texelFetch(NodesStep,i,0).r) + size; n++)
		{

			float sum = 0.0f;
			int start = int(texelFetch(WeightsStep,i - 1,0).r) + node * weightsPerNode;
			int PrevLayerNode = 0;
			for (int w = start; w < start + weightsPerNode; w++)
			{
				sum += texelFetch(weights,w,0).r * PrevNodes[PrevLayerNode];
				PrevLayerNode++;
			}

			NewNodes[node] =sigmoid(sum +  texelFetch(biases,n,0).r);
			node++;
		}
		for (int i = 0; i < 100; i++)
			PrevNodes[i] = NewNodes[i];
	}
	return(NewNodes[0]);
}

void main()
{


	
	//un();
	//FragColor = vec4(Run(0.0f),Run(0.33f),Run(0.66f),1.0f); 
	//FragColor = vec4(2.0f); 
	//FragColor = vec4(texelFetch(biases,0,0)); 
	//FragColor = vec4(vec3(GetFromTeture(3*NNTStep)*1.0f),1.0f); 
	//float texel = GetFromTeture(UV.x)*0.1f;
	

	//FragColor = vec4(clamp(abs(texel),0.0f,1.0f)); //vec4(UV.x,UV.y,1.0f,1.0f);//
}





	
	