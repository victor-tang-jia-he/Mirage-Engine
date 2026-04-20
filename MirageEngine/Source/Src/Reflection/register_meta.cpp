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

	// Assign a unique ID to the class. 
	// "_hs" transforms the string into a unique integer at compile time.
	// 
	// gameObject.type("GameObject"_hs);
	gameObject.type(entt::type_id<GameObject>().hash());

	gameObject.ctor<std::shared_ptr<GameObject>, std::string, bool, bool>();

	gameObject.data<&GameObject::SetName, &GameObject::GetName>("Name");
	gameObject.data<&GameObject::SetIsActive, &GameObject::GetIsActive>("IsActive");
	gameObject.data<&GameObject::SetIsStatic, &GameObject::GetIsStatic>("IsStatic");

	gameObject.data<static_cast<void(GameObject::*)(Math::Vector3)>(&GameObject::SetPosition), &GameObject::GetPosition>("Position");
	gameObject.data<static_cast<void(GameObject::*)(Math::Quaternion)>(&GameObject::SetRotation), &GameObject::GetRotation>("Rotation");
	gameObject.data<static_cast<void(GameObject::*)(Math::Vector3)>(&GameObject::SetScale), &GameObject::GetScale>("Scale");
}

void RegisterMeta::RegisterTransformMeta()
{	
	entt::meta_factory<Components::Transform> transform{};

	transform.type(entt::type_id<Components::Transform>().hash());
	transform.type("Transform"_hs);

	transform.ctor<Math::Vector3, Math::Quaternion, Math::Vector3>();

	transform.data<&Components::Transform::SetPosition, &Components::Transform::GetPosition>("Position");
	transform.data<&Components::Transform::SetRotation, &Components::Transform::GetRotation>("Rotation");
	transform.data<&Components::Transform::SetScale, &Components::Transform::GetScale>("Scale");
	
	auto transType = entt::resolve("Transform"_hs);
	if (transType)
	{
		for (auto [id, data] : transType.data())
		{
		}
	}
}

void RegisterMeta::RegisterMeshRendererMeta()
{
	entt::meta_factory<MeshRenderer> meshRenderer{};

	// IMPORTANT: We use entt::type_id<T>().hash() as the ID because components are stored as base "Component*" pointers. 
	// When we call comp->GetType(), it returns the native C++ hash. By using the same hash here, the Inspector can "resolve" the correct meta-data.
	// Right now it causes a crash.
	// 
	// meshRenderer.type("MeshRendererReflection"_hs);
	meshRenderer.type(entt::type_id<MeshRenderer>().hash());

	meshRenderer.base<Component>();

	meshRenderer.data<&MeshRenderer::SetMesh, &MeshRenderer::GetMesh>("Mesh");
	meshRenderer.data<&MeshRenderer::SetMaterial, &MeshRenderer::GetMaterial>("Material");
}

void RegisterMeta::RegisterCameraMeta()
{
	entt::meta_factory<Camera> camera{};
	// camera.type("CameraReflection"_hs);
	camera.type(entt::type_id<Camera>().hash());


	camera.base<Component>();

	camera.data<&Camera::SetFov, &Camera::GetFov>("Field of View");
	camera.data<&Camera::SetNear, &Camera::GetNear>("Near Plane");
	camera.data<&Camera::SetFar, &Camera::GetFar>("Far Plane");
	camera.data<&Camera::SetWidth, &Camera::GetWidth>("Width");
	camera.data<&Camera::SetHeight, &Camera::GetHeight>("Height");
}

void RegisterMeta::RegisterInputMappingMeta()
{
	using namespace MirageEngine::Core::Input;

	entt::meta_factory<CallbackInputInfo> callbackInputInfo{};
	// callbackInputInfo.type("InputInfo"_hs);
	callbackInputInfo.type(entt::type_id<CallbackInputInfo>().hash());

	callbackInputInfo.data<&CallbackInputInfo::Pressed>("Pressed");
	callbackInputInfo.data<&CallbackInputInfo::Held>("Held");
	callbackInputInfo.data<&CallbackInputInfo::Released>("Released");;

	entt::meta_factory<InputMapping> inputMapping{};
	// inputMapping.type("InputMappingReflection"_hs);
	inputMapping.type(entt::type_id<InputMapping>().hash());

	inputMapping.base<Component>();

	inputMapping.data<&InputMapping::Map>("Bindings");

	inputMapping.func<&InputMapping::Clear>("ClearAll");

	// inputMapping.func<(void(InputMapping::*)(int))& InputMapping::Remove>("RemoveKey");
	inputMapping.func<&InputMapping::Remove>("RemoveKey");
}
