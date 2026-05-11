#include <Components/rigidbody.h>
#include <Components/transform.h>
#include <Physics/physics_manager.h>
#include <Physics/common_enum.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogComponentRigidBody)

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core;

Rigidbody::Rigidbody(ICCollider* _shape, Physics::ERigidBodyType _type)
{
    m_Rigidbody = Physics::PhysicsManager::Instance().CreateRigidBody(_shape->GetCollision(), _type);

    m_Rigidbody->OnSimulated = [this]()
    {
        m_Rigidbody->UpdateTransform(GetOwner()->Transform);
    };
}

Rigidbody::~Rigidbody()
{
    delete m_Rigidbody;
    LOG(logTemp, LogComponentRigidBody, "Rigidbody destroyed");
}

void Rigidbody::OnTransformChanged(Components::Transform& _newTransform) { m_Rigidbody->OnTransformChanged(_newTransform); }

void Rigidbody::SetType(Physics::ERigidBodyType _type) { m_Rigidbody->SetType(_type); }

void Rigidbody::AddCollider(ICCollider* _newCollider) { m_Rigidbody->AddCollider(_newCollider->GetCollision()); }

void Rigidbody::RemoveCollider(ICCollider* _collider) { m_Rigidbody->RemoveCollider(_collider->GetCollision()); }

void Rigidbody::SetMass(float _mass) { m_Rigidbody->SetMass(_mass); }

void Rigidbody::SetCollisionGroup(Physics::ECollisionGroup _group) { m_Rigidbody->SetCollisionGroup(_group); }

void Rigidbody::SetCollisionMask(Physics::ECollisionGroup _mask) { m_Rigidbody->SetCollisionMask(_mask); }

void Rigidbody::SetCollisionResponse(Physics::ECollisionResponse response) { m_Rigidbody->SetCollisionResponse(response); }

Physics::ECollisionGroup Rigidbody::GetCollisionGroup() const { return m_Rigidbody->GetCollisionGroup(); }

Physics::ECollisionGroup Rigidbody::GetCollisionMask() const { return m_Rigidbody->GetCollisionMask(); }
