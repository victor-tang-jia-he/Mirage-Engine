#include "console.h"
#include "UI_wrapper.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Mirage
{
	void Console::InitPanel()
	{
		AddLog("I am a test");
		AddLog("I am a second test");
		AddLog("You know the drill");
	}

	void Console::DrawPanel()
	{
		UIWrapper::BeginWindow("Console");
		if (ImGui::Button("Clear"))
		{
			m_Logs.clear();
		}

		for (auto& log : m_Logs)
		{
			ImGui::Text(log.c_str());
		}

		UIWrapper::EndWindow();
	}

	void Console::ShutdownPanel()
	{

	}

	void Console::AddLog(const char* _log)
	{
		m_Logs.push_back(_log);
	}
}