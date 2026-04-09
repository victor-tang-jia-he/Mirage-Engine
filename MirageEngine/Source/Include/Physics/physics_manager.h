#pragma once
#include "common_enum.h"
#include <Logs/log_macro.h>
#include <Physics/i_world_physics.h>
#include <Physics/Bullet3/bullet3_world_physics.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_capsule_collider.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPhysics);

namespace MirageEngine::Core::Physics
{
    class IWorldPhysics;
    class ICollider;
    class IRigidBody;

    class PhysicsManager
    {
        private:

            EPhysicsEngine m_physicsEngine = EPhysicsEngine::Bullet3;

            std::vector<IRigidBody*> m_WorldRigidbodys;

        public:

            IWorldPhysics* m_worldPhysics = nullptr;

            static PhysicsManager* Instance();

            PhysicsManager()  = default;
            ~PhysicsManager() = default;
            void Initialize(EPhysicsEngine _physicsEngine = EPhysicsEngine::Bullet3);
            void Update(float _deltaTime);

            IRigidBody* CreateRigidBody(ICollider* _shape, ERigidBodyType _type);

            ICollider* CreateCollider(ECollisionShapeType _shapeType);

            void RegisterRigidBody(IRigidBody* _body);

            void UnregisterRigidBody(IRigidBody* _body);
    };
} // namespace MirageEngine::Core::Physics
