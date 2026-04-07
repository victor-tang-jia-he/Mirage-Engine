#pragma once
#include "common_enum.h"
#include <Logs/log_macro.h>

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
                    void Update(float deltaTime);

                    IRigidBody* CreateRigidBody(ICollider* shape);

                    ICollider* CreateCollider(ECollisionShapeType shapeType);

                    void RegisterRigidBody(IRigidBody* body);
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
