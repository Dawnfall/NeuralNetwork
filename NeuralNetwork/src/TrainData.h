#pragma once
#include <vector>

namespace dawn
{
	struct TrainData
	{
		std::vector<std::vector<float>> Outputs;
		std::vector<std::vector<float>> Inputs;

		float LearnRate;
		int MaxInterCount;
		float MinError;
	};
}