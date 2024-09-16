#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "Utils.h"
#include <iostream>
//#include <SDL_image.h>


#pragma region Consts
//***********
// CONSTS
const int SCREEN_CLEAR_R = 173;
const int SCREEN_CLEAR_G = 216;
const int SCREEN_CLEAR_B = 230;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int RECT_X = 300;
const int RECT_Y = 200;
const int RECT_WIDTH = 200;
const int RECT_HEIGHT = 200;

const int TEX_WIDTH_PX = 32;
const int TEX_HEIGHT_PX = 32;
#pragma endregion Consts

//*******************

SDL_Event e;

SDL_Window* m_window = nullptr;
SDL_Renderer* m_renderer = nullptr;
SDL_Texture* m_texture = nullptr;
SDL_Surface* m_surface = nullptr;
SDL_Rect square;

bool isMousePressed = false;

bool InitSDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}
SDL_Window* OpenWindow()
{
	SDL_Window* window = nullptr;
	// Create a window
	window = SDL_CreateWindow("SDL Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}

	return window;
}

SDL_Surface* CreateSurface()
{
	// Create a new surface
	SDL_Surface* surface = SDL_CreateRGBSurface(0, RECT_WIDTH, RECT_HEIGHT, 32, 0, 255, 0, 255);
	Uint32 color = SDL_MapRGBA(surface->format, 255, 0, 0, 255);

	// Fill the entire surface with the specified color
	if (SDL_FillRect(surface, nullptr, color) != 0) {
		std::cerr << "Unable to fill surface: " << SDL_GetError() << std::endl;
	}

	return surface;
}

void DetectMousePress(int mouseX, int mouseY)
{
	// Check if the mouse click is inside the square
	if (mouseX >= square.x && mouseX <= square.x + square.w &&
		mouseY >= square.y && mouseY <= square.y + square.h) {

		// Map mouse position to the texture coordinates
		int texX = mouseX - square.x;
		int texY = mouseY - square.y;

		//	Utils::setPixelColor(m_texture, texX, texY, 255, 255, 255, 255);
		Utils::setPixelColor(m_surface, texX, texY, 0, 0, 0, 255);
		//std::cout << "Mouse clicked at texture pixel: ("
		//	<< texX << ", " << texY << ")" << std::endl;
	}
}

void Terminate()
{
	SDL_FreeSurface(m_surface);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}




void ClearWindow(SDL_Window* window, SDL_Renderer* renderer, int r, int g, int b)
{
	// Set the draw color (background color) for clearing (RGBA format)
	SDL_SetRenderDrawColor(renderer, r, g, b, 255); // Black color

	// Clear the screen
	SDL_RenderClear(renderer);
}

int main()
{
	if (!InitSDL())
		return -1;

	square.x = RECT_X;
	square.y = RECT_Y;
	square.w = RECT_WIDTH;
	square.h = RECT_HEIGHT;

	m_window = OpenWindow();
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	m_surface = CreateSurface();

	bool doRun = true;
	while (doRun)
	{
		// Handle events on the queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quit
			if (e.type == SDL_QUIT) {
				doRun = false;
			}

		}

		int mouseX, mouseY;
		if (SDL_GetMouseState(&mouseX, &mouseY) && SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			DetectMousePress(mouseX, mouseY);
		}



		ClearWindow(m_window, m_renderer, SCREEN_CLEAR_R, SCREEN_CLEAR_G, SCREEN_CLEAR_B);


		m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
		SDL_RenderCopy(m_renderer, m_texture, nullptr, &square);

		//// Update the screen
		SDL_RenderPresent(m_renderer);
	}

	Terminate();
}

//// Function to load a texture from a file
//SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
//	// Load the image into a surface
//	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
//	if (loadedSurface == nullptr) {
//		std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
//		return nullptr;
//	}
//
//	// Create texture from the surface
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
//	if (texture == nullptr) {
//		std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
//	}
//
//	// Free the surface, as it's no longer needed
//	SDL_FreeSurface(loadedSurface);
//
//	return texture;
//}
