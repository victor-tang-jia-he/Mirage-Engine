#include <Physics/physics_manager.h>


DEFINE_LOG_CATEGORY(LogPhysics);

using namespace MirageEngine::Core::Physics;

PhysicsManager* m_Instance = nullptr;

PhysicsManager* PhysicsManager::Instance()
{
    if (m_Instance == nullptr) { m_Instance = new PhysicsManager(); }
    return m_Instance;
}

void PhysicsManager::Initialize(EPhysicsEngine _physicsEngine)
{
    // set up physics engine
    m_physicsEngine = _physicsEngine;
    delete m_worldPhysics; // clean up previous physics engine if exists

    LOG(logInfo, LogPhysics, "Physics Engine initialized : ", m_physicsEngine == EPhysicsEngine::Bullet3 ? "Bullet physics" : "Undefined physics Engine");
    switch (m_physicsEngine)
    {
        case EPhysicsEngine::Bullet3: m_worldPhysics = new Bullet3WorldPhysics(); break;
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

//IRigidBody* PhysicsManager::CreateRigidBody(ICollider* _shape, ERigidBodyType _type)
//{
//    if (_shape == nullptr)
//    {
//        LOG(logWarning, LogPhysics, "Cannot create rigid body : shape is null");
//        return nullptr;
//    }
//    IRigidBody* body = nullptr;
//    switch (m_physicsEngine)
//    {
//        case EPhysicsEngine::Bullet3: body = new Bullet3RigidBody(); break;
//        default                     : break;
//    }
//    if (body != nullptr) { body->Create(_shape, _type); }
//    else
//    {
//        LOG(logWarning, LogPhysics, "Cannot create rigid body : physics engine not initialized");
//        return nullptr;
//    }
//    return body;
//}

//ICollider* PhysicsManager::CreateCollider(ECollisionShapeType _shapeType)
//{
//    ICollider* collider = nullptr;
//    switch (m_physicsEngine)
//    {
//        case EPhysicsEngine::Bullet3: collider = new Bullet3Collider(); break;
//        default                     : break;
//    }
//    if (collider != nullptr) { collider->Create(_shapeType); }
//    return collider;
//}

void PhysicsManager::RegisterRigidBody(IRigidBody* _body)
{
    if (m_worldPhysics == nullptr)
    {
        LOG(logWarning, LogPhysics, "Cannot register rigid body : physics engine not initialized");
        return;
    }
    m_worldPhysics->AddRigidBody(_body);
}

void PhysicsManager::UnregisterRigidBody(IRigidBody* _body)
{
    if (m_worldPhysics == nullptr)
    {
        LOG(logWarning, LogPhysics, "Cannot unregister rigid body : physics engine not initialized");
        return;
    }
    m_worldPhysics->RemoveRigidBody(_body);
}