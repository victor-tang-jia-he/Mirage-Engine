//#include <iostream>
//#include <vector>
//#include <Physics/physics_manager.h>
//#include <Physics/Bullet3/bullet3_rigidbody.h>
//
//int main()
//{
//    // Initialize Bullet3 physics world
//    using namespace MirageEngine::Core::Physics;
//    using namespace MirageEngine;
//    PhysicsManager* pManager = PhysicsManager::Instance();
//
//    pManager->Initialize(EPhysicsEngine::Bullet3);
//    ICollider*  collider = pManager->CreateCollider(ECollisionShapeType::Box);
//    IRigidBody* rb       = pManager->CreateRigidBody(collider, ERigidBodyType::Dynamic);
//
//    GameObject* obj = new GameObject(nullptr, "TestObject");
//    obj->AddComponent(collider);
//    obj->AddComponent(rb);
//
//    ICollider*  colliderPlane = pManager->CreateCollider(ECollisionShapeType::Sphere);
//    IRigidBody* rbPlane       = pManager->CreateRigidBody(colliderPlane, ERigidBodyType::Static);
//    rbPlane->SetMass(1.f); // Trigger warning
//    if (auto* box = colliderPlane->As<IBoxCollider>()) { box->SetHalfExtents(Core::Math::Vector3(1.f, 0.5f, 1.f)); }
//    Vector3 rotation = Vector3(0);
//    colliderPlane->SetOffset(Vector3(0), Quaternion(rotation, 45.f));
//
//    GameObject* objPlane = new GameObject(nullptr, "plane"); // fake
//    objPlane->AddComponent(colliderPlane);
//    objPlane->AddComponent(rbPlane);
//    objPlane->SetPosition(MirageEngine::Core::Math::Vector3(0, -5.f, 0));
//
//    std::cout << "Added body to world. Total objects: " << pManager->m_worldPhysics->API_Bullet3().m_World->getNumCollisionObjects() << std::endl;
//
//    for (int i = 0; i < 3000; ++i)
//    {
//        pManager->Update(1.0f / 60.0f); // stepSimulation + sync
//
//        rb->Update();      // sync rigid body with game object transform , GameObject missing update method for now
//        rbPlane->Update(); // sync rigid body with game object transform , GameObject missing update method for now
//        // if (i == 200) rb->SetType(ERigidBodyType::Kinematic); // test static body
//        std::cout << obj->Transform.GetPosition() << std::endl;
//        std::cout << objPlane->Transform.GetPosition() << std::endl;
//    }
//    delete obj;
//    delete objPlane;
//
//    return 0;
//}
