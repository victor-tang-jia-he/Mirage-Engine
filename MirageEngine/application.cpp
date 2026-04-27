#include <GLFW/glfw3.h>
#include <application.h>

#include <renderer.h>
#include <SceneGraph/scene_graph.h>

#include <debug.h>

// Resources
#include <Resources/resource_manager.h>
#include <Resources/mesh.h>
#include <Resources/material.h>
#include <Resources/material_instance.h>
#include <Resources/texture.h>
#include <Resources/shader.h>

// GameObject & Components
#include <GameObject/game_object.h>
#include <Input/input_system.h>
#include <Components/box_collider.h>
#include <Components/capsule_collider.h>
#include <Components/sphere_collider.h>
#include <Components/rigidbody.h>
#include <Components/camera.h>
#include <Components/mesh_renderer.h>

// Physics and physics Components
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Resources;

Application* Application::s_Instance = nullptr;
void Application::InitApp()
{
    s_Instance = this;

    Loggers::LogBuffer::GetInstance().EnableImmediateFlush(true); // Enable immediate flushing of log messages for real-time output during initialization
    Loggers::LogBuffer::GetInstance().EnableFileWriting(true);    // Enable writing logs to file during initialization to capture any issues that may occur before the main loop starts.

    m_Renderer.InitRenderer();

    MirageEngine::Core::Resources::ResourceManager::Instance().Init(m_Renderer.GetRenderInterfaceHandle());
    MirageEngine::Core::Resources::ResourceManager::Instance().PreLoadResources();
    MirageEngine::Core::Physics::PhysicsManager::Instance().Initialize();

    InitScene();
    InitInput();
    InitGameObject();
}

void Application::InitScene()
{
    m_EngineCamera = std::make_shared<Camera>(1920, 1080, 90.f, 0.1f, 100.f, Vector3(0.f, 0.f, -3.f));
    m_SceneGraph.SetEditorCamera(m_EngineCamera);
}

void Application::InitInput()
{
    // creating editorMapping
    m_EditorMapping = std::make_shared<InputMapping>();

    // add random function test member of current class
    // m_EditorMapping->Add(GLFW_KEY_W, this, &Application::EditorPrintInput); // template

    // init with previous created map
    InputSystem::Initialize(m_EditorMapping);
    // get and set callback
    GLFWwindow* window = glfwGetCurrentContext();
    glfwSetKeyCallback(window, InputSystem::KeyCallback);
    glfwSetMouseButtonCallback(window, InputSystem::MouseButtonCallback);
    glfwSetCursorPosCallback(window, InputSystem::CursorPosCallback);
    // bind camera
    m_EngineCamera->BindInput(m_EditorMapping);
}

using namespace MirageEngine::Core::Resources;
MeshRenderer* meshRenderer;
MeshRenderer* meshRenderer2;
Camera*       camera;

