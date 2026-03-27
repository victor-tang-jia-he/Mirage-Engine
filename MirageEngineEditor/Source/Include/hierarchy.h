#pragma once
#include "i_panel.h"

namespace Mirage
{
	class Hierarchy : public IPanel
	{
	public:
		Hierarchy(const char* _name) : IPanel(_name) {};

		void InitPanel() override;
		void DrawPanel() override;
		void ShutdownPanel() override;
	};
}