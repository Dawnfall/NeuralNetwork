#include "Canvas.h"

Canvas::Canvas(int posX, int posY, int width, int height)
{
	m_rect.x = posX;
	m_rect.y = posY;
	m_rect.w = width;
	m_rect.h = height;
}

Canvas::~Canvas()
{
	if (m_texture)
		SDL_DestroyTexture(m_texture);
}

void Canvas::SetImage(int width, int height, Uint8 defaultGray, SDL_Renderer* renderer)
{
	if (m_texture)
		SDL_DestroyTexture(m_texture);

	m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
	if (!m_texture)
	{
		Utils::printErr("Canvas texture could not be created! SDL_Error: " + std::string(SDL_GetError()));
		return;
	}

	m_img = std::make_shared<Img8>(width, height, defaultGray);
}

void Canvas::SetImage(const std::string& imagePath, SDL_Renderer* renderer)
{
	if (m_texture)
		SDL_DestroyTexture(m_texture);

	m_img = Utils::LoadImage8(imagePath);
	if (!m_img)
		return;

	m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, m_img->Width, m_img->Height);
	if (!m_texture)
		m_img = nullptr;
}

void Canvas::UpdatePixelOnCanvas(int mouseX, int mouseY)
{
	// Check if the mouse click is inside the square
	if (mouseX >= m_rect.x && mouseX <= m_rect.x + m_rect.w &&
		mouseY >= m_rect.y && mouseY <= m_rect.y + m_rect.h) {

		int texX = (mouseX - m_rect.x) / (m_rect.w / m_img->Width);
		int texY = (mouseY - m_rect.y) / (m_rect.h / m_img->Height);

		texX = std::clamp(texX, 0, m_img->Width - 1);
		texY = std::clamp(texY, 0, m_img->Height - 1);

		m_img->SetPixelGray(texX, texY, 0);

		//Utils::print("Mouse clicked at texture pixel: (" + std::to_string(texX) + ", " + std::to_string(texY) + ")");
	}
}

void Canvas::Render(SDL_Renderer* renderer)
{
	SDL_UpdateTexture(m_texture, nullptr, m_img->GetBufferARGB32Buffer().data(), m_img->Width * sizeof(Uint32));
	SDL_RenderCopy(renderer, m_texture, nullptr, &m_rect);
}