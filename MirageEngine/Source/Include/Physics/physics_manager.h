#pragma once
#include "common_enum.h"
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

            std::shared_ptr<IWorldPhysics> m_worldPhysics = nullptr;

        public:

            static PhysicsManager& Instance();

            void Shutdown();

            PhysicsManager() = default;
            ~PhysicsManager() = default;
            void Initialize(EPhysicsEngine _physicsEngine = EPhysicsEngine::Bullet3);
            void Update(float _deltaTime);

            std::weak_ptr<IWorldPhysics> GetWorldPhysics() const { return m_worldPhysics; }

            IRigidBody* CreateRigidBody(ICollider* _shape, ERigidBodyType _type);

            ICollider* CreateCollider(ECollisionShapeType _shapeType);

            void RegisterRigidBody(IRigidBody* _body, ECollisionGroup _group, ECollisionGroup _mask);

            void UnregisterRigidBody(IRigidBody* _body);
    };
} // namespace MirageEngine::Core::Physics
