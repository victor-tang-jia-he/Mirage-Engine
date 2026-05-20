#include <quaternion.h>

#include <Components/capsule_collider.h>

#include <Physics/physics_manager.h>
#include <Physics/i_collider.h>

#include <GameObject/game_object.h>

#include <entt/entt.hpp>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Physics;
using namespace MirageEngine::Core;

CapsuleCollider::CapsuleCollider() { m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::CAPSULE); }

MirageEngine::Core::Components::CapsuleCollider::CapsuleCollider(GameObject* _owner)
    : Component(_owner)
{
    m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::CAPSULE);
}

void MirageEngine::Core::Components::CapsuleCollider::AttachMeta(std::shared_ptr<GameObject> _Owner)
{
    std::shared_ptr<CapsuleCollider> compPtr = std::make_shared<CapsuleCollider>(*this);
    _Owner->AddComponent<CapsuleCollider>(compPtr);
}

void MirageEngine::Core::Components::CapsuleCollider::AttachNew(std::shared_ptr<GameObject> _owner)
{
    std::shared_ptr<CapsuleCollider> compPtr = std::make_shared<CapsuleCollider>(_owner.get());
    _owner->AddComponent<CapsuleCollider>(compPtr);
}

entt::id_type MirageEngine::Core::Components::CapsuleCollider::GetType() const
{
    return entt::type_id<CapsuleCollider>().hash();
}

void CapsuleCollider::SetRadius(float _radius) { m_Collider->As<ICapsuleCollider>()->SetRadius(_radius); }

void CapsuleCollider::SetHeight(float _height) { m_Collider->As<ICapsuleCollider>()->SetHeight(_height); }

float CapsuleCollider::GetRadius() const { return m_Collider->As<ICapsuleCollider>()->GetRadius(); }

float CapsuleCollider::GetHeight() const { return m_Collider->As<ICapsuleCollider>()->GetHeight(); }

void CapsuleCollider::SetScale(Math::Vector3 _scale) { m_Collider->SetScale(_scale); }

void CapsuleCollider::SetTranslate(Math::Vector3 _offset) { m_Collider->SetTranslate(_offset); }

void CapsuleCollider::SetRotation(Math::Quaternion _rotation) { m_Collider->SetRotation(_rotation); }

void CapsuleCollider::SetOffset(Math::Vector3 _offset, Math::Quaternion _rotation) { m_Collider->SetOffset(_offset, _rotation); }

Math::Vector3 CapsuleCollider::GetScale() const { return m_Collider->GetScale(); }

Math::Vector3 CapsuleCollider::GetTranslate() const { return m_Collider->GetTranslate(); }

Math::Quaternion CapsuleCollider::GetRotation() const { return m_Collider->GetRotation(); }
