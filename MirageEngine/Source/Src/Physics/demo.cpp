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
    IRigidBody* rb       = pManager->CreateRigidBody(collider, ERigidBodyType::Dynamic);

    GameObject* obj = new GameObject(nullptr, "TestObject");
    obj->AddComponent(collider);
    obj->AddComponent(rb);

    ICollider*  colliderPlane = pManager->CreateCollider(ECollisionShapeType::Box);
    IRigidBody* rbPlane       = pManager->CreateRigidBody(collider, ERigidBodyType::Dynamic);

    GameObject* objPlane = new GameObject(nullptr, "plane"); // fake
    objPlane->AddComponent(colliderPlane);
    objPlane->AddComponent(rbPlane);
    objPlane->SetPosition(MirageEngine::Core::Math::Vector3(0, -5.f, 0));
    rbPlane->OnTransformChanged(&objPlane->Transform); // LIN TU EST RACISTE JE VAIS TE FUMEE AVEC TON GETCOMPONENT QUI NE MARCHE PAS
    for (int i = 0; i < 120; ++i)
    {
        pManager->Update(1.0f / 60.0f); // stepSimulation + sync

        rb->Update(); // sync rigid body with game object transform , GameObject missing update method for now
        // if (i == 200) rb->SetType(ERigidBodyType::Kinematic); // test static body
        std::cout << objPlane->Transform.GetPosition() << std::endl;
        std::cout << obj->Transform.GetPosition() << std::endl;
    }

    return 0;
}
