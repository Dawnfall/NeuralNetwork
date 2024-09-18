#include "Utils.h"
#include <iostream>
#include <sstream>  
#include <SDL_image/include/SDL_image.h>

namespace Utils
{
	void InitIMGUI(SDL_Window* window, SDL_Renderer* renderer)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
	}

	bool InitSDL()
	{
		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}
		return true;
	}

	void TerminateSDL()
	{
		SDL_Quit();
	}

	void TerminateImgui()
	{
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}


	void ClearWindow(SDL_Window* window, SDL_Renderer* renderer, int r, int g, int b)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);
	}

	SDL_Window* OpenWindow(const std::string& name, int width, int height)
	{
		return SDL_CreateWindow(
			name.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_SHOWN);
	}

	std::unordered_map<int, std::vector<std::shared_ptr<Img>>> LoadTrainingData(const std::string& trainingFolder)
	{
		//assumes folder has subfolders with name that matches the expected result, inside are images representing this result
		// trainig data is of form {r1,g1,b1,r2,g2,b2...}

		std::filesystem::path path(trainingFolder);
		std::unordered_map<int, std::vector<std::shared_ptr<Img>>> trainingData;

		if (!std::filesystem::is_directory(path))
		{
			std::cerr << "The provided path is not a directory." << std::endl;
			return trainingData;
		}

		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (!entry.is_directory())
			{
				std::cerr << "Only folders allowed in path folder!" << std::endl;
				trainingData.clear();
				return trainingData;
			}

			std::string folderName = entry.path().filename().string();
			int resultValue;
			auto success = std::from_chars(folderName.data(), folderName.data() + folderName.size(), resultValue);
			if (success.ec != std::errc())
			{
				std::cerr << "Invalid training subfolder name";
				trainingData.clear();
				return trainingData;
			}

			std::vector<std::shared_ptr<Img>> images;
			for (const auto& fileEntry : std::filesystem::directory_iterator(entry.path())) {
				if (fileEntry.is_regular_file()) {
					std::string imgPath = fileEntry.path().filename().string();
					auto image = LoadImage(imgPath.c_str());
					if (!image)
					{
						std::cerr << "Invalid file as training data!" << std::endl;
						trainingData.clear();
						return trainingData;
					}
					images.push_back(image);
				}
			}
			trainingData[resultValue] = images;

		}
		return trainingData;
	}

	std::shared_ptr<Img> LoadImage(const char* imagePath)
	{
		// Load the BMP image
		SDL_Surface* imageSurface = IMG_Load(imagePath);
		if (!imageSurface) {
			std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		// Get image dimensions and pixel data
		int width = imageSurface->w;
		int height = imageSurface->h;
		int pitch = imageSurface->pitch;
		auto* format = imageSurface->format;
		Uint32* pixels = (Uint32*)imageSurface->pixels;


		std::cout << "Image loaded successfully!" << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "Format: " << format->format << std::endl;
		std::cout << "Pitch: " << pitch << std::endl;

		std::vector<Uint32> imgBuffer;





		//const Uint8 Bpp = imageSurface->format->BytesPerPixel;
		//for (int x = 0; x < width; x++)
		//	for (int y = 0; y < height; y++)
		//	{
		//		Uint8* pPixel = (Uint8*)imageSurface->pixels + y * imageSurface->pitch + x * Bpp;
		//		Uint32 PixelData = *(Uint32*)pPixel;
		//		Uint8 r, g, b;
		//		SDL_GetRGB(PixelData, imageSurface->format, &r, &g, &b);

		//		Uint32 colorARGB = (0 << 24) + (r << 16) + (g << 8) + b;
		//		imgBuffer.push_back(colorARGB);
		//	}






		int bbp = pitch / width;
		if (bbp == 1)
		{
			Uint8* bytePixels = reinterpret_cast<Uint8*>(pixels);
			for (int i = 0; i < width * height * sizeof(Uint8); ++i) {
				Uint8 pixelValue = bytePixels[i];
				if (pixelValue > 170)
					pixelValue = 255;
				else
					pixelValue = 0;
				Uint32 colorARGB = (255 << 24) | (pixelValue << 16) | (pixelValue << 8) | pixelValue;
				imgBuffer.push_back(colorARGB);
			}
		}
		//else if (bbp == 2)
		//{

		//}
		//else if (bbp == 3)
		//{

		//}
		//else if (bbp == 4)
		//{

		//}


		//std::cout << "Pitch: " << imageSurface->format->BytesPerPixel << std::endl;
		//std::vector<Uint32> imgBuffer(pixels, pixels + pitch * height);

		SDL_FreeSurface(imageSurface);
		return std::make_shared<Img>(imgBuffer, width, height, format);
	}






	//***********
	// Unused

	char* copystring(const char* str)
	{
		// Allocate memory for the new string (+1 for null terminator)
		char* copy = (char*)std::malloc(std::strlen(str) + 1);

		// Check if memory allocation succeeded
		if (copy != nullptr) {
			// Copy the string into the newly allocated memory
			std::strcpy(copy, str);
		}
		return copy;
	}

	std::vector<int> stringToVecOfInts(const std::string& str)
	{
		std::vector<int> integers;
		std::istringstream iss(str);
		std::string token;

		while (iss >> token)
		{
			try
			{
				integers.push_back(std::stoi(token));
			}
			catch (...)
			{
				return std::vector<int>();
			}
		}
		return integers;
	}

	//std::vector<char> LoadGrayBuffer(const std::string path, int desiredWidth, int desiredHeight)
	//{
	//	int width, height, channels;
	//	unsigned char* buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);
	//	unsigned char* resizedBuffer;
	//	std::vector<char> grayBuffer;

	//	if (!buffer)
	//	{
	//		std::cout << "Filed to load image!" << std::endl;
	//		stbi_image_free(buffer);
	//		return grayBuffer;
	//	}

	//	if (width != desiredWidth && height != desiredHeight)
	//	{
	//		resizedBuffer = ResizeBuffer(buffer, width, height, channels, desiredWidth, desiredHeight);
	//		stbi_image_free(buffer);
	//		buffer = resizedBuffer;
	//	}

	//	std::vector<char> grayBuffer;
	//	for (int i = 0; i < width * height; i++)
	//	{
	//		char gray = 0;
	//		int pxStart = i * channels;
	//		switch (channels)
	//		{
	//		case 1:
	//			gray = buffer[pxStart];
	//			break;
	//		case 2:
	//			gray = buffer[pxStart];
	//			break;
	//		case 3:
	//			char r = buffer[pxStart];
	//			char g = buffer[pxStart + 1];
	//			char b = buffer[pxStart + 2];

	//			gray = r;
	//			break;
	//		case 4:
	//			char r = buffer[pxStart];
	//			char g = buffer[pxStart + 1];
	//			char b = buffer[pxStart + 2];

	//			gray = r;
	//			break;
	//		default:
	//			std::cerr << "unsupported channel size" << std::endl;
	//			gray = 0;
	//			break;
	//		}
	//		grayBuffer.push_back(gray);
	//	}

	//	return grayBuffer;
	//}

	//unsigned char* ResizeBuffer(unsigned char* buffer, int width, int height, int channels, int newWidth, int newHeight)
	//{
	//	unsigned char* resizedBuffer = new unsigned char[newWidth * newHeight * channels];

	//	int stride = 0;

	//	stbir_pixel_layout pixelLayout;
	//	switch (channels)
	//	{
	//	case 1:
	//		break; //g
	//	case 2:
	//		break; // ga
	//	case 3:
	//		break; // rgb
	//	case 4:
	//		break; //rgba
	//	default:
	//		std::cerr << "unsupported channel format" << std::endl;
	//		return nullptr;
	//	}

	//	stbir_resize(buffer, width, height, stride, resizedBuffer, newWidth, newHeight, stride, STBIR_RGBA, STBIR_TYPE_UINT8, STBIR_EDGE_CLAMP, STBIR_FILTER_DEFAULT);

	//	resizedBuffer;
	//}

}