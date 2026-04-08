#include <Reflection/register_meta.h>

using namespace MirageEngine;

RegisterMeta::RegisterMeta()
{
	entt::meta_any empty{};
	entt::meta_any other{ std::in_place_type<void> };
}

RegisterMeta::~RegisterMeta()
{
}

void RegisterMeta::GameObjectMeta()
{
	using namespace entt::literals;		// for _hs (hash)

	entt::meta_factory<GameObject> gameObject{};

	gameObject.type("gameObjectReflection"_hs);

	gameObject.ctor<GameObject*, std::string, bool, bool>();

	gameObject.data<nullptr, &GameObject::GetName>("m_Name"_hs);
	gameObject.data<nullptr, &GameObject::GetComponents>("m_Components"_hs);

	gameObject.func<&GameObject::AddChild>("AddChild"_hs);
}
