#pragma once
#include "i_panel.h"

namespace Mirage
{
	class Explorer : public IPanel
	{
	public:
		Explorer(const char* _name) : IPanel(_name) {};
		void DrawPanel() override;
	};
}