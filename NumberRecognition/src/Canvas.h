#pragma once
#include "SDL.h"
#include <memory>
#include <algorithm>

#include "Img.h"
#include "Utils.h"

class Canvas
{
public:
	Canvas(int posX, int posY, int width, int height);
	~Canvas();

	void SetImage(int width, int height, Uint8 defaultGray, SDL_Renderer* renderer);
	void SetImage(const std::string& imagePath, SDL_Renderer* renderer);

	void UpdatePixelOnCanvas(int mouseX, int mouseY);

	void Render(SDL_Renderer* renderer);

	std::shared_ptr<Img8> m_img = nullptr;
	SDL_Texture* m_texture = nullptr;
	SDL_Rect m_rect;
};