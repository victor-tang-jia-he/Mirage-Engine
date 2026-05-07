#include <Reflection/register_meta.h>
#include "Components/mesh_renderer.h"
#include <Resources/mesh.h>
#include <Resources/texture.h>

// Components includes
#include <GameObject/game_object.h>
#include <Components/transform.h>
#include <Components/camera.h>
//#include <Components/input_mapping.h>
#include <Components/influence_emitter.h>
#include <AI/influence_grid.h>
#include <Input/input_mapping.h>
#include <AI/responsive_curve.h>
#include <Components/nav_agent.h>
#include <Components/utility_agent.h>

// Ressources includes
#include <Resources/material.h>
#include <Resources/shader.h>

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

void RegisterMeta::RegisterAllMeta(entt::locator<entt::meta_ctx>::node_type handle)
{
    entt::locator<entt::meta_ctx>::reset(handle);

	RegisterGameObjectMeta();

	RegisterTransformMeta();

	RegisterMeshRendererMeta();
	RegisterCameraMeta();
	RegisterInputMappingMeta();
	RegisterInfluenceGridMeta();
	RegisterInfluenceEmitterMeta();
	RegisterNavAgentMeta();   
	RegisterUtilityAgentMeta();
	RegisterResponsiveCurveMeta();

	RegisterMeshMeta();
	RegisterTextureMeta();
	RegisterMaterialMeta();
	RegisterShaderMeta();
}

void RegisterMeta::RegisterGameObjectMeta()
{
	entt::meta_factory<GameObject> gameObject{};

	// Assign a unique ID to the class. 
	// "_hs" transforms the string into a unique integer at compile time.
	gameObject.type(entt::type_id<GameObject>().hash(), "GameObject");

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

	transform.type(entt::type_id<Transform>().hash(), "Transform");
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
	meshRenderer.type(entt::type_id<MeshRenderer>().hash(), "MeshRenderer");

	meshRenderer.base<Component>();

	meshRenderer.data<&MeshRenderer::SetMesh, &MeshRenderer::GetMesh>("Mesh");
	meshRenderer.data<&MeshRenderer::SetMaterial, &MeshRenderer::GetMaterial>("Material");
}

void RegisterMeta::RegisterCameraMeta()
{
	entt::meta_factory<Camera> camera{};

	camera.type(entt::type_id<Camera>().hash(), "Camera");

	camera.base<Component>();

	camera.data<&Camera::SetFov, &Camera::GetFov>("FOV");
	camera.data<&Camera::SetNear, &Camera::GetNear>("Near");
	camera.data<&Camera::SetFar, &Camera::GetFar>("Far");
}

void RegisterMeta::RegisterInputMappingMeta()
{
	using namespace MirageEngine::Core::Input;

	entt::meta_factory<CallbackInputInfo> callbackInputInfo{};

	callbackInputInfo.type(entt::type_id<CallbackInputInfo>().hash(), "CallbackInputInfo");

	callbackInputInfo.data<&CallbackInputInfo::Pressed>("Pressed");
	callbackInputInfo.data<&CallbackInputInfo::Held>("Held");
	callbackInputInfo.data<&CallbackInputInfo::Released>("Released");;

	entt::meta_factory<InputMapping> inputMapping{};

	inputMapping.type(entt::type_id<InputMapping>().hash(), "InputMapping");

	inputMapping.base<Component>();

	inputMapping.data<&InputMapping::m_Map>("Bindings");

	inputMapping.func<&InputMapping::Clear>("ClearAll");

	inputMapping.func<&InputMapping::Remove>("RemoveKey");
}

void RegisterMeta::RegisterMeshMeta()
{
	using namespace MirageEngine::Core::Resources;
	entt::meta_factory<Mesh> mesh{};

	mesh.type(entt::type_id<Mesh>().hash(), "Mesh");

	mesh.base<IResource>();

	mesh.data<nullptr, &Mesh::GetName>("Name");
}

void RegisterMeta::RegisterTextureMeta()
{
	using namespace MirageEngine::Core::Resources;
	entt::meta_factory<Texture> texture{};
	
	texture.type(entt::type_id<Texture>().hash(), "Texture");

	texture.base<IResource>();

	texture.data<nullptr, &Texture::GetName>("Name");
}

void MirageEngine::RegisterMeta::RegisterMaterialMeta()
{
	entt::meta_factory<Resources::Material> material{};
	material.type(entt::type_id<Resources::Material>().hash(), "Material");

	material.base<Resources::IResource>();

	material.data<nullptr, &Resources::Material::GetName>("Name");
}

void MirageEngine::RegisterMeta::RegisterShaderMeta()
{
	entt::meta_factory<Resources::Shader> material{};
	material.type(entt::type_id<Resources::Shader>().hash(), "Shader");

	material.base<Resources::IResource>();

	material.data<nullptr, &Resources::Shader::GetName>("Name");
}

