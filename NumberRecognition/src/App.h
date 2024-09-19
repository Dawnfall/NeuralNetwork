#pragma once
#include "Trainer.h"
#include "SDL/include/SDL.h"
#include "Canvas.h"
#include "Utils.h"

#include <memory>

//***********
// CONSTS
#pragma region Consts
const std::string APP_NAME = "DIGIT RECOGNITION";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int SCREEN_CLEAR_R = 173;
const int SCREEN_CLEAR_G = 216;
const int SCREEN_CLEAR_B = 230;

const int CANVAS_RECT_X = 10;
const int CANVAS_RECT_Y = 10;
const int CANVAS_RECT_WIDTH = 256;
const int CANVAS_RECT_HEIGHT = 256;

const int IMG_RESOLUTION_WIDTH = 16;
const int IMG_RESOLUTION_HEIGHT = 16;

const std::string TEST_IMG_PATH_RESIZED = "C:/Users/Dawn/Desktop/digits updated/ReSized/6/img007-00011.png";
const std::string TEST_IMG_PATH_ORIGINAL = "C:/Users/Dawn/Desktop/digits updated/Original/0/img001-00001.png";
const std::string TEST_IMG_PATH_UNIMPORTANT = "C:/Users/Dawn/Desktop/fff/20170719_021516.jpg";
#pragma endregion Consts

//*******************

class App
{
public:
	App();
	~App();

	int InitData();
	int Run();

	void DrawUI();

	void OnClearCanvasButtonClick();
	void OnCreateNewNNButtonClick();
	void OnRunButtonClick();
	void OnLoadDataButtonClick();
	void OnTrainButtonClick();
private:
	Trainer m_nnTrainer;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	Canvas m_canvas;

	std::string m_layersInput = "256 128 64 10"; //256 64 32 10
	std::string m_trainDataFolderInput = "C:/Users/Dawn/Desktop/digits_updated/ReSized";
	std::string m_trainCountInput = "10";
	std::string m_learningRateInput = "0.3";
	std::string m_minErrorInput = "0.1";
	std::string m_resultText = "";
	std::string m_outputText = "";

};
