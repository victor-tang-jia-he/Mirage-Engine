#pragma once

#include "bullet3_pch.h"
#include "../i_world_physics.h"

namespace MirageEngine::Core::Physics
{
	class IRigidBody;

	class Bullet3WorldPhysics : public IWorldPhysics
	{
		using Vector3 = MirageEngine::Core::Math::Vector3;

	public:

		btDefaultCollisionConfiguration* m_CollisionConfig;
		btCollisionDispatcher* m_Dispatcher;
		btBroadphaseInterface* m_Broadphase;
		btSequentialImpulseConstraintSolver* m_Solver;
		btDiscreteDynamicsWorld* m_World;

		Bullet3WorldPhysics()
		{
			m_CollisionConfig = new btDefaultCollisionConfiguration();
			m_Dispatcher = new btCollisionDispatcher(m_CollisionConfig);
			m_Broadphase = new btDbvtBroadphase();
			m_Solver = new btSequentialImpulseConstraintSolver();
			m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfig);
			m_World->setGravity(btVector3(0, -9.81f, 0)); // default gravity
		}

		~Bullet3WorldPhysics() override
		{
			delete m_World;
			delete m_Solver;
			delete m_Broadphase;
			delete m_Dispatcher;
			delete m_CollisionConfig;
		}

		void AddRigidBody(IRigidBody* _body, ECollisionGroup _group, ECollisionGroup _mask) override;

		void RemoveRigidBody(IRigidBody* _body) override;

		void Update(float _deltaTime) override { m_World->stepSimulation(_deltaTime); }

		void SetGravity(float _x, float _y, float _z) override { m_World->setGravity(btVector3(_x, _y, _z)); }

		void SetGravity(Vector3& _v) override { m_World->setGravity(btVector3(_v.X, _v.Y, _v.Z)); }

		Bullet3WorldPhysics& API_Bullet3() override { return *this; }
	};
} // namespace MirageEngine::Core::Physics
