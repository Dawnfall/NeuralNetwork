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
	m_nn = std::make_unique<dawn::NeuralNetwork>(dawn::NeuralNetwork(layerSizes, randGen));
	return true;
}