void RegisterMeta::RegisterInfluenceGridMeta()
{
	entt::meta_factory<InfluenceGrid> influenceGrid{};
	influenceGrid.type(entt::type_id<InfluenceGrid>().hash(), "InfluenceGrid");

	influenceGrid.base<Component>();

	influenceGrid.data<&InfluenceGrid::SetWidth, &InfluenceGrid::GetWidth>("Width");
	influenceGrid.data<&InfluenceGrid::SetHeight, &InfluenceGrid::GetHeight>("Height");
	influenceGrid.data<&InfluenceGrid::SetCellSize, &InfluenceGrid::GetCellSize>("Cell Size");
	influenceGrid.data<&InfluenceGrid::SetDecayRate, &InfluenceGrid::GetDecayRate>("Decay Rate");
	influenceGrid.data<&InfluenceGrid::SetChannelsNames, &InfluenceGrid::GetChannelsNames>("Channels");
	influenceGrid.func<&InfluenceGrid::Bake>("Bake Grid");
}

void RegisterMeta::RegisterInfluenceEmitterMeta()
{	
	entt::meta_factory<InfluenceEmitter> influenceEmitter{};
	influenceEmitter.type(entt::type_id<InfluenceEmitter>().hash(), "InfluenceEmitter");

	influenceEmitter.base<Component>();
	influenceEmitter.data<&InfluenceEmitter::SetInfluenceType, &InfluenceEmitter::GetInfluenceType>("Type");
	influenceEmitter.data<&InfluenceEmitter::SetRadius, &InfluenceEmitter::GetRadius>("Radius");
	influenceEmitter.data<&InfluenceEmitter::SetStrength, &InfluenceEmitter::GetStrength>("Strength");
	influenceEmitter.data<&InfluenceEmitter::SetTargetGrid, &InfluenceEmitter::GetTargetGrid>("Grid");
}

void RegisterMeta::RegisterResponsiveCurveMeta()
{
	entt::meta_factory<AI::ResponsiveCurve> curve{};
	curve.type(entt::type_id<AI::ResponsiveCurve>().hash(), "ResponsiveCurve");

	entt::meta_factory<AI::ResponsiveCurve::CurveType> enumFactory{};
	enumFactory.type(entt::type_id<AI::ResponsiveCurve::CurveType>().hash(), "CurveType");
	enumFactory.data<AI::ResponsiveCurve::CurveType::Linear>("Linear");
	enumFactory.data<AI::ResponsiveCurve::CurveType::Exponential>("Exponential");
	enumFactory.data<AI::ResponsiveCurve::CurveType::Logistic>("Logistic");

	curve.data<&AI::ResponsiveCurve::SetSlope, &AI::ResponsiveCurve::GetSlope>("Slope");
	curve.data<&AI::ResponsiveCurve::SetExponent, &AI::ResponsiveCurve::GetExponent>("Exponent");
	curve.data<&AI::ResponsiveCurve::SetXOffset, &AI::ResponsiveCurve::GetXOffset>("X Offset");
	curve.data<&AI::ResponsiveCurve::SetYOffset, &AI::ResponsiveCurve::GetYOffset>("Y Offset");
	curve.data<&AI::ResponsiveCurve::SetInverted, &AI::ResponsiveCurve::GetInverted>("Inverted");
	curve.data<&AI::ResponsiveCurve::SetType, &AI::ResponsiveCurve::GetType>("Curve Type");
}

void RegisterMeta::RegisterNavAgentMeta()
{
	entt::meta_factory<AI::NavProfile> navProfile{};
	navProfile.type(entt::type_id<AI::NavProfile>().hash(), "NavProfile");
	navProfile.data<&AI::NavProfile::influenceWeights>("Weights");
	navProfile.data<&AI::NavProfile::canWalkDiagonal>("Can Walk Diagonal");

	entt::meta_factory<NavAgent> navAgent{};
	navAgent.type(entt::type_id<NavAgent>().hash(), "NavAgent");
	navAgent.base<Component>();

	navAgent.data<&NavAgent::SetMovementSpeed, &NavAgent::GetMovementSpeed>("Movement Speed");
	navAgent.data<&NavAgent::SetRotationSpeed, &NavAgent::GetRotationSpeed>("Rotation Speed");
	navAgent.data<&NavAgent::SetStoppingDistance, &NavAgent::GetStoppingDistance>("Stopping Distance");
	navAgent.data<&NavAgent::SetRecalculateInterval, &NavAgent::GetRecalculateInterval>("Recalculate Interval");

	navAgent.data<&NavAgent::SetProfile, &NavAgent::GetProfile>("Navigation Profile");
}

void RegisterMeta::RegisterUtilityAgentMeta()
{
	entt::meta_factory<UtilityAgent> utilityAgent{};
	utilityAgent.type(entt::type_id<UtilityAgent>().hash(), "UtilityAgent");
	utilityAgent.base<Component>();

	utilityAgent.data<&UtilityAgent::SetDecisionInterval, &UtilityAgent::GetDecisionInterval>("Decision Interval");
	utilityAgent.data<&UtilityAgent::SetInertia, &UtilityAgent::GetInertia>("Inertia");

	utilityAgent.data<nullptr, &UtilityAgent::GetLastDecisionReason>("Status");
}