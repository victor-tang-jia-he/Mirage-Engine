#include <Reflection/register_meta.h>

// Components includes
#include <GameObject/game_object.h>
#include <Components/transform.h>

// EnTT includes
#include <entt/entt.hpp>
//#include <entt/meta/factory.hpp>
//#include <entt/meta/container.hpp>
//#include <entt/meta/meta.hpp>

#include <vector3.h>
#include <quaternion.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace entt::literals;		// for _hs (hash)

RegisterMeta::RegisterMeta()
{
	entt::meta_any empty{};
	entt::meta_any other{ std::in_place_type<void> };
}

RegisterMeta::~RegisterMeta()
{
}

void RegisterMeta::RegisterAllMeta()
{
	RegisterGameObjectMeta();
}

void RegisterMeta::RegisterGameObjectMeta()
{
	entt::meta_factory<GameObject> gameObject{};

	gameObject.type("GameObject"_hs);

	gameObject.ctor<GameObject*, std::string, bool, bool>();

	gameObject.data<nullptr, &GameObject::GetName>("Name"_hs);
	gameObject.data<nullptr, &GameObject::GetComponents>("Components"_hs);

	gameObject.func<&GameObject::AddChild>("AddChild"_hs);
}

void RegisterMeta::RegisterTransformMeta()
{
	entt::meta_factory<Components::Transform> transform{};

	transform.type("Transform"_hs);

	transform.ctor<Math::Vector3, Math::Quaternion, Math::Vector3>();

	transform.data<nullptr, &Components::Transform::GetPosition>("Position"_hs);
	transform.data<nullptr, &Components::Transform::GetRotation>("Rotation"_hs);
	transform.data<nullptr, &Components::Transform::GetScale>("Scale"_hs);
}