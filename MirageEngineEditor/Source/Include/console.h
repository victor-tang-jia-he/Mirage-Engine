#pragma once
#include "i_panel.h"
#include "imgui.h"
#include <vector>
#include <string>

namespace Mirage
{
	enum LogLevel
	{
		INFO,
		WARNING,
		ERROR
	};

	struct LogInfo
	{
		std::string Message;
		LogLevel Level = LogLevel::INFO;
	};

	class Console : public IPanel
	{
	private:
		std::vector<LogInfo> m_Logs;
		ImGuiTextFilter m_SearchBar;
		bool m_bDisplayInfo;
		bool m_bDisplayWarning;
		bool m_bDisplayError;

	public:
		Console(const char* _name) : IPanel(_name) {};
		
		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;
		void AddLog(const char* _log, LogLevel _level);
	};
}