#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace MirageEngine
{
	class GameObject;

	namespace Core::Components
	{
		class Transform;
	}

	class Serializer
	{
	public:
		json SerializeGameObject(const GameObject& _gameObject);

		json SerializeTransform(const Core::Components::Transform& _transform);
	};
}