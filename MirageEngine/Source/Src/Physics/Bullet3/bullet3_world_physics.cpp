#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_world_physics.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Physics;

void Bullet3WorldPhysics::AddRigidBody(IRigidBody* body)
{
    if (body == nullptr) return;
    if (m_world == nullptr)
    {
        LOG(logError, LogPhysics, "Cannot add rigid body to world : physics engine not initialized");
        return;
    }

    // Add the rigid body to the Bullet3 world
    m_world->addRigidBody(body->API_Bullet3().GetBullet3RigidBody());
}

void MirageEngine::Core::Physics::Bullet3WorldPhysics::RemoveRigidBody(IRigidBody* body)
{
    if (body == nullptr) return;
    if (m_world == nullptr)
    {
        LOG(logError, LogPhysics, "Cannot remove rigid body from world : physics engine not initialized");
        return;
    }
    // Remove the rigid body from the Bullet3 world
    m_world->removeRigidBody(body->API_Bullet3().GetBullet3RigidBody());
}
