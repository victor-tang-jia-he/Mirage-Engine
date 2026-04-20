#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

using json = nlohmann::json;

namespace MirageEngine
{
	class SceneGraph;
	class GameObject;

	namespace Core::Components
	{
		class Component;
		class Transform;
	}

	class Serializer
	{
	public:
		json SerializeScene(SceneGraph _sceneGraph);

	private:
		json SerializeGameObject(std::shared_ptr<GameObject> _gameObject);
		json SerializeComponent(std::shared_ptr<Core::Components::Component> _component);
		json SerializeTransform(std::shared_ptr <Core::Components::Transform> _transform);
		json SerializeMetaData(entt::meta_type _type, entt::meta_data _data, void* _instancePtr);
	};
}