#include "App.h"
#include <misc/cpp/imgui_stdlib.h>
#include <algorithm>

#include "Constants.h"

App::App()
{
	Utils::InitSDL();
}

App::~App()
{
	if (m_imageManager.GetRenderer())
		SDL_DestroyRenderer(m_imageManager.GetRenderer());
	if (m_window)
		SDL_DestroyWindow(m_window);

	Utils::TerminateImgui();
	Utils::TerminateSDL();
}

void App::DrawUI_ImageRecognition()
{
	m_imageManager.GetCanvas().Render(m_imageManager.GetRenderer());

	int mouseX, mouseY;
	if (SDL_GetMouseState(&mouseX, &mouseY) && SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		m_imageManager.GetCanvas().UpdatePixelOnCanvas(mouseX, mouseY);
	}

	ImGui::Begin("Neural network");

	ImGui::Text(m_imageManager.Image_NN_text.c_str());

	ImGui::End();

	ImGui::Begin("Image Recognition ...TODO...");

	if (ImGui::Button("Clear Image"))
		m_imageManager.GetCanvas().m_img->ResetBufferWithARGB8888Color(255);

	ImGui::Text("Layer sizes:");
	ImGui::SameLine();

	ImGui::InputText("##input1", &m_imageManager.Image_layersInput);
	if (ImGui::Button("Create new NN"))
		m_imageManager.CreateNewNN();

	ImGui::Text("Number of trainings:");
	ImGui::SameLine();
	ImGui::InputText("##input3", &m_imageManager.Image_trainCountInput);

	ImGui::Text("Learn rate:");
	ImGui::SameLine();
	ImGui::InputText("##input4", &m_imageManager.Image_learningRateInput);

	ImGui::Text("Minimum error:");
	ImGui::SameLine();
	ImGui::InputText("##input5", &m_imageManager.Image_minErrorInput);

	ImGui::Text("Train Folder:");
	ImGui::SameLine();
	ImGui::InputText("##input2", &m_imageManager.Image_trainDataFolderInput);

	if (ImGui::Button("Train"))
		m_imageManager.Train();
	
	if (ImGui::Button("Detect Number"))
		m_imageManager.CalculateValue();

	ImGui::Text(m_imageManager.Image_resultText.c_str());

	ImGui::End();
}

void App::DrawUI_XOR()
{
	ImGui::Begin("Neural network");

	ImGui::Text(m_xorManager.Xor_NN_text.c_str());

	ImGui::End();

	ImGui::Begin("XOR");

	ImGui::Text("Layer sizes:");
	ImGui::SameLine();

	ImGui::InputText("##input1", &m_xorManager.Xor_layersInput);
	if (ImGui::Button("Create new NN"))
		m_xorManager.CreateNewNN();

	ImGui::Text("Number of trainings:");
	ImGui::SameLine();
	ImGui::InputText("##input3", &m_xorManager.Xor_maxTrainInput);

	ImGui::Text("Learn rate:");
	ImGui::SameLine();
	ImGui::InputText("##input4", &m_xorManager.Xor_m_learnRateInput);

	ImGui::Text("Minimum error:");
	ImGui::SameLine();
	ImGui::InputText("##input5", &m_xorManager.Xor_minErrInput);

	if (ImGui::Button("Train"))
		m_xorManager.Train();

	ImGui::Text("TEST");

	ImGui::Text("a = ");
	ImGui::SameLine();
	ImGui::InputText("##inputTestA", &m_xorManager.Xor_test_aInput);
	ImGui::Text("b = ");
	ImGui::SameLine();
	ImGui::InputText("##inputTestB", &m_xorManager.Xor_test_bInput);

	if (ImGui::Button("RUN"))
		m_xorManager.Run();

	//result
	ImGui::Text(m_xorManager.Xor_test_resultText.c_str());

	ImGui::End();
}

int App::InitData()
{
	m_window = Utils::OpenWindow(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (m_window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	m_imageManager.SetRenderer(SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED));
	if (m_imageManager.GetRenderer() == nullptr) {
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
	m_imageManager.GetCanvas().SetImage(IMG_RESOLUTION_WIDTH, IMG_RESOLUTION_HEIGHT, 255, m_imageManager.GetRenderer());

	return 0;
}



int App::Run()
{
	if (InitData() < 0)
		return -1;

	Utils::InitIMGUI(m_window, m_imageManager.GetRenderer());

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

		Utils::ClearWindow(m_window, m_imageManager.GetRenderer(), SCREEN_CLEAR_R, SCREEN_CLEAR_G, SCREEN_CLEAR_B);

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		switch (m_state)
		{
		case AppState::XOR:
			DrawUI_XOR();
			break;
		case AppState::IMAGE:
			DrawUI_ImageRecognition();
			break;
		default:
			break;
		}

		ImGui::Begin("Select Mode");

		if (ImGui::Button("Image"))
			m_state = AppState::IMAGE;
		ImGui::SameLine();
		if (ImGui::Button("XOR"))
			m_state = AppState::XOR;

		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_imageManager.GetRenderer());

		SDL_RenderPresent(m_imageManager.GetRenderer());
	}
	return 0;
}










