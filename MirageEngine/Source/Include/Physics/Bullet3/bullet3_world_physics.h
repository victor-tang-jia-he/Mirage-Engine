#pragma once

#include "bullet3_pch.h"
#include "../i_world_physics.h"

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class IRigidBody;

            class Bullet3WorldPhysics : public IWorldPhysics
            {
                    using Vector3 = MirageEngine::Core::Math::Vector3;

                public:

                    btDefaultCollisionConfiguration*     m_collisionConfig;
                    btCollisionDispatcher*               m_dispatcher;
                    btBroadphaseInterface*               m_broadphase;
                    btSequentialImpulseConstraintSolver* m_solver;
                    btDiscreteDynamicsWorld*             m_world;

                    Bullet3WorldPhysics()
                    {
                        m_collisionConfig = new btDefaultCollisionConfiguration();
                        m_dispatcher      = new btCollisionDispatcher(m_collisionConfig);
                        m_broadphase      = new btDbvtBroadphase();
                        m_solver          = new btSequentialImpulseConstraintSolver();
                        m_world           = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
                        m_world->setGravity(btVector3(0, -9.81f, 0)); // default gravity
                    }

                    ~Bullet3WorldPhysics() override
                    {
                        delete m_world;
                        delete m_solver;
                        delete m_broadphase;
                        delete m_dispatcher;
                        delete m_collisionConfig;
                    }

                    void AddRigidBody(IRigidBody* body) override;

                    void RemoveRigidBody(IRigidBody* body) override;

                    void Update(float deltaTime) override { m_world->stepSimulation(deltaTime); }

                    void SetGravity(float x, float y, float z) override { m_world->setGravity(btVector3(x, y, z)); }

                    void SetGravity(Vector3& v) override { m_world->setGravity(btVector3(v.X, v.Y, v.Z)); }

                    Bullet3WorldPhysics& API_Bullet3() override { return *this; }
            };

        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
