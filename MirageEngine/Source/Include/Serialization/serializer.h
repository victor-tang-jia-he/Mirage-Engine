#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

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
        }
    } // namespace Core

    namespace AI
    {
        class NavProfile;
	} // namespace AI

    class Serializer
    {
    public:

        // Save SceneGraph as a Json file
        static void SaveScene(SceneGraph _sceneGraph);

    private:

        static json SerializeScene(SceneGraph _sceneGraph);
        static json SerializeGameObject(std::shared_ptr<GameObject> _gameObject, json _jParent);

        static json SerializeComponent(std::shared_ptr<Core::Components::Component> _component);
        static json SerializeTransform(Core::Components::Transform _transform);

        // Serializes Components reflected meta data
        static json SerializeMetaData(entt::meta_type _type, entt::meta_data _data, void* _instancePtr);

        static json SerializeNavProfile(AI::NavProfile _navProfile);

        static json SerializeVector3(Core::Math::Vector3 _vector3);
        static json SerializeVector4(Core::Math::Vector4 _vector4);
        static json SerializeQuaternion(Core::Math::Quaternion _quaternion);

        static bool ConvertJsonVarToJsonFile(json _jTarget);

        template <typename T>
        static json SerializeVector(std::vector<T> _vector);

		template <typename T>
		static json SerializeMap(std::unordered_map<std::string, T> _map);
    };
} // namespace MirageEngine

#include "serializer.inl"
