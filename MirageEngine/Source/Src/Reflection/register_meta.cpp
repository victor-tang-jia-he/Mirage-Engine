#include <Reflection/register_meta.h>

// Components includes
#include <GameObject/game_object.h>
#include <Components/transform.h>
#include <Components/mesh_renderer.h>
#include <Components/camera.h>
#include <Components/box_collider.h>
#include <Components/sphere_collider.h>
#include <Components/capsule_collider.h>
#include <Components/rigidbody.h>

// Lights includes
#include <Light/directional_light.h>
#include <Light/point_light.h>
#include <Light/spot_light.h>

#include <Physics/common_enum.h>

#include <AI/ai_action.h>
#include <Components/influence_emitter.h>
#include <AI/influence_grid.h>
#include <Input/input_mapping.h>
#include <AI/responsive_curve.h>
#include <Components/nav_agent.h>
#include <Components/utility_agent.h>

// Ressources includes
#include <Resources/material.h>
#include <Resources/shader.h>
#include <Resources/mesh.h>
#include <Resources/texture.h>

// EnTT includes
#include <entt/entt.hpp>

#include <vector3.h>
#include <quaternion.h>

#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRegisterMeta)

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace MirageEngine::Core::Components;

using namespace entt::literals;		// for _hs (hash)

RegisterMeta::RegisterMeta() {}

RegisterMeta::~RegisterMeta() {}

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

	RegisterLightMeta();
	RegisterDirectionalLightMeta();
	RegisterPointLightMeta();
	RegisterSpotLightMeta();

	RegisterBoxColliderMeta();
	RegisterSphereColliderMeta();
	RegisterCapsuleColliderMeta();
	RegisterRigidbodyMeta();

	RegisterMeshMeta();
	RegisterTextureMeta();
	RegisterMaterialMeta();
	RegisterMaterialInstanceMeta();
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
	LOG(logTemp, LogRegisterMeta, "Registered GameObject Meta");
}

