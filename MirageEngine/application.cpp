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
#include <Components/box_collider.h>
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
Camera* camera;

void Application::InitGameObject()
{
	using namespace MirageEngine;
	using namespace MirageEngine::Core::Physics;

	PhysicsManager* physics = PhysicsManager::Instance();

	// =========================
	//  Physics setup
	// =========================
	BoxCollider* dynamicCollider = new BoxCollider();
	IRigidBody* dynamicRB = physics->CreateRigidBody(dynamicCollider->GetCollision(), ERigidBodyType::Dynamic);

	dynamicRB->SetCollisionGroup(ECollisionGroup::Group1);
	dynamicRB->SetCollisionMask(
		ECollisionGroup::Group1 |
		ECollisionGroup::Group2 |
		ECollisionGroup::Group3
	);
	dynamicRB->SetCollisionResponse(ECollisionResponse::Block);

	ICollider* staticCollider = physics->CreateCollider(ECollisionShapeType::Box);
	IRigidBody* staticRB = physics->CreateRigidBody(staticCollider, ERigidBodyType::Static);

	staticRB->SetCollisionGroup(ECollisionGroup::Group2);
	staticRB->SetCollisionMask(ECollisionGroup::All);

	// =========================
	//  Scene graph
	// =========================
	GameObject* root = m_SceneGraph.CreateGameObject("Root", nullptr);
	GameObject* cameraGO = m_SceneGraph.CreateGameObject("Commelin", root);
	GameObject* floorGO = m_SceneGraph.CreateGameObject("Lin", root);
	GameObject* child1 = m_SceneGraph.CreateGameObject("Monin", floorGO);
	GameObject* child2 = m_SceneGraph.CreateGameObject("Tang", child1);

	// =========================
	//  Camera object
	// =========================
	cameraGO->AddComponent<Camera>();
	cameraGO->AddComponent<InputMapping>();
	cameraGO->AddComponent<MeshRenderer>();
	cameraGO->AddComponent(dynamicCollider);
	cameraGO->AddComponent(dynamicRB);

	Camera* cam = cameraGO->GetComponent<Camera>();
	InputMapping* input = cameraGO->GetComponent<InputMapping>();
	MeshRenderer* renderer1 = cameraGO->GetComponent<MeshRenderer>();

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

	MeshRenderer* renderer2 = floorGO->GetComponent<MeshRenderer>();

	Quaternion rotationQ(Vector3(1.f, 0.f, 0.f), PI / 12.f);

	floorGO->SetPosition(Vector3(0, -3, 0));
	floorGO->SetRotation(rotationQ);
	floorGO->SetScale(Vector3(10, 0, 10));

	// =========================
	//  Resources
	// =========================
	ResourceManager* rm = ResourceManager::Instance();

	Texture* texCat = rm->GetTexture("cat");
	Texture* texMirage = rm->GetTexture("Mirage");

	Material* mat1 = rm->CreateMaterial("TestMaterial");
	Material* mat2 = rm->CreateMaterial("Test2Material");

	mat1->SetShader(m_Renderer.m_Pipeline);
	mat2->SetShader(m_Renderer.m_Pipeline);

	mat1->SetTexture("cat", texCat);
	mat2->SetTexture("Mirage", texMirage);

	renderer1->SetMaterial(mat1);
	renderer2->SetMaterial(mat2);

	Mesh* cubeMesh = rm->GetMesh("cube");
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
	camera = cam;
	meshRenderer = renderer1;
	meshRenderer2 = renderer2;
}

void Application::ShutDown()
{
	m_Renderer.Shutdown();
	delete m_EditorMapping;
	delete m_PlayMapping;
	delete m_EngineCamera;
}
