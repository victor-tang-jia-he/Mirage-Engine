#pragma once
#include "i_panel.h"
#include <vector>
#include <string>

namespace Mirage
{
	class Console : public IPanel
	{
	private:
		std::vector<std::string> m_Logs;

	public:
		Console(const char* _name) : IPanel(_name) {};
		
		void InitPanel() override;
		void DrawPanel() override;
		void ShutdownPanel() override;
		void AddLog(const char* _log);
	};
}