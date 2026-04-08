#include <Reflection/register_meta.h>
#include "Components/mesh_renderer.h"

// Components includes
#include <GameObject/game_object.h>
#include <Components/transform.h>
#include <Components/camera.h>
#include <Components/input_mapping.h>

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
	RegisterTransformMeta();
	RegisterMeshRendererMeta();
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
void RegisterMeta::RegisterMeshRendererMeta()
{	
	using namespace entt::literals;
	using MeshRenderer = Core::Components::MeshRenderer;
	using Component = Core::Components::Component;

	entt::meta_factory<MeshRenderer>{}
	.type("MeshRendererReflection"_hs)
		.base<Component>()
		.data<&MeshRenderer::SetMesh, &MeshRenderer::GetMesh>("Mesh"_hs)
		.data<&MeshRenderer::SetMaterial, &MeshRenderer::GetMaterial>("Material"_hs);
}

void RegisterMeta::RegisterCameraMeta()
{
	using namespace entt::literals;
	using Camera = Core::Components::Camera;
	using Component = Core::Components::Component;

	entt::meta_factory<Camera>{}
	.type("CameraReflection"_hs)
		.base<Component>()
		.data<&Camera::SetFov, &Camera::GetFov>("Field of View"_hs)
		.data<&Camera::SetNear, &Camera::GetNear>("Near Plane"_hs)
		.data<&Camera::SetFar, &Camera::GetFar>("Far Plane"_hs)
		.data<&Camera::SetWidth, &Camera::GetWidth>("Width"_hs)
		.data<&Camera::SetHeight, &Camera::GetHeight>("Height"_hs);
}

void RegisterMeta::RegisterInputMappingMeta()
{
	using namespace entt::literals;
}
