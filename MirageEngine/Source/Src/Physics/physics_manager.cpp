#include <Physics/physics_manager.h>

DEFINE_LOG_CATEGORY(LogPhysics);

using namespace MirageEngine::Core::Physics;

PhysicsManager* m_Instance = nullptr;

PhysicsManager& PhysicsManager::Instance()
{
    if (m_Instance == nullptr) { m_Instance = new PhysicsManager(); }
    return *m_Instance;
}

void PhysicsManager::Shutdown()
{
    m_WorldRigidbodys.clear();
    delete m_Instance;
    m_Instance = nullptr;
}

void PhysicsManager::Initialize(EPhysicsEngine _physicsEngine)
{
    // set up physics engine
    m_physicsEngine = _physicsEngine;
    m_worldPhysics.reset(); // clean up previous physics engine if exists

    LOG(logInfo, LogPhysics, "Physics Engine initialized : ", m_physicsEngine == EPhysicsEngine::Bullet3 ? "Bullet physics" : "Undefined physics Engine");
    switch (m_physicsEngine)
    {
        case EPhysicsEngine::Bullet3: m_worldPhysics = std::make_shared<Bullet3WorldPhysics>(); break;
        default                     : break;
    }
}

void PhysicsManager::Update(float _deltaTime)
{
    if (m_worldPhysics == nullptr)
    {
        LOG(logWarning, LogPhysics, "Cannot Update : physics engine not initialized");
        return;
    }

    m_worldPhysics->Update(_deltaTime);
}

IRigidBody* PhysicsManager::CreateRigidBody(ICollider* _shape, ERigidBodyType _type)
{
    if (_shape == nullptr)
    {
        LOG(logWarning, LogPhysics, "Cannot create rigid body : shape is null");
        return nullptr;
    }
    IRigidBody* body = nullptr;
    switch (m_physicsEngine)
    {
        case EPhysicsEngine::Bullet3: body = new Bullet3RigidBody(); break;
        default                     : break;
    }
    if (body != nullptr) { body->Create(_shape, _type); }
    else
    {
        LOG(logWarning, LogPhysics, "Cannot create rigid body : physics engine not initialized");
        return nullptr;
    }
    return body;
}

ICollider* PhysicsManager::CreateCollider(ECollisionShapeType _shapeType)
{
    ICollider* collider = nullptr;
    switch (m_physicsEngine)
    {
        case EPhysicsEngine::Bullet3:
            switch (_shapeType)
            {
                case ECollisionShapeType::Box    : collider = new Bullet3BoxCollider(); break;
                case ECollisionShapeType::Sphere : collider = new Bullet3SphereCollider(); break;
                case ECollisionShapeType::Capsule: collider = new Bullet3CapsuleCollider(); break;
                default                          : break;
            }
            break;
        default: break;
    }
    if (collider != nullptr) { collider->Create(); }
    return collider;
}

void PhysicsManager::RegisterRigidBody(IRigidBody* _body, ECollisionGroup _group, ECollisionGroup _mask)
{
    if (m_worldPhysics == nullptr)
    {
        LOG(logWarning, LogPhysics, "Cannot register rigid body : physics engine not initialized");
        return;
    }
    m_worldPhysics->AddRigidBody(_body, _group, _mask);
    m_WorldRigidbodys.push_back(_body);
}

void PhysicsManager::UnregisterRigidBody(IRigidBody* _body)
{
    if (m_worldPhysics == nullptr)
    {
        LOG(logWarning, LogPhysics, "Cannot unregister rigid body : physics engine not initialized");
        return;
    }
    m_worldPhysics->RemoveRigidBody(_body);
    m_WorldRigidbodys.erase(std::remove(m_WorldRigidbodys.begin(), m_WorldRigidbodys.end(), _body), m_WorldRigidbodys.end());
}
