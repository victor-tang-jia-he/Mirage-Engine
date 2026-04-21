#include <Components/sphere_collider.h>
#include <quaternion.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Physics;
using namespace MirageEngine::Core;

SphereCollider::SphereCollider() { m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::Sphere); }

SphereCollider::~SphereCollider() { m_Collider.reset(); }

void SphereCollider::SetRadius(float _radius) { m_Collider->As<ISphereCollider>()->SetRadius(_radius); }

float SphereCollider::GetRadius() const { return m_Collider->As<ISphereCollider>()->GetRadius(); }

void SphereCollider::SetScale(Math::Vector3 _scale) { m_Collider->SetScale(_scale); }

void SphereCollider::SetTranslate(Math::Vector3 _offset) { m_Collider->SetTranslate(_offset); }

void SphereCollider::SetRotation(Math::Quaternion _rotation) { m_Collider->SetRotation(_rotation); }

void SphereCollider::SetOffset(Math::Vector3 _offset, Math::Quaternion _rotation) { m_Collider->SetOffset(_offset, _rotation); }

Math::Vector3 SphereCollider::GetScale() const { return m_Collider->GetScale(); }

Math::Vector3 SphereCollider::GetTranslate() const { return m_Collider->GetTranslate(); }

Math::Quaternion SphereCollider::GetRotation() const { return m_Collider->GetRotation(); }
