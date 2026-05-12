#include <Components/box_collider.h>
#include <quaternion.h>
#include <Physics/physics_manager.h>
#include <entt/entt.hpp>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Physics;
using namespace MirageEngine::Core;

BoxCollider::BoxCollider() 
{ 
    m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::Box);
}

MirageEngine::Core::Components::BoxCollider::BoxCollider(GameObject* _owner)
    : Component(_owner)
{
    m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::Box);
}

void MirageEngine::Core::Components::BoxCollider::AttachMeta(std::shared_ptr<GameObject> _Owner)
{
    std::shared_ptr<BoxCollider> compPtr = std::make_shared<BoxCollider>(*this);
    _Owner->AddComponent<BoxCollider>(compPtr);
}

void MirageEngine::Core::Components::BoxCollider::AttachNew(std::shared_ptr<GameObject> _owner)
{
    std::shared_ptr<BoxCollider> compPtr = std::make_shared<BoxCollider>(_owner.get());
    _owner->AddComponent<BoxCollider>(compPtr);
}

entt::id_type MirageEngine::Core::Components::BoxCollider::GetType() const
{
    return entt::type_id<BoxCollider>().hash();
}

void BoxCollider::SetHalfExtents(const Math::Vector3& _halfExtents) { m_Collider->As<IBoxCollider>()->SetHalfExtents(_halfExtents); }

Math::Vector3 BoxCollider::GetHalfExtents() { return m_Collider->As<IBoxCollider>()->GetHalfExtents(); }

void BoxCollider::SetScale(Math::Vector3 _scale) { m_Collider->SetScale(_scale); }

void BoxCollider::SetTranslate(Math::Vector3 _offset) { m_Collider->SetTranslate(_offset); }

void BoxCollider::SetRotation(Math::Quaternion _rotation) { m_Collider->SetRotation(_rotation); }

void BoxCollider::SetOffset(Math::Vector3 _offset, Math::Quaternion _rotation) { m_Collider->SetOffset(_offset, _rotation); }

Math::Vector3 BoxCollider::GetScale() const { return m_Collider->GetScale(); }

Math::Vector3 BoxCollider::GetTranslate() const { return m_Collider->GetTranslate(); }

Math::Quaternion BoxCollider::GetRotation() const { return m_Collider->GetRotation(); }
