#include <application.h>

// RHI
#include <OpenGL/openGL_render_interface.h>
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

void Application::InitApp()
{
    Loggers::LogBuffer::GetInstance().EnableImmediateFlush(true); // Enable immediate flushing of log messages for real-time output during initialization
    Loggers::LogBuffer::GetInstance().EnableFileWriting(true);    // Enable writing logs to file during initialization to capture any issues that may occur before the main loop starts.
    InitWindow();
    InitRHI();
    InitScene();
    InitInput();
    InitGameObject();
}

void Application::InitWindow()
{
    if (!glfwInit()) { DEBUG_THROW("Failed to init GLFW"); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Width  = 1920;
    m_Height = 1080;

    m_Window = glfwCreateWindow(m_Width, m_Height, "Mirage Engine", nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        DEBUG_THROW("Failed to create window");
    }

    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { DEBUG_THROW("Failed to init GLAD"); }

    glViewport(0, 0, m_Width, m_Height);

    DEBUG_LOG("Window Created");
}

void Application::InitRHI()
{
    using namespace MirageEngine::Core::Resources;
    using namespace MirageEngine::Core::Components;
    // Select RHI API
    switch (m_API)
    {
        case MirageRHI::IRenderInterface::API::OPENGL: m_RenderInterface = new MirageRHI::OpenGLRenderInterface(); break;
        default                                      : DEBUG_THROW("Unsupported RHI API");
    }
    ResourceManager* resManager = ResourceManager::Instance();
    resManager->Init(m_RenderInterface);

    MirageEngine::Core::Resources::ResourceManager::Instance()->PreLoadResources();
    DEBUG_LOG("Render Interface Created");

    // Instantiate All Classes
    m_Instance                 = m_RenderInterface->InstantiateInstance();
    m_Surface                  = m_RenderInterface->InstantiateSurface();
    m_Device                   = m_RenderInterface->InstantiateDevice();
    m_SwapChain                = m_RenderInterface->InstantiateSwapChain();
    m_RenderPass               = m_RenderInterface->InstantiateRenderPass();
    m_PipelineParameters       = m_RenderInterface->InstantiatePipelineParameters();
    m_ScreenPipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
    m_Pipeline                 = m_RenderInterface->InstantiatePipeline();
    m_ScreenPipeline           = m_RenderInterface->InstantiatePipeline();
    m_Framebuffer              = m_RenderInterface->InstantiateFramebuffer();
    m_UniformBuffer            = m_RenderInterface->InstantiateUniformBuffer();
    m_CommandBuffer            = m_RenderInterface->InstantiateCommandBuffer();
    DEBUG_LOG("All Classes Instantiated");

    //Mesh* mesh = resManager->CreateMesh("Cube.obj", "Assets/Mesh/Cube.obj");
    Mesh* mesh = resManager->GetMesh("cube");
    DEBUG_LOG("Vertex and Index Data Created");

    // Create All Classes
    m_Instance->Create("Mirage");
    m_Surface->Create(m_Window, m_Width, m_Height);
    m_Device->Create();
    m_SwapChain->Create(2, m_Width, m_Height);
    m_RenderPass->Create(Vector4(0.f, 0.5f, 0.5f, 1.0f), 1.0f);

    //resManager->CreateShader("default.vert", "Assets/Shaders/default.vert", MirageRHI::IShader::ShaderType::Vertex);
    //resManager->CreateShader("default.frag", "Assets/Shaders/default.frag", MirageRHI::IShader::ShaderType::Fragment);
    //resManager->CreateShader("screen.vert", "Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
    //resManager->CreateShader("screen.frag", "Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);
    m_PipelineParameters->Create(MirageRHI::VertexType::PBR, MirageRHI::CullingType::None, true);
    m_ScreenPipelineParameters->Create(MirageRHI::VertexType::None, MirageRHI::CullingType::None, false);

    m_Pipeline->Create(resManager->GetVertexShader("default_Vertex")->ShaderBind, resManager->GetFragmentShader("default_Fragment")->ShaderBind, m_PipelineParameters);

    MirageRHI::IShader* Vert = new OpenGLShader();
    Vert->Create("Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
    MirageRHI::IShader* Frag = new OpenGLShader();
    Frag->Create("Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);

    m_ScreenPipeline->Create(Vert, Frag, m_ScreenPipelineParameters);

    // m_ScreenPipeline->Create(resManager->Get<Shader>("screen.vert")->ShaderBind, resManager->Get<Shader>("screen.frag")->ShaderBind, m_ScreenPipelineParameters);
    //resManager->CreateTexture("Cat", "Assets/Textures/cat.png")->Bind();
    //resManager->CreateTexture("Mirage", "Assets/Textures/Mirage.png")->Bind();

    resManager->GetTexture("cat")->Bind();
    resManager->GetTexture("Mirage")->Bind();
    mesh->Bind(); // upload vertex and index data to GPU, then clear CPU-side data to save memory
    MeshRenderer one = MeshRenderer(mesh, resManager->CreateMaterial("DefaultMaterial"));
    MeshRenderer two = MeshRenderer(mesh, resManager->GetMaterial("DefaultMaterial"));
    one.Material->SetTexture("Cat", resManager->GetTexture("cat"));
    two.Material->SetTexture("Mirage", resManager->GetTexture("Mirage"));
    two.SetPos(Vector3(2.f, 2.f, 0.f));

    Vector3 rotation = Vector3(0.f, 180.f, 0.f);
    two.SetRotation(Quaternion(rotation));
    m_meshRenderer.push_back(std::make_shared<MeshRenderer>(one));
    m_meshRenderer.push_back(std::make_shared<MeshRenderer>(two));
    Material* material = nullptr;
    if (resManager->TryGet<Material>("DefaultMaterial", material)) { material->SetShader(m_Pipeline); }
    m_UniformBuffer->Create();
    m_CommandBuffer->Create();
    DEBUG_LOG("All Classes Created");
}

void Application::InitScene()
{
	m_EngineCamera = new Camera(1920, 1080, 90.f, 0.1f, 100.f, Vector3(0.f, 0.f, -3.f));
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

void Application::InitGameObject()
{
	MirageEngine::GameObject* root = new MirageEngine::GameObject(nullptr, "Root");
	MirageEngine::GameObject* obj1 = new MirageEngine::GameObject(root, "Commelin");
	MirageEngine::GameObject* obj2 = new MirageEngine::GameObject(root, "Lin");
	MirageEngine::GameObject* obj3 = new MirageEngine::GameObject(obj2, "Monin");
	MirageEngine::GameObject* obj4 = new MirageEngine::GameObject(obj3, "Tang");
	root->AddChild(obj1);
	root->AddChild(obj2);
	obj2->AddChild(obj3);
	obj3->AddChild(obj4);

	root->AddChild(obj3);

	obj1->AddComponent<Camera>();
	obj1->AddComponent<InputMapping>();
	Camera* camera = obj1->GetComponent<Camera>();
	// InputMapping* inputMapping = obj1->GetComponent<InputMapping>();

	std::cout << obj1->GetName().c_str() << " -> " << camera->GetName().c_str() << std::endl;
	// std::cout << obj1->GetName().c_str() << " -> " << inputMapping->GetName().c_str() << std::endl;
	camera->SetFov(60.f);
	/*inputMapping->Add(GLFW_KEY_T, new Event<CallbackInputInfo>("InputSystem", [](CallbackInputInfo info)
	{
		if (info.Pressed)
		{
			printf("T Pressed\n");
		}
		if (info.Released)
		{
			printf("T Released\n");
		}
	}));*/
}

bool Application::IsRunning() const { return !glfwWindowShouldClose(m_Window); }

void Application::BeginFrame()
{
    glfwPollEvents();
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(m_Window, true);

    InputSystem::UpdateHeldInput();
    uint32_t currentFBO = m_SwapChain->AcquireCurrentFBO();
    m_CommandBuffer->BeginDraw(m_RenderPass, currentFBO);
}

void Application::RenderScene()
{
    // using namespace MirageEngine::Core::Resources;

    m_CommandBuffer->BindPipeline(m_Pipeline);
    m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);

	for (std::shared_ptr<MeshRenderer> meshRenderer : m_meshRenderer)
	{
		meshRenderer->Render(m_CommandBuffer, m_Pipeline, m_UniformBuffer);
	}

    for (std::shared_ptr<MirageEngine::Core::Components::MeshRenderer> meshRenderer : m_meshRenderer) { meshRenderer->Render(m_CommandBuffer, m_Pipeline, m_UniformBuffer); }

    m_CommandBuffer->EndDraw();
    m_CommandBuffer->BeginDraw(m_RenderPass, 0);

    m_CommandBuffer->BindPipeline(m_ScreenPipeline);
    m_CommandBuffer->ApplyPipelineParameters(m_ScreenPipeline);
    m_CommandBuffer->BindTextureFromID(m_SwapChain->AcquireCurrentImage());

    m_CommandBuffer->Draw(3);
}

void Application::EndFrame()
{
    m_CommandBuffer->EndDraw();
    m_CommandBuffer->SubmitAndPresent(m_SwapChain, m_Surface);
}

void Application::ShutDown()
{
    glfwMakeContextCurrent(m_Window);
    glfwTerminate();
    delete m_EditorMapping;
    delete m_PlayMapping;
    delete m_EngineCamera;
}
