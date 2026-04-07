#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_collider.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Physics;

bool Bullet3RigidBody::Create(ICollider* shape)
{
    if (shape == nullptr)
    {
        LOG(logError, LogPhysics, "Cannot create rigid body : shape is null");
        return false;
    }
    btCollisionShape* collisionShape = shape->API_Bullet3().GetBullet3CollisionShape();
    if (collisionShape == nullptr)
    {
        LOG(logError, LogPhysics, "Cannot create rigid body : collision shape is null");
        return false;
    }
    btDefaultMotionState*                    motionState = new btDefaultMotionState();
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_Mass, motionState, collisionShape);

    btVector3 inertia(m_Velocity.X, m_Velocity.Y, m_Velocity.Z);
    inertia += rigidBodyCI.m_localInertia;

    shape->API_Bullet3().GetBullet3CollisionShape()->calculateLocalInertia(m_Mass, inertia);
    m_rigidBody = new btRigidBody(rigidBodyCI);
    PhysicsManager::Instance()->RegisterRigidBody(this);
    return true;
}

void MirageEngine::Core::Physics::Bullet3RigidBody::Update()
{
    if (m_rigidBody == nullptr) return;
    MirageEngine::GameObject* owner = GetOwner();
    if (owner == nullptr) return;
    btVector3 position = m_rigidBody->getWorldTransform().getOrigin();

    owner->Transform.SetPosition(MirageEngine::Core::Math::Vector3(position.getX(), position.getY(), position.getZ()));
    m_Velocity = MirageEngine::Core::Math::Vector3(m_rigidBody->getLinearVelocity().getX(), m_rigidBody->getLinearVelocity().getY(), m_rigidBody->getLinearVelocity().getZ());
}
