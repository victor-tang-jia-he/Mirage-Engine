#pragma once
#include "common_enum.h"
#include <Logs/log_macro.h>
#include <Physics/i_world_physics.h>
#include <Physics/Bullet3/bullet3_world_physics.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_collider.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPhysics);

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class IWorldPhysics;
            class ICollider;
            class IRigidBody;
            class PhysicsManager
            {
                private:
                    EPhysicsEngine m_physicsEngine = EPhysicsEngine::Bullet3;

                    IWorldPhysics* m_worldPhysics = nullptr;

                public:

                    static PhysicsManager* Instance();

                    PhysicsManager()  = default;
                    ~PhysicsManager() = default;
                    void Initialize(EPhysicsEngine _physicsEngine);
                    void Update(float _deltaTime);

                    IRigidBody* CreateRigidBody(ICollider* _shape, ERigidBodyType _type);

                    ICollider* CreateCollider(ECollisionShapeType _shapeType);

                    void RegisterRigidBody(IRigidBody* _body);

                    void UnregisterRigidBody(IRigidBody* _body);
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
