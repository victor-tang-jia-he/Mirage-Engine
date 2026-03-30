#include <application.h>
#include <OpenGL/openGL_render_interface.h>
#include <debug.h>
#include <Components/camera.h>
#include <Resources/resource_manager.h>

using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Math;

void Application::InitApp()
{
	InitWindow();
	InitRHI();
	InitScene();
	InitInput();
}

void Application::InitWindow()
{
	if (!glfwInit()) {
		DEBUG_THROW("Failed to init GLFW");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Width = 1920;
	m_Height = 1080;

	m_Window = glfwCreateWindow(m_Width, m_Height, "Mirage Engine", nullptr, nullptr);
	if (!m_Window) {
		glfwTerminate();
		DEBUG_THROW("Failed to create window");
	}

	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DEBUG_THROW("Failed to init GLAD");
	}

	glViewport(0, 0, m_Width, m_Height);

	DEBUG_LOG("Window Created");
}

void Application::InitRHI()
{
	using namespace MirageEngine::Core::Resources;
	// Select RHI API
	switch (m_API)
	{
	case MirageRHI::IRenderInterface::API::OPENGL:
		m_RenderInterface = new MirageRHI::OpenGLRenderInterface();
		break;
	default:
		DEBUG_THROW("Unsupported RHI API");
	}
	DEBUG_LOG("Render Interface Created");
	ResourceManager::Instance()->Init(m_RenderInterface);

	// Instantiate All Classes
	m_Instance = m_RenderInterface->InstantiateInstance();
	m_Surface = m_RenderInterface->InstantiateSurface();
	m_Device = m_RenderInterface->InstantiateDevice();
	m_SwapChain = m_RenderInterface->InstantiateSwapChain();
	m_RenderPass = m_RenderInterface->InstantiateRenderPass();
	m_PipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
	m_Pipeline = m_RenderInterface->InstantiatePipeline();
	m_Framebuffer = m_RenderInterface->InstantiateFramebuffer();
	m_TextureBuffer = m_RenderInterface->InstantiateBuffer();
	m_UniformBuffer = m_RenderInterface->InstantiateUniformBuffer();
	m_CommandBuffer = m_RenderInterface->InstantiateCommandBuffer();
	DEBUG_LOG("All Classes Instantiated");

	//m_VerticesQuad = // Set Vertices to draw Triangle
	//{
	//	{{1.f, 1.f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	//	{{1.f, -1.f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	//	{{-1.f, -1.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
	//	{{-1.f, 1.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
	//};

	//m_IndicesQuad = // Set Indexes to draw Triangle
	//{
	//	0, 1, 2, 2, 3, 0
	//};
	IMesh* mesh = ResourceManager::Instance()->CreateMesh("Untitled", "Assets/Obj/Untitled.obj");
	DEBUG_LOG("Vertex and Index Data Created");


	// Create All Classes
	m_Instance->Create("Mirage");
	m_Surface->Create(m_Window, m_Width, m_Height);
	m_Device->Create();
	m_SwapChain->Create();
	m_RenderPass->Create(Vector4(0.2f, 0, 0.1f, 1.0f), 1.0f);
	
	
	ResourceManager::Instance()->CreateShader("vertex", "Assets/Shaders/vertex.glsl", MirageRHI::IShader::ShaderType::Vertex);
	ResourceManager::Instance()->CreateShader("fragment", "Assets/Shaders/fragment.glsl", MirageRHI::IShader::ShaderType::Fragment);
	//m_VertexShader = ResourceManager::Instance()->Get<MirageRHI::IShader>("vertex");
	//m_VertexShader->Create("Assets/Shaders/vertex.glsl", MirageRHI::IShader::ShaderType::Vertex);
	//m_FragmentShader->Create("Assets/Shaders/fragment.glsl", MirageRHI::IShader::ShaderType::Fragment);
	m_PipelineParameters->Create(MirageRHI::VertexType::PBR, MirageRHI::CullingType::None, true);
	m_Pipeline = ResourceManager::Instance()->CreatePipeline("pipeline",  "Assets/Shaders/fragment.glsl", "Assets/Shaders/vertex.glsl", m_PipelineParameters);
	//m_Pipeline->Create(m_VertexShader, m_FragmentShader, m_PipelineParameters);


	//m_BufferTriangleVertices->Create(m_VerticesQuad.data(), m_VerticesQuad.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Vertices);
	//m_BufferTriangleIndices->Create(m_IndicesQuad.data(), m_IndicesQuad.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Indices);
	//m_TextureBuffer->Create("Assets/Textures/cat.png", MirageRHI::IBuffer::BufferType::Texture);
	m_TextureBuffer = ResourceManager::Instance()->CreateTexture("Cat" ,"Assets/Textures/cat.png")->GetTextureBuffer();
	m_UniformBuffer->Create();
	m_CommandBuffer->Create();
	m_IndicesCount = (uint32_t)mesh->GetIndicesCount();
	DEBUG_LOG("All Classes Created");
}

void Application::InitScene()
{
	m_EngineCamera = new MirageEngine::Core::Components::Camera(1920, 1080, 90.f, 0.1f, 100.f, Vector3(0.f, 0.f, -3.f));
}