void RegisterMeta::RegisterTransformMeta()
{	
	entt::meta_factory<Components::Transform> transform{};

	transform.type(entt::type_id<Transform>().hash(), "Transform");

	transform.ctor<Math::Vector3, Math::Quaternion, Math::Vector3>();

	transform.data<&Components::Transform::SetPosition, &Components::Transform::GetPosition>("Position");
	transform.data<&Components::Transform::SetRotation, &Components::Transform::GetRotation>("Rotation");
	transform.data<&Components::Transform::SetScale, &Components::Transform::GetScale>("Scale");

	transform.func<&Transform::AttachMeta>("Attach"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered Transform Meta");
}

void RegisterMeta::RegisterMeshRendererMeta()
{
	entt::meta_factory<MeshRenderer> meshRenderer{};

	// IMPORTANT: We use entt::type_id<T>().hash() as the ID because components are stored as base "Component*" pointers. 
	// When we call comp->GetType(), it returns the native C++ hash. By using the same hash here, the Inspector can "resolve" the correct meta-data.
	meshRenderer.type(entt::type_id<MeshRenderer>().hash(), "MeshRenderer");
	meshRenderer.ctor<GameObject*>();

	meshRenderer.base<Component>();

	meshRenderer.data<&MeshRenderer::SetMesh, &MeshRenderer::GetMesh>("Mesh");
	meshRenderer.data<&MeshRenderer::SetMaterial, &MeshRenderer::GetMaterial>("Material");

	meshRenderer.func<&MeshRenderer::AttachMeta>("Attach"_hs);
	meshRenderer.func<&MeshRenderer::AttachNew>("AttachNew"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered MeshRenderer Meta");
}

void RegisterMeta::RegisterCameraMeta()
{
	entt::meta_factory<Camera> camera{};

	camera.type(entt::type_id<Camera>().hash(), "Camera");
	camera.ctor<GameObject*>();

	camera.base<Component>();

	camera.data<&Camera::SetFov, &Camera::GetFov>("FOV");
	camera.data<&Camera::SetNear, &Camera::GetNear>("Near");
	camera.data<&Camera::SetFar, &Camera::GetFar>("Far");
	camera.data<&Camera::SetHeight, &Camera::GetHeight>("Height");
	camera.data<&Camera::SetWidth, &Camera::GetWidth>("Width");
	camera.data<&Camera::SetPosition, &Camera::GetPosition>("Position");
	camera.data<&Camera::SetRotation, &Camera::GetRotation>("Rotation");

	camera.func<&Camera::AttachNew>("AttachNew"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered Camera Meta");
}

void RegisterMeta::RegisterInputMappingMeta()
{
	using namespace MirageEngine::Core::Input;

	entt::meta_factory<CallbackInputInfo> callbackInputInfo{};

	callbackInputInfo.type(entt::type_id<CallbackInputInfo>().hash(), "CallbackInputInfo");

	callbackInputInfo.data<&CallbackInputInfo::Pressed>("Pressed");
	callbackInputInfo.data<&CallbackInputInfo::Held>("Held");
	callbackInputInfo.data<&CallbackInputInfo::Released>("Released");;
	LOG(logTemp, LogRegisterMeta, "Registered CallbackInputInfo Meta");

	entt::meta_factory<InputMapping> inputMapping{};

	inputMapping.type(entt::type_id<InputMapping>().hash(), "InputMapping");
	inputMapping.ctor<GameObject*>();

	inputMapping.base<Component>();

	inputMapping.func<&InputMapping::Clear>("ClearAll");
	inputMapping.func<&InputMapping::Remove>("RemoveKey");

	inputMapping.func<&InputMapping::AttachMeta>("Attach"_hs);
	inputMapping.func<&InputMapping::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterLightMeta()
{
	entt::meta_factory<Light> light{};

	light.type(entt::type_id<Light>().hash(), "Light");

	light.data<&Light::SetColor, &Light::GetColor>("Color");
	light.data<&Light::SetIntensity, &Light::GetIntensity>("Intensity");

	LOG(logTemp, LogRegisterMeta, "Registered Light Meta");
}

void MirageEngine::RegisterMeta::RegisterDirectionalLightMeta()
{
	entt::meta_factory<DirectionalLight> directionalLight{};

	directionalLight.type(entt::type_id<DirectionalLight>().hash(), "DirectionalLight");
	directionalLight.ctor<GameObject*>();

	directionalLight.base<Component>();
	directionalLight.base<Light>();

	directionalLight.data<&DirectionalLight::SetColor, &DirectionalLight::GetColor>("Color");
	directionalLight.data<&DirectionalLight::SetIntensity, &DirectionalLight::GetIntensity>("Intensity");
	directionalLight.data<&DirectionalLight::SetDirection, &DirectionalLight::GetDirection>("Direction");

	directionalLight.func<&DirectionalLight::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterPointLightMeta()
{
	entt::meta_factory<PointLight> pointLight{};

	pointLight.type(entt::type_id<PointLight>().hash(), "PointLight");
	pointLight.ctor<GameObject*>();

	pointLight.base<Component>();
	pointLight.base<Light>();

	pointLight.data<&PointLight::SetColor, &PointLight::GetColor>("Color");
	pointLight.data<&PointLight::SetIntensity, &PointLight::GetIntensity>("Intensity");
	pointLight.data<&PointLight::SetPosition, &PointLight::GetPosition>("Position");

	pointLight.func<&PointLight::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterSpotLightMeta()
{
	entt::meta_factory<SpotLight> spotLight{};

	spotLight.type(entt::type_id<SpotLight>().hash(), "SpotLight");
	spotLight.ctor<GameObject*>();

	spotLight.base<Component>();
	spotLight.base<Light>();

	spotLight.data<&SpotLight::SetColor, &SpotLight::GetColor>("Color");
	spotLight.data<&SpotLight::SetIntensity, &SpotLight::GetIntensity>("Intensity");
	spotLight.data<&SpotLight::SetPosition, &SpotLight::GetPosition>("Position");
	spotLight.data<&SpotLight::SetDirection, &SpotLight::GetDirection>("Direction");
	spotLight.data<&SpotLight::SetInnerCutoff, &SpotLight::GetInnerCutoff>("InnerCutoff");
	spotLight.data<&SpotLight::SetOuterCutoff, &SpotLight::GetOuterCutoff>("OuterCutoff");

	spotLight.func<&SpotLight::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterBoxColliderMeta()
{
	entt::meta_factory<BoxCollider> boxCollider{};

	boxCollider.type(entt::type_id<BoxCollider>().hash(), "BoxCollider");
	boxCollider.ctor<GameObject*>();

	boxCollider.base<Component>();

	boxCollider.data<&BoxCollider::SetHalfExtents, &BoxCollider::GetHalfExtents>("HalfExtents");
	boxCollider.data<&BoxCollider::SetTranslate, &BoxCollider::GetTranslate>("Position");
	boxCollider.data<&BoxCollider::SetRotation, &BoxCollider::GetRotation>("Rotation");
	boxCollider.data<&BoxCollider::SetScale, &BoxCollider::GetScale>("Scale");

	boxCollider.func<&BoxCollider::AttachMeta>("Attach"_hs);
	boxCollider.func<&BoxCollider::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterSphereColliderMeta()
{
	entt::meta_factory<SphereCollider> sphereCollided{};

	sphereCollided.type(entt::type_id<SphereCollider>().hash(), "SphereCollider");
	sphereCollided.ctor<GameObject*>();

	sphereCollided.base<Component>();

	sphereCollided.data<&SphereCollider::SetRadius, &SphereCollider::GetRadius>("Radius");
	sphereCollided.data<&SphereCollider::SetTranslate, &SphereCollider::GetTranslate>("Position");
	sphereCollided.data<&SphereCollider::SetRotation, &SphereCollider::GetRotation>("Rotation");
	sphereCollided.data<&SphereCollider::SetScale, &SphereCollider::GetScale>("Scale");

	sphereCollided.func<&SphereCollider::AttachMeta>("Attach"_hs);
	sphereCollided.func<&SphereCollider::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterCapsuleColliderMeta()
{
	entt::meta_factory<CapsuleCollider> capsuleCollider{};

	capsuleCollider.type(entt::type_id<CapsuleCollider>().hash(), "CapsuleCollider");
	capsuleCollider.ctor<GameObject*>();

	capsuleCollider.base<Component>();

	capsuleCollider.data<&CapsuleCollider::SetHeight, &CapsuleCollider::GetHeight>("Height");
	capsuleCollider.data<&CapsuleCollider::SetRadius, &CapsuleCollider::GetRadius>("Radius");
	capsuleCollider.data<&CapsuleCollider::SetTranslate, &CapsuleCollider::GetTranslate>("Position");
	capsuleCollider.data<&CapsuleCollider::SetRotation, &CapsuleCollider::GetRotation>("Rotation");
	capsuleCollider.data<&CapsuleCollider::SetScale, &CapsuleCollider::GetScale>("Scale");

	capsuleCollider.func<&CapsuleCollider::AttachMeta>("Attach"_hs);
	capsuleCollider.func<&CapsuleCollider::AttachNew>("AttachNew"_hs);
}

void MirageEngine::RegisterMeta::RegisterRigidbodyMeta()
{
	entt::meta_factory<Rigidbody> rigidBody{};

	rigidBody.type(entt::type_id<Rigidbody>().hash(), "Rigidbody");
	rigidBody.ctor<GameObject*>();

	rigidBody.base<Component>();

	rigidBody.data<&Rigidbody::SetRigidBodyType, &Rigidbody::GetRigidBodyType>("Type");
	rigidBody.data<&Rigidbody::SetMass, &Rigidbody::GetMass>("Mass");
	rigidBody.data<&Rigidbody::SetCollisionGroup, &Rigidbody::GetCollisionGroup>("Collision Group");
	rigidBody.data<&Rigidbody::SetCollisionMask, &Rigidbody::GetCollisionMask>("Collision Mask");
	rigidBody.data<&Rigidbody::SetCollisionResponse, &Rigidbody::GetCollisionResponse>("Collision Response");

	rigidBody.func<&Rigidbody::AttachMeta>("Attach"_hs);
	rigidBody.func<&Rigidbody::AttachNew>("AttachNew"_hs);

	// Enums
	entt::meta_factory<Physics::ERigidBodyType>{}
	.type("ERigidBodyType"_hs, "ERigidBodyType")
		.data<Physics::ERigidBodyType::Static>("Static")
		.data<Physics::ERigidBodyType::Dynamic>("Dynamic")
		.data<Physics::ERigidBodyType::Kinematic>("Kinematic");

	entt::meta_factory<Physics::ECollisionGroup>{}
	.type("ECollisionGroup"_hs, "ECollisionGroup")
		.data<Physics::ECollisionGroup::All>("All")
		.data<Physics::ECollisionGroup::Default>("Default")
		.data<Physics::ECollisionGroup::Group1>("Group1")
		.data<Physics::ECollisionGroup::Group2>("Group2")
		.data<Physics::ECollisionGroup::Group3>("Group3")
		.data<Physics::ECollisionGroup::Group4>("Group4")
		.data<Physics::ECollisionGroup::Group5>("Group5")
		.data<Physics::ECollisionGroup::Group6>("Group6")
		.data<Physics::ECollisionGroup::Group7>("Group7")
		.data<Physics::ECollisionGroup::None>("None");

	entt::meta_factory<Physics::ECollisionResponse>{}
	.type("ECollisionResponse"_hs, "ECollisionResponse")
		.data<Physics::ECollisionResponse::Block>("Block")
		.data<Physics::ECollisionResponse::Ignore>("Ignore")
		.data<Physics::ECollisionResponse::Overlap>("Overlap");
	LOG(logTemp, LogRegisterMeta, "Registered InputMapping Meta");
}

void RegisterMeta::RegisterMeshMeta()
{
	using namespace MirageEngine::Core::Resources;
	entt::meta_factory<Mesh> mesh{};

	mesh.type("Mesh"_hs, "Mesh");

	mesh.base<IResource>();

	mesh.data<nullptr, &Mesh::GetName>("Name");

	mesh.func<&Mesh::GetFromManager>("Get"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered Mesh Meta");
}

void RegisterMeta::RegisterTextureMeta()
{
	using namespace MirageEngine::Core::Resources;
	entt::meta_factory<Texture> texture{};
	
	texture.type("Texture"_hs, "Texture");

	texture.base<IResource>();

	texture.data<nullptr, &Texture::GetName>("Name");

	texture.func<&Texture::GetFromManager>("Get"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered Texture Meta");
}

void MirageEngine::RegisterMeta::RegisterMaterialMeta()
{
	entt::meta_factory<Resources::Material> material{};
	material.type("Material"_hs, "Material");

	material.base<Resources::IResource>();

	material.data<nullptr, &Resources::Material::GetName>("Name");

	material.func<&Resources::Material::GetFromManager>("Get"_hs);
}

void MirageEngine::RegisterMeta::RegisterMaterialInstanceMeta()
{
	entt::meta_factory<Resources::MaterialInstance> materialInstance{};
	materialInstance.type("MaterialInstance"_hs, "MaterialInstance");

	materialInstance.base<Resources::IResource>();

	materialInstance.data<nullptr, &Resources::MaterialInstance::GetName>("Name");

	materialInstance.func<&Resources::MaterialInstance::GetFromManager>("Get"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered Material Meta");
}

void MirageEngine::RegisterMeta::RegisterShaderMeta()
{
	entt::meta_factory<Resources::Shader> material{};
	material.type("Shader"_hs, "Shader");

	material.base<Resources::IResource>();

	material.data<nullptr, &Resources::Shader::GetName>("Name");
	LOG(logTemp, LogRegisterMeta, "Registered Shader Meta");
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
	LOG(logTemp, LogRegisterMeta, "Registered InfluenceGrid Meta");
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

	influenceEmitter.func<&InfluenceEmitter::AttachMeta>("Attach"_hs);
	influenceEmitter.func<&InfluenceEmitter::AttachNew>("AttachNew"_hs);
	LOG(logTemp, LogRegisterMeta, "Registered InfluenceEmitter Meta");
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
    utilityAgent.data<&UtilityAgent::SetTargetGrid, &UtilityAgent::GetTargetGrid>("Grid");
	utilityAgent.data<nullptr, &UtilityAgent::GetLastDecisionReason>("Status");
    utilityAgent.data<&UtilityAgent::SetActions, &UtilityAgent::GetActionsRef>("Actions List");
}