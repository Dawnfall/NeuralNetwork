#pragma once

#include "SDL/include/SDL.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "Img.h"

namespace Utils
{
	void InitIMGUI(SDL_Window* window, SDL_Renderer* renderer);
	bool InitSDL();

	void TerminateSDL();
	void TerminateImgui();

	std::shared_ptr<Img> LoadImage(const char* imagePath);
	std::unordered_map<int, std::vector<std::shared_ptr<Img>>> LoadTrainingData(const std::string& trainingFolder); //move

	void ClearWindow(SDL_Window* window, SDL_Renderer* renderer, int r, int g, int b);
	SDL_Window* OpenWindow(const std::string& name, int width, int height);

	std::vector<int> stringToVecOfInts(const std::string& str);

	char* copystring(const char* str);

	//std::vector<char> LoadGrayBuffer(const std::string path, int desiredWidth, int desiredHeight);
	//unsigned char* ResizeBuffer(unsigned char* buffer, int width, int height, int channels, int newWidth, int newHeight);
}