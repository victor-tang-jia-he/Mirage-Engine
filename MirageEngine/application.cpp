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

	ResourceManager* resManager = ResourceManager::Instance();
	resManager->Init(m_Renderer.GetRenderInterfaceHandle());
	MirageEngine::Core::Resources::ResourceManager::Instance()->PreLoadResources();

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
	//m_EditorMapping->Add(GLFW_KEY_W, this, &Application::EditorPrintInput); // template

	// init with previous created map
	InputSystem::Initialize(m_EditorMapping);
	// get and set callback
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetKeyCallback(window, InputSystem::KeyCallback);

	// bind camera
	m_EngineCamera->BindInput(m_EditorMapping);
	DEBUG_LOG("Input system bind");
}

using namespace MirageEngine::Core::Resources;
MeshRenderer* meshRenderer;
Camera* camera;

void Application::InitGameObject()
{
	MirageEngine::GameObject* root = m_SceneGraph.CreateGameObject("Root", nullptr);
	MirageEngine::GameObject* obj1 = m_SceneGraph.CreateGameObject("Commelin", root);
	MirageEngine::GameObject* obj2 = m_SceneGraph.CreateGameObject("Lin", root);
	MirageEngine::GameObject* obj3 = m_SceneGraph.CreateGameObject("Monin", obj2);
	MirageEngine::GameObject* obj4 = m_SceneGraph.CreateGameObject("Tang", obj3);

	//root->AddChild(obj3);

	obj1->AddComponent<Camera>();
	obj1->AddComponent<InputMapping>();
	obj1->AddComponent<MeshRenderer>();
	camera = obj1->GetComponent<Camera>();
	m_SceneGraph.SetEditorCamera(camera);
	InputMapping* inputMapping = obj1->GetComponent<InputMapping>();
	meshRenderer = obj1->GetComponent<MeshRenderer>();

	std::cout << obj1->GetName().c_str() << " -> " << camera->GetName().c_str() << std::endl;
	std::cout << obj1->GetName().c_str() << " -> " << inputMapping->GetName().c_str() << std::endl;
	std::cout << obj1->GetName().c_str() << " -> " << meshRenderer->GetName().c_str() << std::endl;
	camera->BindInput(inputMapping);
    camera->SetFar(100);
    camera->SetHeight(1080);
    camera->SetWidth(1920);
    camera->SetNear(0.1);
	camera->SetFov(60.f);
	Texture* texture = ResourceManager::Instance()->GetTexture("cat");
	Material* material = ResourceManager::Instance()->CreateMaterial("TestMaterial");
    texture->Bind();
	material->SetShader(m_Renderer.m_Pipeline);
	material->SetTexture("cat", texture);
    meshRenderer->SetMaterial(material);
    ResourceManager::Instance()->GetMesh("cube")->Bind();
	meshRenderer->SetMesh(ResourceManager::Instance()->GetMesh("cube"));

    InputSystem::SetCurrentMapping(inputMapping);
}

void Application::ShutDown()
{
	m_Renderer.Shutdown();
    delete m_EditorMapping;
    delete m_PlayMapping;
    delete m_EngineCamera;
}
