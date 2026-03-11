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

	bool UIWrapper::CreateSliderInt(const char* sliderName, int* value, int vMin, int vMax)
	{
		ImGui::SliderInt(sliderName, value, vMin, vMax);
		return false;
	}

	bool UIWrapper::CreateSliderFloat(const char* sliderName, float* value, float vMin, float vMax)
	{
		ImGui::SliderFloat(sliderName, value, vMin, vMax, "%.3f");
		return false;
	}

	bool UIWrapper::CreateButton(const char* buttonName)
	{ 
		ImGui::Button(buttonName);
		return false;
	}

	bool UIWrapper::CreateCheckbox(const char* checkboxName, bool* check)
	{
		ImGui::Checkbox(checkboxName, check);
		return false;
	}
}