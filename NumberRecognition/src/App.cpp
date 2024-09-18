#include "App.h"
#include <misc/cpp/imgui_stdlib.h>
#include <algorithm>

App::App()
{
	Utils::InitSDL();

	m_canvasRect.x = CANVAS_RECT_X;
	m_canvasRect.y = CANVAS_RECT_Y;
	m_canvasRect.w = CANVAS_RECT_WIDTH;
	m_canvasRect.h = CANVAS_RECT_HEIGHT;
}

App::~App()
{
	if (m_canvasTexture)
		SDL_DestroyTexture(m_canvasTexture);
	if (m_window)
		SDL_DestroyWindow(m_window);

	Utils::TerminateImgui();
	Utils::TerminateSDL();
}

int App::InitData()
{
	m_window = Utils::OpenWindow(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (m_window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_RendererInfo info;
	SDL_GetRendererInfo(m_renderer, &info);
	std::cout << "Renderer name: " << info.name << std::endl;
	std::cout << "Texture formats: " << std::endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	}


	//SDL_PIXELFORMAT_INDEX1MSB
	m_canvasTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, IMG_RESOLUTION_WIDTH, IMG_RESOLUTION_HEIGHT);
	if (m_canvasTexture == nullptr) {
		std::cerr << "Canvas texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	std::vector<Uint32> buffer = std::vector<Uint32>(IMG_RESOLUTION_WIDTH * IMG_RESOLUTION_HEIGHT);
	m_canvasImg = std::make_shared<Img>(buffer, IMG_RESOLUTION_WIDTH, IMG_RESOLUTION_HEIGHT, SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888));
	m_canvasImg->ResetBufferWithARGB8888Color(255, 255, 255, 255);

	m_testImg = Utils::LoadImage(TEST_IMG_PATH_RESIZED.c_str());

	return 0;
}

void App::UpdatePixelOnCanvas(std::shared_ptr<Img> img, int mouseX, int mouseY)
{
	// Check if the mouse click is inside the square
	if (mouseX >= m_canvasRect.x && mouseX <= m_canvasRect.x + m_canvasRect.w &&
		mouseY >= m_canvasRect.y && mouseY <= m_canvasRect.y + m_canvasRect.h) {

		int texX = (mouseX - m_canvasRect.x) / (m_canvasRect.w / img->Width);
		int texY = (mouseY - m_canvasRect.y) / (m_canvasRect.h / img->Height);

		texX = std::clamp(texX, 0, img->Width - 1);
		texY = std::clamp(texY, 0, img->Height - 1);

		img->SetPixelGray(texX, texY, 0);
		std::cout << "Mouse clicked at texture pixel: ("
			<< texX << ", " << texY << ")" << std::endl;
	}
}

int App::Run()
{
	if (InitData() < 0)
		return -1;

	Utils::InitIMGUI(m_window, m_renderer);

	bool doRun = true;
	while (doRun)
	{
		// Handle events on the queue
		while (SDL_PollEvent(&e) != 0) {
			ImGui_ImplSDL2_ProcessEvent(&e); // Forward your event to backend
			// User requests quit
			if (e.type == SDL_QUIT) {
				doRun = false;
			}

		}

		int mouseX, mouseY;
		if (SDL_GetMouseState(&mouseX, &mouseY) && SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			UpdatePixelOnCanvas(m_testImg,mouseX,mouseY);
		}

		Utils::ClearWindow(m_window, m_renderer, SCREEN_CLEAR_R, SCREEN_CLEAR_G, SCREEN_CLEAR_B);
		//SDL_UpdateTexture(m_canvasTexture, nullptr, m_canvasImg->Buffer.data(), m_canvasImg->Width * sizeof(Uint32));
		SDL_UpdateTexture(m_canvasTexture, nullptr, m_testImg->Buffer.data(), m_testImg->Width * sizeof(Uint32));
		SDL_RenderCopy(m_renderer, m_canvasTexture, nullptr, &m_canvasRect);

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		DrawUI();
		//ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);

		SDL_RenderPresent(m_renderer);
	}
	return 0;
}


void App::DrawUI()
{
	//*************
	// Under canvas

	ImGui::Begin("Result");
	if (ImGui::Button("Clear"))
	{
		m_canvasImg->ResetBufferWithARGB8888Color(255, 255, 255, 255);
	}
	ImGui::SameLine();
	if (ImGui::Button("RUN"))
	{
		int result = m_nnTrainer.CalculateValue(m_canvasImg->Buffer);

		if (result >= 0)
			std::cout << "Written number is: " << result << std::endl;
	}
	ImGui::End();

	//************
	// Right side

	ImGui::Begin("Input");

	//********
	// layer Sizes
	ImGui::Text("Layer sizes:");
	ImGui::SameLine();

	ImGui::InputText("##input1", &m_layersInput);
	if (ImGui::Button("Create new NN"))
	{
		std::vector<int> layerSizes = Utils::stringToVecOfInts(m_layersInput);

		if (m_nnTrainer.CreateNewNN(layerSizes))
			std::cout << "New NN created" << std::endl;
	}

	//*******
	// Training data

	ImGui::Text("Train Folder:");
	ImGui::SameLine();
	ImGui::InputText("##input2", &m_trainDataFolderInput);
	if (ImGui::Button("Load training data"))
	{
		std::cout << "Load training data";
	}

	//result
	ImGui::Text("Result:");
	ImGui::SameLine();
	ImGui::Text("x");


	//*******
	// Training

	ImGui::Text("Number of trainings:");
	ImGui::SameLine();
	ImGui::InputText("##input3", &m_trainCountInput);

	ImGui::Text("Learn rate:");
	ImGui::SameLine();
	ImGui::InputText("##input4", &m_learningRateInput);

	ImGui::Text("Minimum error:");
	ImGui::SameLine();
	ImGui::InputText("##input5", &m_minErrorInput);


	ImGui::End();
}

