#pragma once

#include "SDL/include/SDL.h"

namespace Utils
{
	void setPixelColor(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
}