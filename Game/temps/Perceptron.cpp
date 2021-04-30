#include "Perceptron.h"
#include <cstdlib>


Perceptron::Perceptron()
	: weight(nullptr)
	, learningRate(0)
{
}

Perceptron::Perceptron(float connections)
	: weight(new float[connections])
	, learningRate(0.1f)
{
	for (int i = 0; i < connections; i++)
		weight[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Perceptron::Perceptron(float connections, float learningRate)
	: weight(new float[connections])
	, learningRate(learningRate)
{
	for (int i = 0; i < connections; i++)
		weight[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Perceptron::~Perceptron()
{
	delete weight;
}

template<int T>
inline float Perceptron::Guess(float inputs[T]) const
{
	float result = 0.0f;

	for (int i = 0; i < T; i++)
		result += inputs[i] * weight[i];
	return ActivationFunc(result);
}

template<int T>
inline float Perceptron::Train(float inputs[T], float label)
{
	float guess = Guess(inputs);
	float error = label - guess;
	for (int i = 0; i < T; i++)
		weight += error * inputs[i] * learningRate;
}

static float ActivationFunc(float input)
{
	//do something
	return input;
}
