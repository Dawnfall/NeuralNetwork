#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

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

class Img8
{
public:
	Img8(const std::vector<Uint8>& grayBuffer, int height, int width) :
		GrayBuffer(grayBuffer),
		Height(height),
		Width(width)
	{}
	Img8(int height, int width,Uint8 defaultPixelGray=0) :
		Height(height),
		Width(width),
		GrayBuffer(width * height, defaultPixelGray)
	{}

	void ResetBufferWithARGB8888Color(Uint8 gray)
	{
		GrayBuffer.assign(GrayBuffer.size(), gray);
		m_buffer32.clear();
	}
	
	std::vector<Uint32> GetBufferARGB32Buffer()
	{
		if (m_buffer32.size() > 0)
			return m_buffer32;
		for (Uint8 pixel : GrayBuffer)
		{
			Uint32 pixel32 = (255 << 24) | (pixel << 16) | (pixel << 8) | pixel;
			m_buffer32.push_back(pixel32);
		}
		return m_buffer32;
	}

	void SetPixelGray(int x, int y, Uint32 gray)
	{
		if (x < 0 || x >= Width || y < 0 || y >= Height) {
			std::cerr << "Pixel coordinates are out of bounds!" << std::endl;
			return;
		}

		int i = Width * y + x;
		GrayBuffer[i] = gray;
		
		if(m_buffer32.size()>0)
			m_buffer32[i] = (255 << 24) | (gray << 16) | (gray << 8) | gray;

	}


	int Height;
	int Width;
	std::vector<Uint8> GrayBuffer;

private:
	std::vector<Uint32> m_buffer32;
};