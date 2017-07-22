#include <iostream>
#include <vector>
#include <cmath>

class PTrainer 
{
	public:  
	vector<float> inputs;
	int answer; 

	PTrainer(float x, float y, int a)
	{
		inputs.resize(3);
		inputs[0] = x;
		inputs[1] = y;
		inputs[2] = 1;
		answer = a;
	}
};

class Perceptron 
{
	public:
	vector<float> weights;
	float c;

	Perceptron(int n, float _c)
  	{
		weights.resize(n);
    		for (int i = 0; i < n; i++) 
		{
		      weights[i] = CluserGraphUtility::random(-1,1); 
		}
		c = _c;
  	}

  	void train(vector<float> inputs, int desired) 
	{
    		int guess = fforward(inputs);
    		float error = desired - guess;
    		for (int i = 0; i < weights.size(); i++) {
      			weights[i] += c * error * inputs[i];         
    		}
  	}

  	int fforward(vector<float> inputs) 
	{
    		float sum = 0;
		for (int i = 0; i < weights.size(); i++) {
      			sum += inputs[i]*weights[i];
    		}
    		return (sum > 0)?1:-1;
  	}
  
  	vector<float> getWeights() 
	{
		return weights; 
	}
};
