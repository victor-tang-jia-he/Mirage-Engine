#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogDeserializer)

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

	class Deserializer
	{
	public:
		static std::shared_ptr<SceneGraph> LoadSceneFromJson(json _jSavedScene);
		static std::shared_ptr<SceneGraph> LoadSceneFromFile(std::string _saveFileName);

	private:
		static std::shared_ptr<SceneGraph> DeserializeScene(json _jSceneGraph);
		static void DeserializeGameObject(json _jGameObject, std::shared_ptr<GameObject> _parent, std::shared_ptr<SceneGraph> _savedScene);
		
		static void DeserializeComponent(json _jComponent, std::shared_ptr<GameObject> _Owner);
		
		static void DeserializeMetaData(entt::meta_type _type, entt::meta_data _data, entt::meta_any& _instance, json _jComponent);
		
		static json DeserializeVector3(Core::Math::Vector3 _vector3);
		static json DeserializeQuaternion(Core::Math::Quaternion _quaternion);

		static json ConvertJsonFileToJsonVar(std::string _fileName);
	};
}