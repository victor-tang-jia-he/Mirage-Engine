#pragma once

#include <Physics/Bullet3/bullet3_pch.h>
#include <Physics/i_world_physics.h>

DECLARE_LOG_CATEGORY_EXTERN(LogBullet3WorldPhysics)

namespace MirageEngine::Core::Physics
{
    class IRigidBody;

    class Bullet3WorldPhysics : public IWorldPhysics
    {
        using Vector3 = MirageEngine::Core::Math::Vector3;

    private:
        btDefaultCollisionConfiguration* m_CollisionConfig;
        btCollisionDispatcher* m_Dispatcher;
        btBroadphaseInterface* m_Broadphase;
        btSequentialImpulseConstraintSolver* m_Solver;
        btDiscreteDynamicsWorld* m_World;

    public:
	    Bullet3WorldPhysics();
	    ~Bullet3WorldPhysics() override;

	    void AddRigidBody(std::shared_ptr<IRigidBody> _body, ECollisionGroup _group, ECollisionGroup _mask) override;
        void RemoveRigidBody(IRigidBody* _body) override;
        void Update(float _deltaTime) override { m_World->stepSimulation(_deltaTime); }

        btDiscreteDynamicsWorld* GetWorld() const { return m_World; }

        void SetGravity(float _x, float _y, float _z) override { m_World->setGravity(btVector3(_x, _y, _z)); }
        void SetGravity(Vector3& _v) override { m_World->setGravity(btVector3(_v.X, _v.Y, _v.Z)); }

        Bullet3WorldPhysics& API_Bullet3() override { return *this; }
    };
} // namespace MirageEngine::Core::Physics
