#pragma once
class Perceptron
{
public:
	Perceptron();
	Perceptron(float connections);
	Perceptron(float connections, float learningRate);

	~Perceptron();

	template<int T>
	float Guess(float inputs[T]) const;

	template<int T>
	float Train(float inputs[T], float label) ;

private:
	float *weight;
	float learningRate;
};

