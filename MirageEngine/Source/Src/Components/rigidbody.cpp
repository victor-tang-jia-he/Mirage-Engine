#include <Components/rigidbody.h>
#include <Components/transform.h>
#include <Physics/physics_manager.h>
#include <Physics/common_enum.h>
#include <Components/box_collider.h>
#include <Components/sphere_collider.h>
#include <Components/capsule_collider.h>
#include <entt/entt.hpp>s
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogComponentRigidBody)

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core;

Rigidbody::Rigidbody(ICCollider* _shape, Physics::ERigidBodyType _type) 
{
    m_Rigidbody = Physics::PhysicsManager::Instance().CreateRigidBody(_shape->GetCollision(), _type);
    m_Rigidbody->OnSimulated = [this]()
        {
            m_Rigidbody->UpdateTransform(m_Owner->Transform);
        };
}

MirageEngine::Core::Components::Rigidbody::Rigidbody(GameObject* _owner)
    : Component(_owner)
{
    std::shared_ptr<ICCollider> shape = m_Owner->GetComponent<BoxCollider>();
    if (!shape)
    {
        shape = m_Owner->GetComponent<SphereCollider>();
        if (!shape)
        {
            shape = m_Owner->GetComponent<CapsuleCollider>();
            if (!shape)
            {
                shape = m_Owner->AddComponent<BoxCollider>();
            }
        }
    }
    m_Rigidbody = Physics::PhysicsManager::Instance().CreateRigidBody(shape->GetCollision(), Physics::ERigidBodyType::DYNAMIC, m_Owner->Transform);

    m_Rigidbody->OnSimulated = [this]()
        {
            m_Rigidbody->UpdateTransform(m_Owner->Transform);
        };
}

MirageEngine::Core::Components::Rigidbody::~Rigidbody()
{
    LOG(logTemp, LogComponentRigidBody, "Rigidbody destroyed : ", m_Owner->GetName()); 
}

entt::id_type MirageEngine::Core::Components::Rigidbody::GetType() const
{
    return entt::type_id<Rigidbody>().hash();
}

void MirageEngine::Core::Components::Rigidbody::AttachMeta(std::shared_ptr<GameObject> _Owner)
{
    std::shared_ptr<Rigidbody> compPtr = std::make_shared<Rigidbody>(*this);
    _Owner->AddComponent<Rigidbody>(compPtr);
}

void MirageEngine::Core::Components::Rigidbody::AttachNew(std::shared_ptr<GameObject> _owner)
{
    std::shared_ptr<Rigidbody> compPtr = std::make_shared<Rigidbody>(_owner.get());
    _owner->AddComponent<Rigidbody>(compPtr);
}

void Rigidbody::OnTransformChanged(Components::Transform& _newTransform) { m_Rigidbody->OnTransformChanged(_newTransform); }

void Rigidbody::SetRigidBodyType(Physics::ERigidBodyType _type) { m_Rigidbody->SetType(_type); }

void Rigidbody::AddCollider(ICCollider* _newCollider) { m_Rigidbody->AddCollider(_newCollider->GetCollision()); }

void Rigidbody::RemoveCollider(ICCollider* _collider) { m_Rigidbody->RemoveCollider(_collider->GetCollision().get()); }

void Rigidbody::SetMass(float _mass) { m_Rigidbody->SetMass(_mass); }

void Rigidbody::SetCollisionGroup(Physics::ECollisionGroup _group) { m_Rigidbody->SetCollisionGroup(_group); }

void Rigidbody::SetCollisionMask(Physics::ECollisionGroup _mask) { m_Rigidbody->SetCollisionMask(_mask); }

void Rigidbody::SetCollisionResponse(Physics::ECollisionResponse response) { m_Rigidbody->SetCollisionResponse(response); }

Physics::ERigidBodyType MirageEngine::Core::Components::Rigidbody::GetRigidBodyType() const { return m_Rigidbody->GetType(); }

float MirageEngine::Core::Components::Rigidbody::GetMass() const { return m_Rigidbody->GetMass(); }

Physics::ECollisionGroup Rigidbody::GetCollisionGroup() const { return m_Rigidbody->GetCollisionGroup(); }

Physics::ECollisionGroup Rigidbody::GetCollisionMask() const { return m_Rigidbody->GetCollisionMask(); }

Physics::ECollisionResponse MirageEngine::Core::Components::Rigidbody::GetCollisionResponse() const { return m_Rigidbody->GetCollisionResponse(); }
