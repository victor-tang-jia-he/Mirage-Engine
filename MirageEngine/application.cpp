#include <application.h>

#include <renderer.h>
#include <SceneGraph/scene_graph.h>

#include <debug.h>

// Resources
#include <Resources/resource_manager.h>
#include <Resources/mesh.h>
#include <Resources/material.h>
#include <Resources/texture.h>
#include <Resources/shader.h>

// GameObject & Components
#include <GameObject/game_object.h>
#include <Components/input_system.h>
#include <Components/camera.h>
#include <Components/mesh_renderer.h>

// Physics and physics Components
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Resources;

void Application::InitApp()
{
    Loggers::LogBuffer::GetInstance().EnableImmediateFlush(true); // Enable immediate flushing of log messages for real-time output during initialization
    Loggers::LogBuffer::GetInstance().EnableFileWriting(true);    // Enable writing logs to file during initialization to capture any issues that may occur before the main loop starts.

    m_Renderer.InitRenderer();

    MirageEngine::Core::Resources::ResourceManager::Instance()->Init(m_Renderer.GetRenderInterfaceHandle());
    MirageEngine::Core::Resources::ResourceManager::Instance()->PreLoadResources();
    MirageEngine::Core::Physics::PhysicsManager::Instance()->Initialize();

    InitScene();
    InitInput();
    InitGameObject();
}

void Application::InitScene()
{
    m_EngineCamera = new Camera(1920, 1080, 90.f, 0.1f, 100.f, Vector3(0.f, 0.f, -3.f));
    m_SceneGraph.SetEditorCamera(m_EngineCamera);
}

void Application::InitInput()
{
    // creating editorMapping
    m_EditorMapping = new InputMapping();

    // add random function test member of current class
    // m_EditorMapping->Add(GLFW_KEY_W, this, &Application::EditorPrintInput); // template

    // init with previous created map
    InputSystem::Initialize(m_EditorMapping);
    // get and set callback
    GLFWwindow* window = glfwGetCurrentContext();
    glfwSetKeyCallback(window, InputSystem::KeyCallback);

    // bind camera
    m_EngineCamera->BindInput(m_EditorMapping);
}

using namespace MirageEngine::Core::Resources;
MeshRenderer* meshRenderer;
MeshRenderer* meshRenderer2;
Camera*       camera;

void Application::InitGameObject()
{
    using namespace MirageEngine::Core::Physics;
    PhysicsManager* pManager      = PhysicsManager::Instance();
    ICollider*      collider      = pManager->CreateCollider(ECollisionShapeType::Box);
    IRigidBody*     rb            = pManager->CreateRigidBody(collider, ERigidBodyType::Dynamic);


    ICollider*      colliderPlane = pManager->CreateCollider(ECollisionShapeType::Box);
    //if (auto* box = colliderPlane->As<IBoxCollider>()) box->SetHalfExtents(Vector3(1, 1, 1));
    Vector3     rotation = Vector3(1.f, 0.f, 0.f);
    Quaternion  rotationQ(rotation, PI / 12.f);
    IRigidBody* rb2 = pManager->CreateRigidBody(colliderPlane, ERigidBodyType::Static);

    rb->SetCollisionGroup(ECollisionGroup::Group1);
    rb->SetCollisionMask(ECollisionGroup::Group1 | ECollisionGroup::Group2 | ECollisionGroup::Group3);
    rb->SetCollisionResponse(ECollisionResponse::Block);

    rb2->SetCollisionGroup(ECollisionGroup::Group2);
    rb2->SetCollisionMask(ECollisionGroup::All);

    MirageEngine::GameObject* root = m_SceneGraph.CreateGameObject("Root", nullptr);
    MirageEngine::GameObject* obj1 = m_SceneGraph.CreateGameObject("Commelin", root);
    MirageEngine::GameObject* obj2 = m_SceneGraph.CreateGameObject("Lin", root);
    MirageEngine::GameObject* obj3 = m_SceneGraph.CreateGameObject("Monin", obj2);
    MirageEngine::GameObject* obj4 = m_SceneGraph.CreateGameObject("Tang", obj3);

    obj1->AddComponent<Camera>();
    obj1->AddComponent<InputMapping>();
    obj1->AddComponent<MeshRenderer>();
    obj1->AddComponent(collider);
    obj1->AddComponent(rb);


    obj2->AddComponent(colliderPlane);
    obj2->AddComponent(rb2);
    obj2->AddComponent<MeshRenderer>();
    obj2->SetPosition(Vector3(0, -3, 0));
    obj2->SetRotation(rotationQ);
    obj2->SetScale(Vector3(10, 1, 10));

    camera = obj1->GetComponent<Camera>();
    m_SceneGraph.SetEditorCamera(camera);
    InputMapping* inputMapping = obj1->GetComponent<InputMapping>();
    meshRenderer               = obj1->GetComponent<MeshRenderer>();
    meshRenderer2              = obj2->GetComponent<MeshRenderer>();
    camera->BindInput(inputMapping);
    camera->SetFar(100);
    camera->SetHeight(1080);
    camera->SetWidth(1920);
    camera->SetNear(0.1);
    camera->SetFov(60.f);
    Texture*  texture  = ResourceManager::Instance()->GetTexture("cat");
    Texture*  mirageTexture  = ResourceManager::Instance()->GetTexture("Mirage");
    Material* material = ResourceManager::Instance()->CreateMaterial("TestMaterial");
    Material* material2 = ResourceManager::Instance()->CreateMaterial("Test2Material");
    material->SetShader(m_Renderer.m_Pipeline);
    material2->SetShader(m_Renderer.m_Pipeline);
    material->SetTexture("cat", texture);
    material2->SetTexture("Mirage", mirageTexture);
    meshRenderer->SetMaterial(material);
    meshRenderer2->SetMaterial(material2);
    ResourceManager::Instance()->GetMesh("cube")->Bind();
    meshRenderer->SetMesh(ResourceManager::Instance()->GetMesh("cube"));
    meshRenderer2->SetMesh(ResourceManager::Instance()->GetMesh("cube"));

    InputSystem::SetCurrentMapping(inputMapping);

    root->Transform.SetPosition(Vector3(0, 0, -10));
}

void Application::ShutDown()
{
    m_Renderer.Shutdown();
    delete m_EditorMapping;
    delete m_PlayMapping;
    delete m_EngineCamera;
}
