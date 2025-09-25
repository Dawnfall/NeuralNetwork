#include "XORManager.h"


void XORManager::CreateNewNN()
{
	std::vector<int> layerSizes = Utils::stringToVecOfInts(Xor_layersInput);

	auto newNN = dawn::CreateNewNN(layerSizes);

	Xor_NN_text = "Neural network: ";
	if (newNN)
	{
		m_nn = std::move(newNN);
		Xor_NN_text += Utils::vecOfIntsToString(GetNN()->GetLayers());
		Utils::print("New NN created");
	}
	else
	{
		Xor_NN_text += "No layer!";
		Utils::print("Unable to create NN");
	}
}

void XORManager::Train()
{
	if (m_nn)
	{
		dawn::TrainData trainData;

		trainData.Inputs = xor_inputs;
		trainData.Outputs = xor_outputs;

		trainData.LearnRate = getXORLearnRate(); 
		trainData.MaxInterCount = getXORMaxTrain(); 
		trainData.MinError = getXORMinError(); 

		dawn::NeuralNetwork::Train(*m_nn, trainData);
	}
}

void XORManager::Run()
{
	std::vector<float> inputs = { static_cast<float>(getXORInputA()),static_cast<float>(getXORInputB()) };

	std::vector<float> results = m_nn->FeedForward(inputs);
	Xor_test_resultText = "Result = " + std::to_string(results[0]);
}