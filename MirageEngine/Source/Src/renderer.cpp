#include "renderer.h"

#include <OpenGL/openGL_render_interface.h>

#include <debug.h>

using namespace MirageEngine;
using namespace MirageEngine::Core::Math;

void Renderer::InitRenderer()
{
	InitWindow();
	InitRHI();
}

void Renderer::Shutdown()
{
	m_CommandBuffer->Destroy();
	m_UniformBuffer->Destroy();
	m_ScreenPipeline->Destroy();
	m_ScreenPipelineParameters->Destroy();
	m_RenderPass->Destroy();
	m_SwapChain->Destroy();
	m_Device->Destroy();
	m_Surface->Destroy();
	m_Instance->Destroy();

	delete m_RenderInterface;
	delete m_CommandBuffer;
	delete m_UniformBuffer;
	delete m_ScreenPipeline;
	delete m_ScreenPipelineParameters;
	delete m_RenderPass;
	delete m_SwapChain;
	delete m_Device;
	delete m_Surface;
	delete m_Instance;

	glfwTerminate();
	DEBUG_LOG("Renderer Shutdown");
}

void Renderer::BeginFrame()
{
	glfwPollEvents();

	uint32_t currentFBO = m_SwapChain->AcquireCurrentFBO();
	m_CommandBuffer->BeginDraw(m_RenderPass, currentFBO);
}

void Renderer::RenderScene()
{
	//m_CommandBuffer->BindPipeline(m_Pipeline);
	//m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);

	//m_UniformBuffer->SetView(m_EngineCamera->GetView());
	//m_UniformBuffer->SetProjection(m_EngineCamera->GetPerspective());
	//m_UniformBuffer->UpdateUniformBuffer(m_Pipeline, Matrix4::CreateTransformMatrix(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1))); // view/proj

	//for (std::shared_ptr<MirageEngine::Core::Components::MeshRenderer> meshRenderer : m_meshRenderer) 
	//{ 
	//	meshRenderer->Render(m_CommandBuffer, m_Pipeline, m_UniformBuffer); 
	//}
}

void Renderer::RenderScreen()
{
	// End previous render pass and start new one for screen rendering
	m_CommandBuffer->EndDraw();
	m_CommandBuffer->BeginDraw(m_RenderPass, 0);

	m_CommandBuffer->BindPipeline(m_ScreenPipeline);
	m_CommandBuffer->ApplyPipelineParameters(m_ScreenPipeline);
	m_CommandBuffer->BindTextureFromID(m_SwapChain->AcquireCurrentImage());

	m_CommandBuffer->Draw(3);
}

void Renderer::EndFrame()
{
	m_CommandBuffer->EndDraw();
	m_CommandBuffer->SubmitAndPresent(m_SwapChain, m_Surface);
}

void Renderer::InitWindow()
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

void Renderer::InitRHI()
{
	// Select RHI API
	switch (m_API)
	{
	case MirageRHI::IRenderInterface::API::OPENGL:
		m_RenderInterface = new MirageRHI::OpenGLRenderInterface();
		break;
	default:
		DEBUG_THROW("Unsupported API");
	}
	DEBUG_LOG("Render Interface Created");

	// Instantiate All Classes
	m_Instance = m_RenderInterface->InstantiateInstance();
	m_Surface = m_RenderInterface->InstantiateSurface();
	m_Device = m_RenderInterface->InstantiateDevice();
	m_SwapChain = m_RenderInterface->InstantiateSwapChain();
	m_RenderPass = m_RenderInterface->InstantiateRenderPass();
	m_ScreenPipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
	m_ScreenPipeline = m_RenderInterface->InstantiatePipeline();
	m_UniformBuffer = m_RenderInterface->InstantiateUniformBuffer();
	m_CommandBuffer = m_RenderInterface->InstantiateCommandBuffer();
	DEBUG_LOG("All Classes Instantiated");

	// Create All Classes
	m_Instance->Create("Mirage");
	m_Surface->Create(m_Window, m_Width, m_Height);
	m_Device->Create();
	m_SwapChain->Create(2, m_Width, m_Height);
	m_RenderPass->Create(Vector4(0.f, 0.5f, 0.5f, 1.0f), 1.0f);

	m_ScreenPipelineParameters->Create(MirageRHI::VertexType::None, MirageRHI::CullingType::None, false);

	// Creation of single use shader objects for screen rendering pipeline
	MirageRHI::IShader* screenVertShader = m_RenderInterface->InstantiateShader();
	MirageRHI::IShader* screenFragShader = m_RenderInterface->InstantiateShader();

	screenVertShader->Create("Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
	screenFragShader->Create("Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);

	m_ScreenPipeline->Create(screenVertShader, screenFragShader, m_ScreenPipelineParameters);

	// Single use shader objects is destroyed after screen rendering pipeline creation
	screenVertShader->Destroy();
	screenFragShader->Destroy();
	
	m_UniformBuffer->Create();
	m_CommandBuffer->Create();
	DEBUG_LOG("All Classes Created");
}