void Application::InitGameObject()
{
    using namespace MirageEngine;
    using namespace MirageEngine::Core::Physics;

    PhysicsManager& physics = PhysicsManager::Instance();

    // =========================
    //  Physics setup
    // =========================
    std::shared_ptr<SphereCollider> dynamicCollider = std::make_shared<SphereCollider>();
    std::shared_ptr<Rigidbody>      dynamicRB       = std::make_shared<Rigidbody>(dynamicCollider.get(), ERigidBodyType::Dynamic);

    dynamicRB->SetCollisionGroup(ECollisionGroup::Group1);
    dynamicRB->SetCollisionMask(ECollisionGroup::Group1 | ECollisionGroup::Group2 | ECollisionGroup::Group3);
    dynamicRB->SetCollisionResponse(ECollisionResponse::Block);

    std::shared_ptr<BoxCollider> staticCollider = std::make_shared<BoxCollider>();
    std::shared_ptr<Rigidbody>   staticRB       = std::make_shared<Rigidbody>(staticCollider.get(), ERigidBodyType::Static);

    staticRB->SetCollisionGroup(ECollisionGroup::Group2);
    staticRB->SetCollisionMask(ECollisionGroup::All);

    // =========================
    //  Scene graph
    // =========================
    std::shared_ptr<GameObject> root     = m_SceneGraph.CreateGameObject("Root", nullptr);
    std::shared_ptr<GameObject> cameraGO = m_SceneGraph.CreateGameObject("Commelin", root);
    std::shared_ptr<GameObject> floorGO  = m_SceneGraph.CreateGameObject("Lin", root);
    std::shared_ptr<GameObject> child1   = m_SceneGraph.CreateGameObject("Monin", floorGO);
    std::shared_ptr<GameObject> child2   = m_SceneGraph.CreateGameObject("Tang", child1);
    ResourceManager::Instance().GetMesh("cube")->Bind();
    ResourceManager::Instance().GetMesh("sphere")->Bind();
    ResourceManager::Instance().GetMesh("cylinder")->Bind();
    ResourceManager::Instance().GetMesh("plane")->Bind();
    ResourceManager::Instance().GetMesh("cone")->Bind();
    // =========================
    //  Camera object
    // =========================
    cameraGO->AddComponent<Camera>();
    cameraGO->AddComponent<InputMapping>();
    cameraGO->AddComponent<MeshRenderer>();
    cameraGO->AddComponent(dynamicCollider);
    cameraGO->AddComponent(dynamicRB);

    std::shared_ptr<Camera>       cam       = cameraGO->GetComponent<Camera>();
    std::shared_ptr<InputMapping> input     = cameraGO->GetComponent<InputMapping>();
    std::shared_ptr<MeshRenderer> renderer1 = cameraGO->GetComponent<MeshRenderer>();

    m_SceneGraph.SetEditorCamera(cam);

    cam->BindInput(input);
    cam->SetNear(0.1f);
    cam->SetFar(100.f);
    cam->SetFov(60.f);
    cam->SetWidth(1920);
    cam->SetHeight(1080);

    InputSystem::SetCurrentMapping(input);

    // =========================
    //  Floor object
    // =========================
    floorGO->AddComponent(staticCollider);
    floorGO->AddComponent(staticRB);
    floorGO->AddComponent<MeshRenderer>();

    std::shared_ptr<MeshRenderer> renderer2 = floorGO->GetComponent<MeshRenderer>();

    Quaternion rotationQ(Vector3(1.f, 0.f, 0.f), PI / 12.f);

    floorGO->SetPosition(Vector3(0, -3, 0));
    floorGO->SetRotation(rotationQ);
    floorGO->SetScale(Vector3(10, 1, 10));

    // =========================
    //  Resources
    // =========================
    ResourceManager& rm = ResourceManager::Instance();

    std::shared_ptr<Texture> texCat    = rm.GetTexture("cat");
    std::shared_ptr<Texture> texMirage = rm.GetTexture("Mirage");

    std::shared_ptr<Material> mat = rm.CreateMaterial("TestMaterial", "Assets/Defaults/default.vert", "Assets/Defaults/default.frag", 0);

    std::shared_ptr<MaterialInstance> mat1 = rm.CreateMaterialInstance("TestMaterialinstance", mat);
    std::shared_ptr<MaterialInstance> mat2 = rm.CreateMaterialInstance("TestMaterialinstance2", mat);

    texCat->Bind();
    mat1->SetTexture("ourTexture", texCat);
    mat2->SetTexture("ourTexture", texMirage);

    renderer1->SetMaterial(mat1);
    renderer2->SetMaterial(mat2);

    std::shared_ptr<Mesh> cubeMesh = rm.GetMesh("cube");
    cubeMesh->Bind();

    renderer1->SetMesh(cubeMesh);
    renderer2->SetMesh(cubeMesh);

    // =========================
    //  Root transform
    // =========================
    root->Transform.SetPosition(Vector3(0, 0, -10));

    // =========================
    //  Cache members
    // =========================
    camera        = cam.get();
    meshRenderer  = renderer1.get();
    meshRenderer2 = renderer2.get();
}

void Application::ShutDown()
{
    MirageEngine::Core::Input::InputSystem::Shutdown();
    MirageEngine::Core::Resources::ResourceManager::Instance().Shutdown();
    MirageEngine::Core::Physics::PhysicsManager::Instance().Shutdown();

    m_Renderer.Shutdown();
    m_SceneGraph.Clean();
    m_EditorMapping.reset();
    m_PlayMapping.reset();
    m_EngineCamera.reset();
}
