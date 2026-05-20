#pragma once

#include <i_panel.h>

DECLARE_LOG_CATEGORY_EXTERN(LogViewport)

namespace Mirage
{
	class Viewport : public IPanel
	{
	public :
		enum class Type { Editor, Game };

	private:
		Type m_Type;

	public:

		Viewport(const char* _name, Type _type) : IPanel(_name), m_Type(_type) {};
			
		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;
		
		Type GetViewportType() const { return m_Type; }
	};
}