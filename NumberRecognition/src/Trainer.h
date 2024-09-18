#pragma once
#include "NeuralManager.h"
#include "Utils.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

class Trainer
{
	int trainCount = 0;
	float learnRate = 0.1f;
	float minError = 0.01f;

	std::unique_ptr<dawn::NeuralNetwork> m_nn = nullptr;
	std::unordered_map<int, std::vector<std::vector<Uint8>>> m_trainData;

public:
	Trainer() {}



	bool CreateNewNN(const std::vector<int>& layerSizes);

	int CalculateValue(const std::vector<Uint32>& imgBuffer)
	{
		if (!m_nn)
		{
			std::cout << "No neural network! Create one first!";
			return -1;
		}
		if (m_nn->GetLayerSize(0) != imgBuffer.size() * 3)
		{
			std::cout << "Invalid input size!" << std::endl;
			return -1;
		}

		std::vector<float> input;
		for (Uint32 argb : imgBuffer)
		{
			Uint8 r = (argb >> 16) & 0xFF;
			Uint8 g = (argb >> 8) & 0xFF;
			Uint8 b = argb & 0xFF;

			input.push_back((float)r / 255.0f);
			input.push_back((float)g / 255.0f);
			input.push_back((float)b / 255.0f);
		}

		std::vector<float> results = m_nn->FeedForward(input);

		int max = -1;
		for (int i = 0; i < results.size(); i++)
			if (results[i] > max)
				max = results[i];

		return max;
	}



	void LoadTrainData(const std::string& trainFolderPath)
	{
		//m_trainData = Utils::LoadTrainingData(trainFolderPath);
	}

	void TrainNN()
	{

	}


};