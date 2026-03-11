#include "../Include/UIWrapper.h"

#include "../../Libraries/imgui/imgui.h"
#include "../../Libraries/imgui/backends/imgui_impl_glfw.h"
#include "../../Libraries/imgui/backends/imgui_impl_opengl3.h"
#include "../../Libraries/GLFW/include/GLFW/glfw3.h"

namespace Mirage
{
	void UIWrapper::Init(void* windowHandler)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		GLFWwindow* window = static_cast<GLFWwindow*>(windowHandler);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void UIWrapper::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void UIWrapper::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
	}

	void UIWrapper::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}