void Application::InitInput()
{

	// creating editorMapping
	editorMapping = new InputMapping();
	editorMapping->Map[GLFW_KEY_W] = InputBind{};
	editorMapping->Map[GLFW_KEY_A] = InputBind{};
	editorMapping->Map[GLFW_KEY_S] = InputBind{};
	editorMapping->Map[GLFW_KEY_D] = InputBind{};
	editorMapping->Map[GLFW_KEY_R] = InputBind{};
	editorMapping->Map[GLFW_KEY_Q] = InputBind{};
	editorMapping->Map[GLFW_KEY_E] = InputBind{};

	playMapping = new InputMapping();
	playMapping->Map[GLFW_KEY_W] = InputBind{};
	playMapping->Map[GLFW_KEY_A] = InputBind{};
	playMapping->Map[GLFW_KEY_S] = InputBind{};
	playMapping->Map[GLFW_KEY_D] = InputBind{};
	playMapping->Map[GLFW_KEY_R] = InputBind{};

	// add random function test member of current class
	editorMapping->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
	editorMapping->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
	editorMapping->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
	editorMapping->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
	editorMapping->Map[GLFW_KEY_R].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { StartPlay(info); }));

	playMapping->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
	playMapping->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
	playMapping->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
	playMapping->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
	playMapping->Map[GLFW_KEY_R].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EndPlay(info); }));

	// init with previous created map
	InputSystem::Initialize(editorMapping);

	// get and set callback
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetKeyCallback(window, InputSystem::KeyCallback);

	// bind camera
	m_EngineCamera->BindInput(editorMapping);
	DEBUG_LOG("Input system bind");
}

void Application::PrintInput(MirageEngine::Core::Input::CallbackInputInfo info)
{
	if (info.Pressed)
	{
		printf("Pressed\n");
	}
	if (info.Released)
	{
		printf("Released\n");
	}
}

void Application::EditorPrintInput(MirageEngine::Core::Input::CallbackInputInfo info)
{
	if (info.Pressed)
	{
		printf("Editor Pressed\n");
	}
	if (info.Released)
	{
		printf("Editor Released\n");
	}
}

void Application::StartPlay(MirageEngine::Core::Input::CallbackInputInfo info)
{
	if (info.Pressed)
	{
		printf("Editor -> Play\n");
		InputSystem::SetCurrentMapping(playMapping);
	}
}

void Application::EndPlay(MirageEngine::Core::Input::CallbackInputInfo info)
{
	if (info.Pressed)
	{
		printf("Play -> Editor\n");
		InputSystem::SetCurrentMapping(editorMapping);
	}
}

void Application::RunMainLoop()
{
	using namespace MirageEngine::Core::Resources;
	IMesh* mesh = ResourceManager::Instance()->Get<IMesh>("Untitled");
	Matrix4 tempModel = Matrix4::CreateTransformMatrix(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	while (!glfwWindowShouldClose(m_Window))
	{
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);

		InputSystem::UpdateHeldInput();
		// Setup to Draw
		m_CommandBuffer->BeginRenderPass(m_RenderPass);
		m_CommandBuffer->BeginDraw();
		m_CommandBuffer->BindPipeline(m_Pipeline);
		m_CommandBuffer->BindBuffer(mesh->GetIndices());
		m_CommandBuffer->BindBuffer(mesh->GetVertexBuffer());
		m_CommandBuffer->BindBuffer(m_TextureBuffer);
		m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);
		m_UniformBuffer->SetView(m_EngineCamera->GetView());
		m_UniformBuffer->SetProjection(m_EngineCamera->GetPerspective());
		m_UniformBuffer->UpdateUniformBuffer(m_Pipeline, tempModel);
		m_CommandBuffer->DrawIndexed((uint32_t)(mesh->GetIndicesCount()));
		m_CommandBuffer->EndDraw();
		m_CommandBuffer->EndRenderPass();
		m_CommandBuffer->SubmitAndPresent(m_SwapChain, m_Surface);
		glfwPollEvents();
	}
}

bool Application::IsRunning() const {
	return !glfwWindowShouldClose(m_Window);
}

void Application::BeginFrame() {
	glfwPollEvents();

	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);

	InputSystem::UpdateHeldInput();
	m_CommandBuffer->BeginRenderPass(m_RenderPass);
	m_CommandBuffer->BeginDraw();
}

void Application::RenderScene()
{
	using namespace MirageEngine::Core::Resources;

	IMesh* mesh = ResourceManager::Instance()->Get<IMesh>("Untitled");
	m_CommandBuffer->BindPipeline(m_Pipeline);
	m_CommandBuffer->BindBuffer(mesh->GetIndices());
	m_CommandBuffer->BindBuffer(mesh->GetVertexBuffer());
	m_CommandBuffer->BindBuffer(m_TextureBuffer);
	m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);
	m_UniformBuffer->SetView(m_EngineCamera->GetView());
	m_UniformBuffer->SetProjection(m_EngineCamera->GetPerspective());
	m_UniformBuffer->UpdateUniformBuffer(m_Pipeline, Matrix4::CreateTransformMatrix(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1)));
	m_CommandBuffer->DrawIndexed(m_IndicesCount);
}

void Application::EndFrame()
{
	m_CommandBuffer->EndDraw();
	m_CommandBuffer->EndRenderPass();
	m_CommandBuffer->SubmitAndPresent(m_SwapChain, m_Surface);
}

void Application::ShutDown()
{
	glfwMakeContextCurrent(m_Window);
	glfwTerminate();
	delete editorMapping;
	delete playMapping;
	delete m_EngineCamera;
}