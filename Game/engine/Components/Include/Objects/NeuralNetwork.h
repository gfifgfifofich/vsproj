#pragma once

class NeuralNetwork
{
public:

	bool Created = false;
	float* outputs;
	float* inputs;



	float* Nodes;//s

	float* biases;//s
	float* biasGradients;//s

	float* weights;//s*(s-1)
	float* weightGradients;//s*(s-1)
	float* weightOutputs;//s*(s-1)


	int* Arch;
	int* NodesStep;
	int* WeightsStep;



	int NodesAmount;
	int WeightsAmount;

	int sizein;
	int sizeout;
	int LayersAmount;

	float lastCost = -1.0f;

	float rate = 1.0f;
	float c1 = 1.0f;
	float h = 1.0f;
	int gyms = 96;// used only in a method of randomizing everything and choosing best. better to be allignet to trheadcount of cpu

	void Create(int* Architecture, int archsize);
	void Randomize();
	void Run(float* inputData);
	void Run(float ActFunc(float), float* inputData);

	// inputs an array through a NN and calculates diviation from output 
	float Cost(float* input, float* output, int amount);

	// Uses Cost() to minimize the diviation between outputs array and NN.outputs . Method - finite difirences. For more variety see CustomLearn()
	void learn(float rate, float* input, float* output, int amount);

	//Setups nn for future learning. After the learning iteration compleated, call ApplyLearning
	void SetupLearing();
	void ApplyLearing(float cost);
	
	//Copies itself into other nn;
	void CopyInto(NeuralNetwork* target);
	//Copies itself into other nn;
	void CopyIntoWithGradient(NeuralNetwork* target);

	void ApplyGrad();
	void DeApplyGrad();

	// unimplemented, use to make a custom NN learning behaviour
	std::vector<NeuralNetwork*> DataStorage;
	
	// Draws NN structure, all weights(lines) and biases(circles)
	void Draw(glm::vec2 position = glm::vec2(0.0f), float weigthScale = 1.0f, float NeuronScale = 1.0f, glm::vec2 scale = glm::vec2(1.0f));

	//Draws NN "During its work", weight activations(lines), Nodes values(circles)
	void Draw(float* inputdata, glm::vec2 position = glm::vec2(0.0f), float weigthScale = 1.0f, float NeuronScale = 1.0f, glm::vec2 scale = glm::vec2(1.0f));
	
	void SaveTo(std::string filename);
	void LoadFrom(std::string filename);

	void Delete();
	~NeuralNetwork()
	{
		Delete();
	}
};