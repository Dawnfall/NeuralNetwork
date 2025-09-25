#pragma once
#include "SDL/include/SDL.h"
#include "Utils.h"
#include "NeuralNetwork.h"
#include "XORManager.h"
#include "ImageManager.h"

#include <memory>
#include <string>

enum class AppState
{
	XOR,
	IMAGE
};

class App
{
public:
	App();
	~App();

	int InitData();
	int Run();

private:

	void DrawUI_ImageRecognition();
	void DrawUI_XOR();

	SDL_Window* m_window = nullptr;

	AppState m_state = AppState::XOR;
	ImageManager m_imageManager;
	XORManager m_xorManager;

};
