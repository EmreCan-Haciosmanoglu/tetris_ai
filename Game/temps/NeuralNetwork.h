#pragma once
#include "Matrix.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>

class NeuralNetwork
{
public:
	NeuralNetwork() = default;
	NeuralNetwork(int* nodes, int length, float lr);
	NeuralNetwork(int* nodes, int length, float lr, Matrix** w, Matrix** b);

	~NeuralNetwork();

	Matrix* FeedForward(Matrix* input) const;

	void FeedForward(Matrix** outputs) const;
	void Train(Matrix* inputs, Matrix* targets);
	NeuralNetwork* Mutate(float mr);

	static NeuralNetwork** Generate(NeuralNetwork** parents, int* fitnessScores, int totalScore, int parentCount, int childrenCount);
	static NeuralNetwork** Generate(std::map<int, NeuralNetwork*> brains, int totalScore, int parentCount, int childrenCount);

	static NeuralNetwork** Mutate(NeuralNetwork** source, int mutationRate, int sourceCount);

	void SetWeights(Matrix** w);
	void SetBiases(Matrix** b);

	inline int* GetNodes() const { return Nodes; }
	inline Matrix** GetWeights() const { return weights; }
	inline Matrix** GetBiases() const { return biases; }
	inline int GetLength() const { return Length; }
	inline float GetLR() const { return learning_rate; }
private:
	int* Nodes;
	int Length;

	Matrix** weights;
	Matrix** biases;

	float learning_rate;
};

