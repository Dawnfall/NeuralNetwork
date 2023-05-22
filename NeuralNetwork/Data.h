#pragma once

#include <vector>

namespace dawn
{
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