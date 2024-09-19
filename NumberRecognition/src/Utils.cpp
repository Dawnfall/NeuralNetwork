#include "Utils.h"
#include <iostream>
#include <sstream>  
#include <SDL_image/include/SDL_image.h>
#include <algorithm>

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

	std::unordered_map<std::string, std::vector<std::string>> GetFilePathsInDirectSubfolders(const std::string& folderPath)
	{
		std::unordered_map<std::string, std::vector<std::string>> folderFileMap;

		// Iterate through the directory to get direct subfolders
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (entry.is_directory()) {
				std::string subfolder = entry.path().filename().string();  // Get the subfolder name

				std::vector<std::string> files;
				// Iterate through the files in the subfolder
				for (const auto& fileEntry : std::filesystem::directory_iterator(entry.path())) {
					if (fileEntry.is_regular_file()) {
						files.push_back(fileEntry.path().string());  // Store the file name
					}
				}

				// Add the subfolder and its files to the map
				folderFileMap[subfolder] = files;
			}
		}
		return folderFileMap;
	}

	std::unordered_map<int, std::vector<std::shared_ptr<Img>>> LoadTrainingData(const std::string& trainingFolder)
	{
		//assumes folder has subfolders with name that matches the expected result, inside are images representing this result
		// trainig data is of form {r1,g1,b1,r2,g2,b2...}

		std::filesystem::path path(trainingFolder);
		std::unordered_map<int, std::vector<std::shared_ptr<Img>>> trainingData;

		if (!std::filesystem::is_directory(path))
		{
			Utils::printErr("The provided path is not a directory.");
			return trainingData;
		}

		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (!entry.is_directory())
			{
				Utils::printErr("Only subfolders allowed in path folder!");
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

	std::shared_ptr<Img8> LoadImage8(const std::string& filePath)
	{
		// Load the BMP image
		SDL_Surface* imageSurface = IMG_Load(filePath.c_str());
		if (!imageSurface) {
			std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		int width = imageSurface->w;
		int height = imageSurface->h;
		int pitch = imageSurface->pitch;

		std::vector<Uint8> imgBuffer;

		int bbp = pitch / width;
		if (bbp == 1)
		{
			Uint8* bytePixels = reinterpret_cast<Uint8*>(imageSurface->pixels);
			for (int i = 0; i < width * height * sizeof(Uint8); ++i)
			{
				Uint8 gray = bytePixels[i];
				gray = (gray > 170) ? 255 : 0;
				imgBuffer.push_back(gray);
			}
		}
		else if (bbp == 3)
		{
			Uint8* bytePixels = reinterpret_cast<Uint8*>(imageSurface->pixels);
			for (int i = 0; i < width * height * 3 * sizeof(Uint8); i += 3)
			{
				Uint8 gray = std::max({ bytePixels[i],bytePixels[i + 1],bytePixels[i + 2] });
				gray = (gray > 170) ? 255 : 0;
				imgBuffer.push_back(gray);
			}
		}
		else if (bbp == 4) //assuming ARGB
		{
			Uint8* bytePixels = reinterpret_cast<Uint8*>(imageSurface->pixels);
			for (int i = 0; i < width * height * 4 * sizeof(Uint8); i += 4)
			{
				Uint8 gray = std::max({ bytePixels[i + 1],bytePixels[i + 2],bytePixels[i + 3] });
				gray = (gray > 170) ? 255 : 0;
				imgBuffer.push_back(gray);
			}
		}
		else
		{
			Utils::print("Unsupported pixel format!");
			SDL_FreeSurface(imageSurface);
			return nullptr;
		}

		SDL_FreeSurface(imageSurface);
		return std::make_shared<Img8>(imgBuffer, width, height);
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

		std::cout << "Image loaded successfully!" << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "Format: " << format->format << std::endl;
		std::cout << "Pitch: " << pitch << std::endl;

		std::vector<Uint32> imgBuffer;

		int bbp = pitch / width;
		if (bbp == 1)
		{
			Uint8* bytePixels = reinterpret_cast<Uint8*>(imageSurface->pixels);
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

	void print(const std::string& str)
	{
		std::cout << str << std::endl;

	}
	void printErr(const std::string& str)
	{
		std::cerr << str << std::endl;

	}


	CastResult<int> TryCastInt(const std::string& str)
	{
		try
		{
			return { true,std::stoi(str) };
		}
		catch (...)
		{
			return { false,0 };
		}
	}
}