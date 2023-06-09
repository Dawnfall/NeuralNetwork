#pragma once
#include <vector>

namespace dawn
{
	struct NeuralNetworkData
	{
		int id;
		std::vector<std::vector<std::vector<float>>> weights;
		std::vector<std::vector<float>> biases;
	};

	struct TrainData
	{
		std::vector<float> input;
		std::vector<float> target;
	};

	struct TrainParams
	{
		float learnRate;
		float minEpoch;
		float maxEpoch;
		float minError;
	};


}