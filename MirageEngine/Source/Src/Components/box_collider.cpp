#include <Components/box_collider.h>
#include <quaternion.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Physics;
using namespace MirageEngine::Core;

BoxCollider::BoxCollider() { m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::Box); }

BoxCollider::~BoxCollider() { m_Collider.reset(); }

void BoxCollider::SetHalfExtents(const Math::Vector3& _halfExtents) { m_Collider->As<IBoxCollider>()->SetHalfExtents(_halfExtents); }

Math::Vector3 BoxCollider::GetHalfExtents() { return m_Collider->As<IBoxCollider>()->GetHalfExtents(); }

void BoxCollider::SetScale(Math::Vector3 _scale) { m_Collider->SetScale(_scale); }

void BoxCollider::SetTranslate(Math::Vector3 _offset) { m_Collider->SetTranslate(_offset); }

void BoxCollider::SetRotation(Math::Quaternion _rotation) { m_Collider->SetRotation(_rotation); }

void BoxCollider::SetOffset(Math::Vector3 _offset, Math::Quaternion _rotation) { m_Collider->SetOffset(_offset, _rotation); }

Math::Vector3 BoxCollider::GetScale() const { return m_Collider->GetScale(); }

Math::Vector3 BoxCollider::GetTranslate() const { return m_Collider->GetTranslate(); }

Math::Quaternion BoxCollider::GetRotation() const { return m_Collider->GetRotation(); }
