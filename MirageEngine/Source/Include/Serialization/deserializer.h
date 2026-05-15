#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <Logs/log.h>

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
        } // namespace Components

        namespace Math
        {
            class Vector3;
			class Vector4;
            class Quaternion;
        } // namespace Math

        namespace Resources
        {
            class IResource;
		} // namespace Resources
    } // namespace Core

	namespace AI
	{
		struct NavProfile;
	} // namespace AI

	class Deserializer
	{
    private:
		static std::unordered_map<std::string, json> m_ComponentSearchMap;

	public:
		static std::shared_ptr<SceneGraph> LoadSceneFromJson(json _jSavedScene);
		static std::shared_ptr<SceneGraph> LoadSceneFromFile(std::string _saveFileName);

	private:
		static std::shared_ptr<SceneGraph> DeserializeScene(json _jSceneGraph);
		static void DeserializeGameObject(json _jGameObject, std::shared_ptr<GameObject> _parent, std::shared_ptr<SceneGraph> _savedScene);
		
		static void DeserializeComponent(json _jComponent, std::shared_ptr<GameObject> _Owner);
		static entt::meta_any DeserializeTransform(json _jComponent, std::shared_ptr<GameObject> _Owner);
		
		static void DeserializeMetaData(entt::meta_type _type, entt::meta_data _data, entt::meta_any& _instance, json _jComponent);
		
		static AI::NavProfile DeserializeNavProfile(json _jNavProfile);

		static Core::Math::Vector3 DeserializeVector3(json _jVector3);
		static Core::Math::Vector4 DeserializeVector4(json _jVector4);
		static Core::Math::Quaternion DeserializeQuaternion(json _jQuaternion);

		static json ConvertJsonFileToJsonVar(std::string _fileName);

		template<typename T>
		static std::vector<T> DeserializeVector(json _jVector);
	};
}

#include "deserializer.inl"