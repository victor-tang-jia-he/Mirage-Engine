#include "console.h"
#include "UI_wrapper.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Mirage
{
	void Console::InitPanel()
	{
		AddLog("I am a test", LogLevel::INFO);
		AddLog("I am a second test", LogLevel::ERROR);
		AddLog("You know the drill", LogLevel::WARNING);
	}

	void Console::DrawPanel(uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Console");

		if(ImGui::Button("Clear"))
			m_Logs.clear();
		ImGui::SameLine();
		m_SearchBar.Draw("Search", 150.0f);
		if (ImGui::TreeNode("Filters"))
		{
			ImGui::Checkbox("Info", &m_bDisplayInfo);
			ImGui::Checkbox("Warning", &m_bDisplayWarning);
			ImGui::Checkbox("Error", &m_bDisplayError);

			ImGui::TreePop();
		}

		//AddLog("fire", LogLevel::INFO);

		ImGui::BeginChild("Scrolling Region", ImVec2(0, 0), ImGuiChildFlags_Borders);

		bool isAtBottom = (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.0f);

		for (auto& log : m_Logs)
		{
			if (!m_SearchBar.PassFilter(log.Message.c_str()))
				continue;
			if(log.Level == LogLevel::ERROR && m_bDisplayError)
				ImGui::TextColored(ImVec4(0.8f, 0, 0, 1), "[ERROR] %s", log.Message.c_str());
			else if(log.Level == LogLevel::WARNING && m_bDisplayWarning)
				ImGui::TextColored(ImVec4(0.8f, 0.8f, 0, 1), "[WARNING] %s", log.Message.c_str());
			else if(log.Level == LogLevel::INFO && m_bDisplayInfo)
				ImGui::TextColored(ImVec4(1, 1, 1, 1), log.Message.c_str());
		}

		if (isAtBottom)
		{
			ImGui::SetScrollHereY(1.0f);
		}

		ImGui::EndChild();

		UIWrapper::EndWindow();
	}

	void Console::ShutdownPanel()
	{
		if (!std::filesystem::exists("Logs"))
			std::filesystem::create_directories("Logs");

		std::ofstream File("Logs/ConsoleLog.txt");

		for (auto& log : m_Logs)
		{
			File << log.Message << "\n";
		}

		File.close();
		m_Logs.clear();
	}

	void Console::AddLog(const char* _log, LogLevel _level)
	{
		if(m_Logs.size() < 500)
		{
			m_Logs.push_back(LogInfo(_log, _level));
		}
		else
		{
			m_Logs.erase(m_Logs.begin());
		}
	}
}