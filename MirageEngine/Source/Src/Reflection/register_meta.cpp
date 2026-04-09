#include <Reflection/register_meta.h>
#include "Components/mesh_renderer.h"

// Components includes
#include <GameObject/game_object.h>
#include <Components/transform.h>
#include <Components/camera.h>
#include <Components/input_mapping.h>

// EnTT includes
#include <entt/entt.hpp>

#include <vector3.h>
#include <quaternion.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace MirageEngine::Core::Components;

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
	RegisterCameraMeta();
	RegisterInputMappingMeta();
}

void RegisterMeta::RegisterGameObjectMeta()
{
	entt::meta_factory<GameObject> gameObject{};

	gameObject.type("GameObject"_hs);

	gameObject.ctor<GameObject*, std::string, bool, bool>();

	gameObject.data<&GameObject::SetName, &GameObject::GetName>("Name"_hs);
	gameObject.data<&GameObject::SetIsActive, &GameObject::GetIsActive>("IsActive"_hs);
	gameObject.data<&GameObject::SetIsStatic, &GameObject::GetIsStatic>("IsStatic"_hs);
}

void RegisterMeta::RegisterTransformMeta()
{
	entt::meta_factory<Components::Transform> transform{};

	transform.type("Transform"_hs);

	transform.ctor<Math::Vector3, Math::Quaternion, Math::Vector3>();

	transform.data<&Components::Transform::SetPosition, &Components::Transform::GetPosition>("Position"_hs);
	transform.data<&Components::Transform::SetRotation, &Components::Transform::GetRotation>("Rotation"_hs);
	transform.data<&Components::Transform::SetRotation, &Components::Transform::GetScale>("Scale"_hs);
}
void RegisterMeta::RegisterMeshRendererMeta()
{
	entt::meta_factory<MeshRenderer> meshRenderer{};
	meshRenderer.type("MeshRendererReflection"_hs);

	meshRenderer.base<Component>();

	meshRenderer.data<&MeshRenderer::SetMesh, &MeshRenderer::GetMesh>("Mesh"_hs);
	meshRenderer.data<&MeshRenderer::SetMaterial, &MeshRenderer::GetMaterial>("Material"_hs);
}

void RegisterMeta::RegisterCameraMeta()
{
	entt::meta_factory<Camera> camera{};
	camera.type("CameraReflection"_hs);

	camera.base<Component>();

	camera.data<&Camera::SetFov, &Camera::GetFov>("Field of View"_hs);
	camera.data<&Camera::SetNear, &Camera::GetNear>("Near Plane"_hs);
	camera.data<&Camera::SetFar, &Camera::GetFar>("Far Plane"_hs);
	camera.data<&Camera::SetWidth, &Camera::GetWidth>("Width"_hs);
	camera.data<&Camera::SetHeight, &Camera::GetHeight>("Height"_hs);
}

void RegisterMeta::RegisterInputMappingMeta()
{
	using namespace MirageEngine::Core::Input;

	entt::meta_factory<CallbackInputInfo> callbackInputInfo{};
	callbackInputInfo.type("InputInfo"_hs);

	callbackInputInfo.data<&CallbackInputInfo::Pressed>("Pressed"_hs);
	callbackInputInfo.data<&CallbackInputInfo::Held>("Held"_hs);
	callbackInputInfo.data<&CallbackInputInfo::Released>("Released"_hs);;

	entt::meta_factory<InputMapping> inputMapping{};
	inputMapping.type("InputMappingReflection"_hs);

	inputMapping.base<Component>();

	inputMapping.data<&InputMapping::Map>("Bindings"_hs);

	inputMapping.func<&InputMapping::Clear>("ClearAll"_hs);
	inputMapping.func<(void(InputMapping::*)(int))& InputMapping::Remove>("RemoveKey"_hs);
}
