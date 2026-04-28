#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogSerializer)

using json = nlohmann::json;

namespace MirageEngine
{
	class SceneGraph;
	class GameObject;

	namespace Core
	{
		namespace Components
		{
			class Component;
			class Transform;
		}
		namespace Math
		{
			class Vector3;
			class Quaternion;
		}
		namespace Resources
		{
			class IResource;
		}
	}

	class Serializer
	{
	public:
		// Save SceneGraph as a Json file
		static void SaveScene(SceneGraph _sceneGraph);

	private:
		static json SerializeScene(SceneGraph _sceneGraph);
		static json SerializeGameObject(std::shared_ptr<GameObject> _gameObject, json _jParent);

		static json SerializeComponent(std::shared_ptr<Core::Components::Component> _component, json _Owner);
		static json SerializeTransform(Core::Components::Transform _transform);

		// Serializes Components reflected meta data
		static json SerializeMetaData(entt::meta_type _type, entt::meta_data _data, void* _instancePtr);

		static json SerializeVector3(Core::Math::Vector3 _vector3);
		static json SerializeQuaternion(Core::Math::Quaternion _quaternion);

		static bool ConvertJsonVarToJsonFile(json _jTarget);
	};
}