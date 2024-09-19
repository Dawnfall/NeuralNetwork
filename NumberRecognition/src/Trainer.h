#pragma once
#include "NeuralManager.h"
#include "Utils.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "SDL.h"

class Trainer
{
	dawn::TrainData trainData;
	std::unique_ptr<dawn::NeuralNetwork> m_nn = nullptr;

public:
	Trainer() 
	{
		trainData.MaxInterCount = 1000;
		trainData.LearnRate = 0.1f;
		trainData.MinError = 0.01f;
	}

	bool CreateNewNN(const std::vector<int>& layerSizes);
	int CalculateValue(const std::vector<Uint8>& imgBuffer);
	void LoadTrainData(const std::string& trainFolderPath);
	void TrainNN();

};