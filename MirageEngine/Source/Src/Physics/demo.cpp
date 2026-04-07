#include <iostream>
#include <vector>
#include <Physics/physics_manager.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_collider.h>

static int demo()
{
    // Initialize Bullet3 physics world
    using namespace MirageEngine::Core::Physics;
    using namespace MirageEngine;
    PhysicsManager* pManager = PhysicsManager::Instance();

    pManager->Initialize(EPhysicsEngine::Bullet3);
    ICollider*  collider = pManager->CreateCollider(ECollisionShapeType::Box);
    IRigidBody* rb       = pManager->CreateRigidBody(collider);

    GameObject* obj = new GameObject(nullptr, "TestObject");
    obj->AddComponent(collider);
    obj->AddComponent(rb);

    for (int i = 0; i < 300; ++i)
    {
        pManager->Update(1.0f / 60.0f); // stepSimulation + sync

        rb->Update(); // sync rigid body with game object transform , GameObject missing update method for now

        std::cout << obj->Transform.GetPosition() << std::endl;
    }

    return 0;
}
