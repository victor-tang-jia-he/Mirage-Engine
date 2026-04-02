//#include "renderer.h"
//
//#include <debug.h>
//
//using namespace MirageRHI;
//
//void MirageEngine::Renderer::InitRenderer()
//{
//	InitWindow();
//	InitRHI();
//}
//
//void MirageEngine::Renderer::Shutdown()
//{
//}
//
//void MirageEngine::Renderer::InitWindow()
//{
//	if (!glfwInit()) {
//		DEBUG_THROW("Failed to init GLFW");
//	}
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	m_Width = 1920;
//	m_Height = 1080;
//
//	m_Window = glfwCreateWindow(m_Width, m_Height, "Mirage Engine", nullptr, nullptr);
//	if (!m_Window) {
//		glfwTerminate();
//		DEBUG_THROW("Failed to create window");
//	}
//
//	glfwMakeContextCurrent(m_Window);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		DEBUG_THROW("Failed to init GLAD");
//	}
//
//	glViewport(0, 0, m_Width, m_Height);
//
//	DEBUG_LOG("Window Created");
//}
//
//void MirageEngine::Renderer::InitRHI()
//{
//	// Select RHI API
//	switch (m_API)
//	{
//	case MirageRHI::IRenderInterface::API::OPENGL:
//		m_RenderInterface = new MirageRHI::OpenGLRenderInterface();
//		break;
//	default:
//		DEBUG_THROW("Unsupported RHI API");
//	}
//	DEBUG_LOG("Render Interface Created");
//
//	// Instantiate All Classes
//	m_Instance = m_RenderInterface->InstantiateInstance();
//	m_Surface = m_RenderInterface->InstantiateSurface();
//	m_Device = m_RenderInterface->InstantiateDevice();
//	m_SwapChain = m_RenderInterface->InstantiateSwapChain();
//	m_RenderPass = m_RenderInterface->InstantiateRenderPass();
//	m_PipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
//	m_ScreenPipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
//	m_Pipeline = m_RenderInterface->InstantiatePipeline();
//	m_ScreenPipeline = m_RenderInterface->InstantiatePipeline();
//	m_UniformBuffer = m_RenderInterface->InstantiateUniformBuffer();
//	m_CommandBuffer = m_RenderInterface->InstantiateCommandBuffer();
//	DEBUG_LOG("All Classes Instantiated");
//
//	// Create All Classes
//	m_Instance->Create("Mirage");
//	m_Surface->Create(m_Window, m_Width, m_Height);
//	m_Device->Create();
//	m_SwapChain->Create(2, m_Width, m_Height);
//	m_RenderPass->Create(Vector4(0.f, 0.5f, 0.5f, 1.0f), 1.0f);
//
//	m_PipelineParameters->Create(MirageRHI::VertexType::PBR, MirageRHI::CullingType::None, true);
//	m_ScreenPipelineParameters->Create(MirageRHI::VertexType::None, MirageRHI::CullingType::None, false);
//	m_Pipeline->Create(ResourceManager::Instance()->Get<Shader>("defaultVert")->ShaderBind, ResourceManager::Instance()->Get<Shader>("defaultFrag")->ShaderBind, m_PipelineParameters);
//
//	MirageRHI::IShader* Vert = new OpenGLShader();
//	Vert->Create("Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
//	MirageRHI::IShader* Frag = new OpenGLShader();
//	Frag->Create("Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);
//
//	m_ScreenPipeline->Create(Vert, Frag, m_ScreenPipelineParameters);
//
//	//m_ScreenPipeline->Create(ResourceManager::Instance()->Get<Shader>("screenVert")->ShaderBind, ResourceManager::Instance()->Get<Shader>("screenFrag")->ShaderBind, m_ScreenPipelineParameters);
//	ResourceManager::Instance()->CreateTexture("Cat", "Assets/Textures/cat.png")->GetTextureBuffer();
//	ResourceManager::Instance()->CreateTexture("Mirage", "Assets/Textures/Mirage.png")->GetTextureBuffer();
//	MeshRenderer one = MeshRenderer(mesh, ResourceManager::Instance()->CreateMaterial("DefaultMaterial"));
//	MeshRenderer two = MeshRenderer(mesh, ResourceManager::Instance()->Get<Material>("DefaultMaterial"));
//	one.Material->SetTexture("Cat", ResourceManager::Instance()->Get<Texture>("Cat"));
//	two.Material->SetTexture("Mirage", ResourceManager::Instance()->Get<Texture>("Mirage"));
//	two.SetPos(Vector3(2.f, 2.f, 0.f));
//
//
//	Vector3 rotation = Vector3(0.f, 180.f, 0.f);
//	two.SetRotation(Quaternion(rotation));
//	m_meshRenderer.push_back(std::make_shared<MeshRenderer>(one));
//	m_meshRenderer.push_back(std::make_shared<MeshRenderer>(two));
//	Material* material = nullptr;
//	if (ResourceManager::Instance()->TryGet<Material>("DefaultMaterial", material))
//	{
//		material->SetShader(m_Pipeline);
//	}
//	m_UniformBuffer->Create();
//	m_CommandBuffer->Create();
//	DEBUG_LOG("All Classes Created");
//}
//
//
