#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_world_physics.h>
#include <Physics/physics_manager.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogBullet3WorldPhysics)

using namespace MirageEngine::Core::Physics;
MirageEngine::Core::Physics::Bullet3WorldPhysics::Bullet3WorldPhysics()
{
	m_CollisionConfig = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfig);
	m_Broadphase = new btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver();
	m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfig);
	m_World->setGravity(btVector3(0, -9.81f, 0)); // default gravity
	LOG(logInfo, LogBullet3WorldPhysics, "Create New physics world");
}
MirageEngine::Core::Physics::Bullet3WorldPhysics::~Bullet3WorldPhysics()
{
	delete m_CollisionConfig;
	delete	m_Dispatcher;
	delete	m_Broadphase;
	delete	m_Solver;
	delete	m_World;
	LOG(logInfo, LogBullet3WorldPhysics, "Destroy physics world");
}
void Bullet3WorldPhysics::AddRigidBody(std::shared_ptr<IRigidBody> _body, ECollisionGroup _group, ECollisionGroup _mask)
{
	if (_body == nullptr) return;
	if (m_World == nullptr)
	{
		LOG(logError, LogBullet3WorldPhysics, "Cannot add rigid body to world : physics engine not initialized");
		return;
	}

	// Add the rigid body to the Bullet3 world
	_body->API_Bullet3().GetBullet3RigidBody()->forceActivationState(ACTIVE_TAG);
	m_World->addRigidBody(_body->API_Bullet3().GetBullet3RigidBody(), +_group, +_mask);
}

void Bullet3WorldPhysics::RemoveRigidBody(IRigidBody* _body)
{
	if (_body == nullptr) return;
	if (m_World == nullptr)
	{
		LOG(logError, LogBullet3WorldPhysics, "Cannot remove rigid body from world : physics engine not initialized");
		return;
	}
	// Remove the rigid body from the Bullet3 world
	m_World->removeRigidBody(_body->API_Bullet3().GetBullet3RigidBody());
}
