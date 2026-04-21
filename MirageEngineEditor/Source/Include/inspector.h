#pragma once
#include <Logs/log_macro.h>

#include "i_panel.h"
#include <entt/entt.hpp>

DECLARE_LOG_CATEGORY_EXTERN(LogInspector)

namespace MirageEngine
{
	class SceneGraph;
	namespace Core::Components { class Component; }
}

namespace Mirage
{
	class Inspector : public IPanel
	{
	private:
		MirageEngine::SceneGraph* m_Context;
	public:
		Inspector(const char* _name, MirageEngine::SceneGraph* _context)
			: IPanel(_name), m_Context(_context) {
		}
		
		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;

		void DrawProperty(entt::meta_data _data, entt::meta_any _instance);
		void DrawPropertyDirect(entt::meta_type _type, entt::meta_data _data, void* _instancePtr);
	};
}