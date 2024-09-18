#pragma once
#include <SDL.h>
#include <iostream>

class Img
{
public:
	Img(std::vector<Uint32> buffer, int width, int height, SDL_PixelFormat* format) :
		Width(width),
		Height(height),
		Buffer(buffer),
		Format(format)
	{}

	int Width;
	int Height;
	std::vector<Uint32> Buffer;
	SDL_PixelFormat* Format;

	void ResetBufferWithARGB8888Color(Uint8 a, Uint8 r, Uint8 g, Uint8 b)
	{
		Uint32 color = (a << 24) | (r << 16) | (g << 8) | b;
		Buffer.assign(Buffer.size(), color);
	}

	void SetPixelGray(int x, int y, Uint32 gray)
	{
		if (x < 0 || x >= Width || y < 0 || y >= Height) {
			std::cerr << "Pixel coordinates are out of bounds!" << std::endl;
			return;
		}

		int i = Width * y + x;
		Uint32 newColor = (255 << 24) | (gray << 16) | (gray << 8) | gray;
		Buffer[i] = newColor;
	}

	//rgbrgbrgb...
	std::vector<Uint8> GetChannels()
	{
		std::vector<Uint8> rgbValues(Width * Height * 3);
		// Extract RGB values for each pixel
		for (int y = 0; y < Height; ++y) {
			for (int x = 0; x < Width; ++x) {
				Uint32 pixel = Buffer[y * Width + x];
				Uint8 r, g, b;
				SDL_GetRGB(pixel, Format, &r, &g, &b);

				// Store RGB values in the vector
				rgbValues[(y * Width + x) * 3] = r;
				rgbValues[(y * Width + x) * 3 + 1] = g;
				rgbValues[(y * Width + x) * 3 + 2] = b;
			}
		}
		return rgbValues;
	}
};