#include "canpch.h"
#include "Can.h"
#include "Can/EntryPoint.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameLayer.h"

class Tetris : public Can::Application
{
public:
	Tetris()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins-Light.otf", 24.0f);
		PushLayer(new GameLayer());
	}

	~Tetris()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new Tetris();
}