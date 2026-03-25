#include "application.h"
#include "OpenGL/openGL_render_interface.h"
#include "debug.h"

using namespace MirageEngine::Core::Math;

void Application::InitApp()
{
	InitWindow();
	InitRHI();
	InitScene();
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

	// Instantiate All Classes
	m_Instance = m_RenderInterface->InstantiateInstance();
	m_Surface = m_RenderInterface->InstantiateSurface();
	m_Device = m_RenderInterface->InstantiateDevice();
	m_SwapChain = m_RenderInterface->InstantiateSwapChain();
	m_RenderPass = m_RenderInterface->InstantiateRenderPass();
	m_VertexShader = m_RenderInterface->InstantiateShader();
	m_FragmentShader = m_RenderInterface->InstantiateShader();
	m_PipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
	m_Pipeline = m_RenderInterface->InstantiatePipeline();
	m_Framebuffer = m_RenderInterface->InstantiateFramebuffer();
	m_BufferTriangleVertices = m_RenderInterface->InstantiateBuffer();
	m_BufferTriangleIndices = m_RenderInterface->InstantiateBuffer();
	m_TextureBuffer = m_RenderInterface->InstantiateBuffer();
	m_UniformBuffer = m_RenderInterface->InstantiateUniformBuffer();
	m_CommandBuffer = m_RenderInterface->InstantiateCommandBuffer();
	DEBUG_LOG("All Classes Instantiated");

	m_VerticesQuad = // Set Vertices to draw Triangle
	{
		{{1.f, 1.f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.f, -1.f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.f, -1.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{-1.f, 1.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
	};

	m_IndicesQuad = // Set Indexes to draw Triangle
	{
		0, 1, 2, 2, 3, 0
	};
	DEBUG_LOG("Vertex and Index Data Created");


	// Create All Classes
	m_Instance->Create("Mirage");
	m_Surface->Create(m_Window, m_Width, m_Height);
	m_Device->Create();
	m_SwapChain->Create();
	m_RenderPass->Create(Vector4(0.2f, 0, 0.1f, 1.0f), 1.0f);
	m_VertexShader->Create("assets/shaders/vertex.glsl", MirageRHI::IShader::ShaderType::Vertex);
	m_FragmentShader->Create("assets/shaders/fragment.glsl", MirageRHI::IShader::ShaderType::Fragment);
	m_PipelineParameters->Create(MirageRHI::VertexType::PBR, MirageRHI::CullingType::None, true);
	m_Pipeline->Create(m_VertexShader, m_FragmentShader, m_PipelineParameters);
	m_BufferTriangleVertices->Create(m_VerticesQuad.data(), m_VerticesQuad.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Vertices);
	m_BufferTriangleIndices->Create(m_IndicesQuad.data(), m_IndicesQuad.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Indices);
	m_TextureBuffer->Create("assets/textures/cat.png", MirageRHI::IBuffer::BufferType::Texture);
	m_UniformBuffer->Create();
	m_CommandBuffer->Create();
	DEBUG_LOG("All Classes Created");
}

void Application::InitScene()
{
}

void Application::RunMainLoop()
{
	uint32_t indicesTriangleListSize = (uint32_t)(m_IndicesQuad.size());

	while (!glfwWindowShouldClose(m_Window))
	{
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);


		// Setup to Draw
		m_CommandBuffer->BeginRenderPass(m_RenderPass);
		m_CommandBuffer->BeginDraw();
		m_CommandBuffer->BindPipeline(m_Pipeline);
		m_CommandBuffer->BindBuffer(m_BufferTriangleIndices);
		m_CommandBuffer->BindBuffer(m_BufferTriangleVertices);
		m_CommandBuffer->BindBuffer(m_TextureBuffer);
		m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);
		m_UniformBuffer->UpdateUniformBuffer(m_Pipeline, Vector3(0, 0, -2), Vector3(1, 1, 1), Vector3(0, 0, 0));
		m_CommandBuffer->DrawIndexed(indicesTriangleListSize);
		m_CommandBuffer->EndDraw();
		m_CommandBuffer->EndRenderPass();
		m_CommandBuffer->SubmitAndPresent(m_SwapChain, m_Surface);
		glfwPollEvents();
	}
}

void Application::ShutDown()
{
	glfwMakeContextCurrent(m_Window);
	glfwTerminate();
}