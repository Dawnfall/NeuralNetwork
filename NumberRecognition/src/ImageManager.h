#pragma once

#include <string>
#include "Canvas.h"
#include "TrainData.h"
#include "Constants.h"
#include "NeuralNetwork.h"

class ImageManager
{
public:
	ImageManager():
		m_canvas(CANVAS_RECT_X, CANVAS_RECT_Y, CANVAS_RECT_WIDTH, CANVAS_RECT_HEIGHT)
	{ }

	float getLearnRate()
	{
		try
		{
			return std::stof(Image_learningRateInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid learning rate input");
			return 0.0f;
		}
	}
	int getTrainCount()
	{
		try
		{
			return  std::stoi(Image_trainCountInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid train count");
			return 0;
		}
	}
	float getMinError()
	{
		try
		{
			return  std::stof(Image_minErrorInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid min Error");
			return 0.0f;
		}
	}

	SDL_Renderer* GetRenderer() { return m_renderer; }
	void SetRenderer(SDL_Renderer* renderer) { m_renderer = renderer; }
	dawn::NeuralNetwork* GetNN() { return m_nn ? m_nn.get() : nullptr; }

	Canvas& GetCanvas() { return m_canvas; }
	void CreateNewNN();

	void Train();
	void CalculateValue();

	std::string Image_NN_text = "Neural network: ";

	std::string Image_layersInput = "256 128 64 10"; //256 64 32 10
	std::string Image_trainDataFolderInput = "C:/Users/Dawn/Desktop/digits_updated/ReSized";
	std::string Image_trainCountInput = "1000";
	std::string Image_learningRateInput = "0.1";
	std::string Image_minErrorInput = "0.01";
	std::string Image_outputText = "";
	std::string Image_resultText = "Written number = ? ";
private:

	dawn::TrainData LoadTrainData(const std::string& trainFolderPath);

	std::unique_ptr<dawn::NeuralNetwork> m_nn = nullptr;
	Canvas m_canvas;
	SDL_Renderer* m_renderer = nullptr;
};