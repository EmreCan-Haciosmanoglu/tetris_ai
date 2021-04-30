#include "canpch.h"
#include "Can.h"
#include "Can/EntryPoint.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameLayer.h"

class Tetris : public Can::Application
{
public:
	Tetris(const Can::WindowProps& props)
		:Application(props)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-Light.ttf", 24.0f);
		PushLayer(new GameLayer());
	}

	~Tetris()
	{

	}
};

Can::Application* Can::CreateApplication(const Can::WindowProps& props)
{
	return new Tetris(props);
}