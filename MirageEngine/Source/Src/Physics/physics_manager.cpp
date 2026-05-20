#include <Physics/physics_manager.h>

// Bullet 3
#include <Physics/Bullet3/bullet3_physics_factory.h>

// Global includes
#include <Physics/i_rigidbody.h>
#include <Physics/i_collider.h>
#include <Physics/i_world_physics.h>

#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogPhysics);

using namespace MirageEngine::Core::Physics;

PhysicsManager* m_Instance = nullptr;

PhysicsManager& PhysicsManager::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new PhysicsManager();
	}
	return *m_Instance;
}

void PhysicsManager::Shutdown()
{
	if (m_WorldRigidbodys.size() > 0)
	{
		LOG(logWarning, LogPhysics, "Initializing physics manager while there are still rigid bodies registered. All rigid bodies will be cleared.");
		for (int i = 0; i < m_WorldRigidbodys.size(); i++)
		{
			LOG(logTemp, LogPhysics, " - Unregistering rigid body ", i + 1, " : ", int(m_WorldRigidbodys[i]->GetType()));
			UnregisterRigidBody(m_WorldRigidbodys[i]);
		}
	}
	m_WorldRigidbodys.clear();
	m_WorldPhysics.reset();
	delete m_Instance;
	m_Instance = nullptr;
}

void PhysicsManager::Initialize(EPhysicsEngine _physicsEngine)
{
	// set up physics engine
	m_PhysicsEngine = _physicsEngine;
	if (m_WorldRigidbodys.size() > 0)
	{
		LOG(logWarning, LogPhysics, "Initializing physics manager while there are still rigid bodies registered. All rigid bodies will be cleared.");
		for (int i = 0; i < m_WorldRigidbodys.size(); i++)
		{
			LOG(logTemp, LogPhysics, " - Unregistering rigid body ", i + 1, " : ", int(m_WorldRigidbodys[i]->GetType()));
			UnregisterRigidBody(m_WorldRigidbodys[i]);
		}
	}
	m_WorldRigidbodys.clear();
	m_WorldPhysics.reset(); // clean up previous physics engine if exists
	LOG(logInfo, LogPhysics, "Physics Engine initialized : ", m_PhysicsEngine == EPhysicsEngine::BULLET_3 ? "Bullet physics" : "Undefined physics Engine");
	switch (m_PhysicsEngine)
	{
	case EPhysicsEngine::BULLET_3: m_PhysicsFactory = std::make_shared<Bullet3PhysicsFactory>(); break;
	default: break;
	}

	if (m_PhysicsFactory)
	{
		m_WorldPhysics = m_PhysicsFactory->CreateWorld();
	}
	else
	{
		LOG(logError, LogPhysics, "Failed to initialize physics engine : no valid factory found");
	}
}

void PhysicsManager::Update(float _deltaTime)
{
	if (!m_WorldPhysics)
	{
		LOG(logFatal, LogPhysics, "Physics world is NULL (not initialized)");
		return;
	}
	if (_deltaTime <= 0.0f)
	{
		LOG(logWarning, LogPhysics, "DeltaTime is 0 or negative -> simulation may not run");
	}

	if (_deltaTime > 0.1f)
	{
		LOG(logWarning, LogPhysics, "DeltaTime too high -> unstable physics possible");
	}
	if (m_WorldRigidbodys.empty())
	{
		LOG(logWarning, LogPhysics, "No rigidbodies registered in world");
	}

	m_WorldPhysics->Update(_deltaTime);

	for (IRigidBody* rb : m_WorldRigidbodys)
	{
		if (!rb)
		{
			LOG(logError, LogPhysics, "Null rigidbody detected in world list");
			continue;
		}

		if (rb->OnSimulated)
		{
			rb->OnSimulated();
		}
	}
}
void PhysicsManager::DebugDraw()
{
	m_WorldPhysics->DebugDraw();
}
std::shared_ptr<IRigidBody> PhysicsManager::CreateRigidBody(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, MirageEngine::Core::Components::Transform _newTransform)
{
	if (_shape.get() == nullptr)
	{
		LOG(logWarning, LogPhysics, "Cannot create rigid body : shape is null");
		return nullptr;
	}
	std::shared_ptr<Physics::IRigidBody> body = m_PhysicsFactory->CreateRigidBody();
	if (body != nullptr) { body->Create(_shape, _type, _newTransform); }
	else
	{
		LOG(logWarning, LogPhysics, "Cannot create rigid body : physics engine not initialized");
		return nullptr;
	}
	return body;
}

std::shared_ptr<ICollider> PhysicsManager::CreateCollider(ECollisionShapeType _shapeType)
{
	std::shared_ptr<Physics::ICollider> collider = m_PhysicsFactory->CreateCollider(_shapeType);
	if (collider.get() != nullptr) 
	{ 
		collider->Create(); 
	}

	return collider;
}

void PhysicsManager::RegisterRigidBody(std::shared_ptr<IRigidBody> _body, ECollisionGroup _group, ECollisionGroup _mask)
{
	if (m_WorldPhysics.get() == nullptr)
	{
		LOG(logWarning, LogPhysics, "Cannot register rigid body : physics engine not initialized");
		return;
	}
	m_WorldPhysics->AddRigidBody(_body, _group, _mask);
	m_WorldRigidbodys.push_back(_body.get());
}

void PhysicsManager::UnregisterRigidBody(IRigidBody* _body)
{
	if (m_WorldPhysics.get() == nullptr)
	{
		LOG(logWarning, LogPhysics, "Cannot unregister rigid body : physics engine not initialized");
		return;
	}
	m_WorldPhysics->RemoveRigidBody(_body);
	m_WorldRigidbodys.erase(std::remove(m_WorldRigidbodys.begin(), m_WorldRigidbodys.end(), _body), m_WorldRigidbodys.end());
}
