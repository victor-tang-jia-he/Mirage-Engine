#include "UI_wrapper.h"
#include "vector3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace Math = MirageEngine::Core::Math;

namespace Mirage
{
	void UIWrapper::Init(void* _windowHandle)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();

		GLFWwindow* window = static_cast<GLFWwindow*>(_windowHandle);
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
		//ImGui::ShowDemoWindow();
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

	bool UIWrapper::BeginWindow(const char* _windowName, bool* _isOpen)
	{
		return ImGui::Begin(_windowName, _isOpen);
	}

	void UIWrapper::EndWindow()
	{
		ImGui::End();
	}

	void UIWrapper::CreateSeparatorText(const char* _separatorName)
	{
		ImGui::SeparatorText(_separatorName);
	}

	void UIWrapper::CreateColorPicker(Math::Vector3* _color)
	{
		ImVec4 Color(_color->X / 255.0f, _color->Y / 255.0f, _color->Z / 255.0f, 1.0f);
		ImVec4 refVecColor(1.0f, 0.0f, 1.0f, 1.0f);
		ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_NoAlpha;
		ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

		ImGuiColorEditFlags flags = base_flags | color_picker_flags;
		ImGui::ColorPicker4("MyColor##4", (float*)&Color, flags, false ? &refVecColor.x : NULL);
	}

	bool UIWrapper::CreateSliderInt(const char* _sliderName, int* _value, int _vMin, int _vMax)
	{
		return ImGui::SliderInt(_sliderName, _value, _vMin, _vMax);
	}

	bool UIWrapper::CreateSliderFloat(const char* _sliderName, float* _value, float _vMin, float _vMax)
	{
		return ImGui::SliderFloat(_sliderName, _value, _vMin, _vMax, "%.3f");
	}

	bool UIWrapper::CreateButton(const char* _buttonName)
	{ 
		return ImGui::Button(_buttonName);
	}

	bool UIWrapper::CreateCheckbox(const char* _checkboxName, bool* _check)
	{
		return ImGui::Checkbox(_checkboxName, _check);
	}
}