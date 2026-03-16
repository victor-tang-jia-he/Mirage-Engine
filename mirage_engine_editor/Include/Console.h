#pragma once
#include "i_panel.h"

namespace Mirage
{
	class Console : public IPanel
	{
	public:
		Console(const char* _name) : IPanel(_name) {};
		
		void InitPanel() override;
		void DrawPanel() override;
		void ShutdownPanel() override;
	};
}