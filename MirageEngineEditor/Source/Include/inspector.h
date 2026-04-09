#pragma once
#include "i_panel.h"

namespace Mirage
{
	class Inspector : public IPanel
	{
	public:
		Inspector(const char* _name) : IPanel(_name) {};
		
		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;
	};
}