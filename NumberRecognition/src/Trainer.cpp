#include "Trainer.h"

bool Trainer::CreateNewNN(const std::vector<int>& layerSizes)
{
	if (layerSizes.size() < 2)
	{
		std::cout << "not enough NN layers" << std::endl;
		return false;
	}

	for (auto& layer : layerSizes)
		if (layer < 1)
		{
			std::cout << "layer size must at least 1" << std::endl;
			return false;
		}

	std::mt19937 randGen;
	m_nn = std::make_unique<dawn::NeuralNetwork>(layerSizes, randGen, MathUtils::ReLU, MathUtils::ReLUDerivative);
	return true;
}

int Trainer::CalculateValue(const std::vector<Uint8>& imgBuffer)
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

	std::vector<float> input;
	for (Uint8 gray : imgBuffer)
		input.push_back(static_cast<float>(gray) / 255.0f);

	std::vector<float> results = m_nn->FeedForward(input);

	float maxValue = -1.0f;
	int maxIndex = -1;
	for (int i = 0; i < results.size(); i++)
		if (results[i] > maxValue)
		{
			maxValue = results[i];
			maxIndex = i;
		}

	return maxIndex;
}

void Trainer::LoadTrainData(const std::string& trainFolderPath)
{
	auto fileMap = Utils::GetFilePathsInDirectSubfolders(trainFolderPath);

	trainData.Inputs.clear();
	trainData.Outputs.clear();

	for (auto& folder : fileMap)
	{
		auto res = Utils::TryCastInt(folder.first);
		if (!res.Success)
		{
			Utils::printErr("Invalid subfolder name");
			return;
		}

		std::vector<float> output(10, 0);
		output[res.Result] = 1.0f;

		for (auto& file : folder.second)
		{
			auto img = Utils::LoadImage8(file);

			std::vector<float> input;
			for (Uint8 gray : img->GrayBuffer)
				input.push_back(static_cast<float>(gray) / 255.0f);

			trainData.Inputs.push_back(input);
			trainData.Outputs.push_back(output);
		}

	}
	//m_trainData = Utils::LoadTrainingData(trainFolderPath);
}

void Trainer::TrainNN()
{
	if (!m_nn)
	{
		Utils::printErr("No neural network defined");
	}

	dawn::NeuralNetwork::Train(*m_nn, trainData);
	Utils::print("Training Complete!");
}