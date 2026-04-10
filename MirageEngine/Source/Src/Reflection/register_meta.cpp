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

	gameObject.data<&GameObject::SetName, &GameObject::GetName>("Name");
	gameObject.data<&GameObject::SetIsActive, &GameObject::GetIsActive>("IsActive");
	gameObject.data<&GameObject::SetIsStatic, &GameObject::GetIsStatic>("IsStatic");
}

void RegisterMeta::RegisterTransformMeta()
{
	std::cout << "[DEBUG REGISTER] Registering Transform metadata\n" << std::flush;
	
	entt::meta_factory<Components::Transform> transform{};

	transform.type("Transform"_hs);
	transform.ctor<Math::Vector3, Math::Quaternion, Math::Vector3>();

	std::cout << "[DEBUG REGISTER] Setting Position property with setter/getter\n" << std::flush;
	transform.data<&Components::Transform::SetPosition, &Components::Transform::GetPosition>("Position");
	
	std::cout << "[DEBUG REGISTER] Setting Rotation property with setter/getter\n" << std::flush;
	transform.data<&Components::Transform::SetRotation, &Components::Transform::GetRotation>("Rotation");
	
	std::cout << "[DEBUG REGISTER] Setting Scale property with setter/getter\n" << std::flush;
	transform.data<&Components::Transform::SetScale, &Components::Transform::GetScale>("Scale");
	
	auto transType = entt::resolve("Transform"_hs);
	if (transType) {
		std::cout << "[DEBUG REGISTER] Transform type resolved successfully\n" << std::flush;
		for (auto [id, data] : transType.data()) {
			std::cout << "[DEBUG REGISTER]   - Property: " << data.name() << "\n" << std::flush;
		}
	} else {
		std::cout << "[DEBUG REGISTER] ERREUR: Transform type could not be resolved\n" << std::flush;
	}
}

void RegisterMeta::RegisterMeshRendererMeta()
{
	entt::meta_factory<MeshRenderer> meshRenderer{};
	meshRenderer.type("MeshRendererReflection"_hs);

	meshRenderer.base<Component>();

	meshRenderer.data<&MeshRenderer::SetMesh, &MeshRenderer::GetMesh>("Mesh");
	meshRenderer.data<&MeshRenderer::SetMaterial, &MeshRenderer::GetMaterial>("Material");
}

void RegisterMeta::RegisterCameraMeta()
{
	entt::meta_factory<Camera> camera{};
	camera.type("CameraReflection"_hs);

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
	callbackInputInfo.type("InputInfo"_hs);

	callbackInputInfo.data<&CallbackInputInfo::Pressed>("Pressed");
	callbackInputInfo.data<&CallbackInputInfo::Held>("Held");
	callbackInputInfo.data<&CallbackInputInfo::Released>("Released");;

	entt::meta_factory<InputMapping> inputMapping{};
	inputMapping.type("InputMappingReflection"_hs);

	inputMapping.base<Component>();

	inputMapping.data<&InputMapping::Map>("Bindings");

	inputMapping.func<&InputMapping::Clear>("ClearAll");
	inputMapping.func<(void(InputMapping::*)(int))& InputMapping::Remove>("RemoveKey");
}
