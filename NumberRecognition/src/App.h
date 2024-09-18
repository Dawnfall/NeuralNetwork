#pragma once
#include "Trainer.h"
#include "SDL/include/SDL.h"
#include <iostream>

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
	void UpdatePixelOnCanvas(std::shared_ptr<Img> img, int mouseX, int mouseY);
private:
	Trainer m_nnTrainer;

	SDL_Window* m_window = nullptr;
	SDL_Texture* m_canvasTexture = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	SDL_Rect m_canvasRect;
	std::shared_ptr<Img> m_canvasImg = nullptr;

	SDL_Event e;

	std::string m_layersInput = "";
	std::string m_trainDataFolderInput = "";
	std::string m_trainCountInput = "";
	std::string m_learningRateInput = "";
	std::string m_minErrorInput = "";
	std::string m_resultText = "";
	std::string m_outputText = "";

	std::shared_ptr<Img> m_testImg = nullptr;

};
