#pragma once
#include <Logs/log_macro.h>

#include "i_panel.h"
#include <entt/entt.hpp>

DECLARE_LOG_CATEGORY_EXTERN(LogInspector)

namespace MirageEngine
{
	class SceneGraph;
	namespace Core::Components { class Component; }
	class GameObject;
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

	private:
		void ResolveVec3(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolveQuaternion(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolveString(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolveInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolveFloat(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolveBool(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolveUnsignedInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);
		void ResolvePointer(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any &_instance);

		bool ResolveComponent(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance);
		bool ResolveResource(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance);

		void ResolveMesh(entt::meta_any _value);
		void ResolveMaterial(entt::meta_any _value);
		void ResolveTexture(entt::meta_any _value);
		void ResolveShader(entt::meta_any _value);

		void RenderAddComponentPopup(std::shared_ptr<MirageEngine::GameObject> _selectedObj);
	};
}