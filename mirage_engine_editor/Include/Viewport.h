#pragma once
#include "i_panel.h"

namespace Mirage
{
	class Viewport : public IPanel
	{
	public:
		Viewport(const char* _name) : IPanel(_name) {};
		void DrawPanel() override;
	};
}