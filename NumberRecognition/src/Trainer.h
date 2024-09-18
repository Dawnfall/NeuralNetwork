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

	std::vector<std::vector<std::vector<float>>> m_trainInputs;
	std::vector<std::vector<float>> m_trainOutputs;

public:
	Trainer() {}

	bool CreateNewNN(const std::vector<int>& layerSizes);

	int CalculateValue(const std::vector<float>& imgBuffer)
	{
		if (!m_nn)
		{
			Utils::printErr("No neural network! Create one first!");
			return -1;
		}
		if (m_nn->GetLayerSize(0) != imgBuffer.size())
		{
			Utils::printErr("Invalid input size!");
			return -1;
		}

		//std::vector<float> input;
		//for (Uint32 argb : imgBuffer)
		//{
		//	Uint8 r = (argb >> 16) & 0xFF;
		//	Uint8 g = (argb >> 8) & 0xFF;
		//	Uint8 b = argb & 0xFF;

		//	input.push_back((float)r / 255.0f);
		//	input.push_back((float)g / 255.0f);
		//	input.push_back((float)b / 255.0f);
		//}

		//std::vector<float> results = m_nn->FeedForward(input);

		int max = -1;
		//for (int i = 0; i < results.size(); i++)
		//	if (results[i] > max)
		//		max = results[i];

		return max;
	}



	void LoadTrainData(const std::string& trainFolderPath)
	{
		auto fileMap = Utils::GetFilePathsInDirectSubfolders(trainFolderPath);

		for (auto& folder : fileMap)
		{
			auto res = Utils::TryCastInt(folder.first);
			if (!res.Success)
			{
				Utils::printErr("Invalid subfolder name");
				return;
			}

			for (auto& file : folder.second)
			{
				Utils::LoadImage8(file);
			}

		}
		//m_trainData = Utils::LoadTrainingData(trainFolderPath);
	}

	void TrainNN(float learnRate, int trainCount)
	{
		if (m_nn)
		{
			Utils::printErr("No neural network defined");
		}
		if (m_trainInputs.size() || m_trainOutputs.size() == 0)
		{
			Utils::printErr("No training data");
		}

		for (int count = 0; count < trainCount; ++count)
			for (int i = 0; i < 10; i++)
			{
				std::vector<float>& output = m_trainOutputs[i];
				for (std::vector<float>& input : m_trainInputs[i])
				{
					m_nn->BackPropagate(input, output, learnRate);
				}
			}

		Utils::print("Training Complete!");

	}


};