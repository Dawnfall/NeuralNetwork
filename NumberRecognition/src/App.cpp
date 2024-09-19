#include "App.h"
#include <misc/cpp/imgui_stdlib.h>
#include <algorithm>

App::App() :
	m_canvas(CANVAS_RECT_X, CANVAS_RECT_Y, CANVAS_RECT_WIDTH, CANVAS_RECT_HEIGHT)
{
	Utils::InitSDL();
}

App::~App()
{
	if (m_renderer)
		SDL_DestroyRenderer(m_renderer);
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

	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(m_renderer, &info);
	//std::cout << "Renderer name: " << info.name << std::endl;
	//std::cout << "Texture formats: " << std::endl;
	//for (Uint32 i = 0; i < info.num_texture_formats; i++)
	//{
	//	std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	//}

	//m_canvas.SetImage(TEST_IMG_PATH_ORIGINAL, m_renderer);
	m_canvas.SetImage(IMG_RESOLUTION_WIDTH, IMG_RESOLUTION_HEIGHT, 255, m_renderer);


	return 0;
}



int App::Run()
{
	if (InitData() < 0)
		return -1;

	Utils::InitIMGUI(m_window, m_renderer);

	bool doRun = true;
	while (doRun)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			ImGui_ImplSDL2_ProcessEvent(&e);

			if (e.type == SDL_QUIT) {
				doRun = false;
			}

		}

		int mouseX, mouseY;
		if (SDL_GetMouseState(&mouseX, &mouseY) && SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			m_canvas.UpdatePixelOnCanvas(mouseX, mouseY);
		}

		Utils::ClearWindow(m_window, m_renderer, SCREEN_CLEAR_R, SCREEN_CLEAR_G, SCREEN_CLEAR_B);
		m_canvas.Render(m_renderer);

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		DrawUI();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);

		SDL_RenderPresent(m_renderer);
	}
	return 0;
}

void App::OnClearCanvasButtonClick()
{
	m_canvas.m_img->ResetBufferWithARGB8888Color(255);
}
void App::OnCreateNewNNButtonClick()
{
	std::vector<int> layerSizes = Utils::stringToVecOfInts(m_layersInput);

	if (m_nnTrainer.CreateNewNN(layerSizes))
		Utils::print("New NN created");
	else
		Utils::print("Unable to create NN");
}
void App::OnRunButtonClick()
{
	if (!m_canvas.m_img)
		return;

	int result = m_nnTrainer.CalculateValue(m_canvas.m_img->GrayBuffer);

	if (result >= 0)
	{
		m_resultText = std::to_string(result);
		Utils::print("Written number is: " + std::to_string(result));
	}
	else
	{
		Utils::print("Written number wasnt calculated!");

	}
}
void App::OnLoadDataButtonClick()
{
	m_nnTrainer.LoadTrainData(m_trainDataFolderInput);
	Utils::print("Train data loading completed.");

}
void App::OnTrainButtonClick()
{
	float learnRate = 0.0f;
	try
	{
		learnRate = std::stof(m_learningRateInput);
	}
	catch (...)
	{
		Utils::printErr("Invalid learning rate input");
		return;
	}

	int trainCount = 0;
	try
	{
		trainCount = std::stoi(m_trainCountInput);
	}
	catch (...)
	{
		Utils::printErr("Invalid train count");
		return;
	}

	m_nnTrainer.TrainNN(); //TODO: use parameters
	Utils::print("Training complete!");
}


void App::DrawUI()
{
	//*************
	// Under canvas

	ImGui::Begin("Result");
	if (ImGui::Button("Clear"))
	{
		OnClearCanvasButtonClick();
	}

	ImGui::SameLine();
	if (ImGui::Button("RUN"))
	{
		OnRunButtonClick();
	}


	//result
	ImGui::Text("Result:");
	ImGui::SameLine();
	ImGui::Text(m_resultText.c_str());

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
		OnCreateNewNNButtonClick();
	}

	//*******
	// Training data

	ImGui::Text("Train Folder:");
	ImGui::SameLine();
	ImGui::InputText("##input2", &m_trainDataFolderInput);
	if (ImGui::Button("Load training data"))
	{
		OnLoadDataButtonClick();
	}


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

	if (ImGui::Button("Train"))
	{
		OnTrainButtonClick();
	}

	ImGui::End();
}

