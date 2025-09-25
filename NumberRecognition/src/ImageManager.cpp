#include "ImageManager.h"

void ImageManager::CreateNewNN()
{
	std::vector<int> layerSizes = Utils::stringToVecOfInts(Image_layersInput);

	auto newNN = dawn::CreateNewNN(layerSizes);
	
	Image_NN_text = "Neural network: ";
	if (newNN)
	{
		m_nn = std::move(newNN);
		Image_NN_text += Utils::vecOfIntsToString(GetNN()->GetLayers());
		Utils::print("New NN created");
	}
	else
	{
		Image_NN_text += "No layer!";
		Utils::print("Unable to create NN");
	}
}

void ImageManager::Train()
{
	auto trainData = LoadTrainData(Image_trainDataFolderInput);
	dawn::NeuralNetwork::Train(*m_nn, trainData);
}

void ImageManager::CalculateValue()
{
	if (!GetCanvas().m_img || !m_nn)
	{
		return;
	}

	if (m_nn->GetLayerCount() == 0)
	{
		Utils::printErr("No neural network! Create one first!");
		return;
	}
	if (m_nn->GetLayerSize(0) != m_canvas.m_img->GrayBuffer.size())
	{
		Utils::printErr("Invalid input size!");
		return;
	}

	std::vector<float> input;
	for (Uint8 gray : m_canvas.m_img->GrayBuffer)
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

	if (maxIndex >= 0)
	{
		Image_resultText = "Written number = " + std::to_string(maxIndex);
	}
	else
	{
		Utils::print("Written number wasnt calculated!");
	}
}

dawn::TrainData ImageManager::LoadTrainData(const std::string& trainFolderPath)
{
	dawn::TrainData trainData;

	auto fileMap = Utils::GetFilePathsInDirectSubfolders(trainFolderPath);
	for (auto& folder : fileMap)
	{
		auto res = Utils::TryCastInt(folder.first);
		if (!res.Success)
		{
			Utils::printErr("Invalid subfolder name");
			return trainData;
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

	trainData.MaxInterCount = getTrainCount();
	trainData.LearnRate = getLearnRate();
	trainData.MinError = getMinError();

	Utils::print("Train data loading completed.");
	return trainData;
}

