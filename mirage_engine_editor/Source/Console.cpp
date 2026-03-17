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

		AddLog("fire");

		ImGui::BeginChild("Scrolling Region", ImVec2(0, 230), ImGuiChildFlags_Borders);
		for (auto& log : m_Logs)
		{
			ImGui::Text(log.c_str());
		}
		ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();

		UIWrapper::EndWindow();
	}

	void Console::ShutdownPanel()
	{
		m_Logs.clear();
	}

	void Console::AddLog(const char* _log)
	{
		if(m_Logs.size() < 500)
		{
			m_Logs.push_back(_log);
		}
		else
		{
			m_Logs.erase(m_Logs.begin());
		}
	}
}