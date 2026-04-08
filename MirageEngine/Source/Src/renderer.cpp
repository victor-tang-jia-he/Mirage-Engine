#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"

#include <OpenGL/openGL_render_interface.h>
#include <SceneGraph/scene_graph.h>
#include <Components/mesh_renderer.h>

DEFINE_LOG_CATEGORY(MirageRenderer)

using namespace MirageEngine;
using namespace MirageEngine::Core::Math;

void Renderer::InitRenderer(MirageRHI::IRenderInterface::API _api)
{
	LOG(logInfo, MirageRenderer, "Initiating Renderer...");

	m_API = _api;
	InitWindow();
	InitRHI();

	LOG(logTemp, MirageRenderer, "Renderer successfully Initialized\n");
}

void Renderer::Shutdown()
{
	LOG(logInfo, MirageRenderer, "Shutting down Renderer...");

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
	LOG(logTemp, MirageRenderer, "Renderer successfully Shutdown\n");
}

bool Renderer::IsRunning() const
{
	return !glfwWindowShouldClose(m_Window);
}

void Renderer::BeginFrame()
{
	glfwPollEvents();

	uint32_t currentFBO = m_SwapChain->AcquireCurrentFBO();
	m_CommandBuffer->BeginDraw(m_RenderPass, currentFBO);
}

void Renderer::RenderScene(SceneGraph _sceneGraph)
{
	m_CommandBuffer->BindPipeline(m_Pipeline);
	m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);

	m_UniformBuffer->SetView(_sceneGraph.GetActiveCamera()->GetView());
	m_UniformBuffer->SetProjection(_sceneGraph.GetActiveCamera()->GetPerspective());

	for ( GameObject* root : _sceneGraph.GetRootObjects() ) 
	{ 
		for (GameObject* child : root->GetChildren())
		{
			MirageEngine::Core::Components::MeshRenderer* meshRenderer = child->GetComponent<MirageEngine::Core::Components::MeshRenderer>();
			if (meshRenderer) 
			{
				DrawMesh(meshRenderer, &child->Transform);
			}
		}
	}
}

void Renderer::RenderScreen()
{
	// End previous render pass and start new one for screen rendering
	m_CommandBuffer->EndDraw();
	m_CommandBuffer->BeginDraw(m_RenderPass, 0);

	m_CommandBuffer->BindPipeline(m_ScreenPipeline);
	m_CommandBuffer->ApplyPipelineParameters(m_ScreenPipeline);
	m_CommandBuffer->BindTextureFromID(0);

	m_CommandBuffer->Draw(3);
}

void Renderer::EndFrame()
{
	m_CommandBuffer->EndDraw();
	m_CommandBuffer->SubmitAndPresent(m_SwapChain, m_Surface);
}

uint32_t Renderer::GetFramebufferTextureID() const
{
	if (m_SwapChain)
	{
		return m_SwapChain->AcquireCurrentImage();
	}
	return 0;
}

void Renderer::InitWindow()
{
	if (!glfwInit()) {
		LOG(logFatal, MirageRenderer, "Failed to init GLFW!");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Width = 1920;
	m_Height = 1080;

	m_Window = glfwCreateWindow(m_Width, m_Height, "Mirage Engine", nullptr, nullptr);
	if (!m_Window) {
		glfwTerminate();
		LOG(logFatal, MirageRenderer, "Failed to create window!");
	}

	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG(logFatal, MirageRenderer, "Failed to init GLAD!");
	}

	glViewport(0, 0, m_Width, m_Height);

	LOG(logTemp, MirageRenderer, "Window Created");
}

void Renderer::InitRHI()
{
	// Select RHI API
	switch (m_API)
	{
	case MirageRHI::IRenderInterface::API::OPENGL:
		m_RenderInterface = new MirageRHI::OpenGLRenderInterface();
		LOG(logInfo, MirageRenderer, "Selected RHI API : OpenGL");
		break;
	default:
		LOG(logFatal, MirageRenderer, "Unsupported API");
	}

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
	LOG(logTemp, MirageRenderer, "All Classes successfully Instantiated");

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
	LOG(logTemp, MirageRenderer, "All Classes successfully Created");

	// Temp Pipeline Creation, will be replaced when material system is done !
	{
		m_PipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
		m_Pipeline = m_RenderInterface->InstantiatePipeline();

		m_PipelineParameters->Create(MirageRHI::VertexType::PBR, MirageRHI::CullingType::None, true);

		// Creation of single use shader objects for screen rendering pipeline
		MirageRHI::IShader* tempVertShader = m_RenderInterface->InstantiateShader();
		MirageRHI::IShader* tempFragShader = m_RenderInterface->InstantiateShader();

		tempVertShader->Create("Assets/Shaders/default.vert", MirageRHI::IShader::ShaderType::Vertex);
		tempFragShader->Create("Assets/Shaders/default.frag", MirageRHI::IShader::ShaderType::Fragment);
		m_Pipeline->Create(tempVertShader, tempFragShader, m_PipelineParameters);

		// Single use shader objects is destroyed after screen rendering pipeline creation
		tempVertShader->Destroy();
		tempFragShader->Destroy();
	}
}

void Renderer::DrawMesh(MirageEngine::Core::Components::MeshRenderer* _meshRenderer, MirageEngine::Core::Components::Transform* _meshTransform)
{
	if (!_meshRenderer->GetMesh() || !_meshRenderer->GetMaterial())
		return;

	_meshRenderer->GetMaterial()->Bind();
	m_UniformBuffer->UpdateUniformBuffer(m_Pipeline, _meshTransform->GetMatrix());
	m_CommandBuffer->BindBuffer(_meshRenderer->GetMesh()->GetVertexBuffer());
	m_CommandBuffer->BindBuffer(_meshRenderer->GetMesh()->GetIndexBuffer());
	m_CommandBuffer->Draw((uint32_t)_meshRenderer->GetMesh()->GetIndicesCount());
}