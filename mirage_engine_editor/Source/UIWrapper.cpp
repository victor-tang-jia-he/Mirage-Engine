#include "../Include/UIWrapper.h"
#include "vector3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace Math = MirageEngine::Core::Maths;

namespace Mirage
{
	void UIWrapper::Init(void* windowHandler)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();

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
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::ShowDemoWindow();
	}

	void UIWrapper::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//ImGuiIO& io = ImGui::GetIO();
		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	GLFWwindow* backup_current_context = glfwGetCurrentContext();

		//	ImGui::UpdatePlatformWindows();
		//	ImGui::RenderPlatformWindowsDefault();

		//	glfwMakeContextCurrent(backup_current_context);
		//}
	}

	bool UIWrapper::BeginWindow(const char* windowName, bool* isOpen)
	{
		return ImGui::Begin(windowName, isOpen);
	}

	void UIWrapper::EndWindow()
	{
		ImGui::End();
	}

	void UIWrapper::CreateSeparatorText(const char* separatorName)
	{
		ImGui::SeparatorText(separatorName);
	}

	void UIWrapper::CreateColorPicker(Math::Vector3* color)
	{
		ImVec4 Color(color->x / 255.0f, color->y / 255.0f, color->z / 255.0f, 1.0f);
		ImVec4 refVecColor(1.0f, 0.0f, 1.0f, 1.0f);
		ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_NoAlpha;
		ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

		ImGuiColorEditFlags flags = base_flags | color_picker_flags;
		ImGui::ColorPicker4("MyColor##4", (float*)&Color, flags, false ? &refVecColor.x : NULL);
	}

	bool UIWrapper::CreateSliderInt(const char* sliderName, int* value, int vMin, int vMax)
	{
		return ImGui::SliderInt(sliderName, value, vMin, vMax);
	}

	bool UIWrapper::CreateSliderFloat(const char* sliderName, float* value, float vMin, float vMax)
	{
		return ImGui::SliderFloat(sliderName, value, vMin, vMax, "%.3f");
	}

	bool UIWrapper::CreateButton(const char* buttonName)
	{ 
		return ImGui::Button(buttonName);
	}

	bool UIWrapper::CreateCheckbox(const char* checkboxName, bool* check)
	{
		return ImGui::Checkbox(checkboxName, check);
	}
}