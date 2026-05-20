#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include <Resources/resource_manager.h>
#include <Camera/i_camera.h>
#include <DebugDraw/debug_draw.h>

#include <Logs/log.h>
#include <OpenGL/openGL_render_interface.h>
#include <SceneGraph/scene_graph.h>
#include <Components/mesh_renderer.h>
#include <Components/transform.h> // needed because DrawMesh now receives a Transform reference

#include <Resources/material.h>
#include <Resources/material_instance.h>

// Light include
#include <Light/light_manager.h>
#include <Light/directional_light.h>
#include <Light/point_light.h>
#include <Light/i_light.h>

DEFINE_LOG_CATEGORY(LogMirageRenderer)

using namespace MirageEngine;
using namespace MirageEngine::Core::Math;

void Renderer::InitRenderer(MirageRHI::IRenderInterface::API _api)
{
	LOG(logInfo, LogMirageRenderer, "Initiating Renderer...");

	m_API = _api;
	InitWindow();
	InitRHI();

	LOG(logTemp, LogMirageRenderer, "Renderer successfully Initialized\n");
}

void Renderer::Shutdown()
{
	LOG(logInfo, LogMirageRenderer, "Shutting down Renderer...");

	m_CommandBuffer->Destroy();
	m_PerFrameUBO->Destroy();
	m_PerObjectUBO->Destroy();
	m_LightUBO->Destroy();
	m_ScreenPipeline->Destroy();
	m_ScreenPipelineParameters->Destroy();
	m_RenderPass->Destroy();
	m_SwapChain->Destroy();
	m_Device->Destroy();
	m_Surface->Destroy();
	m_Instance->Destroy();

	m_RenderInterface.reset();
	m_CommandBuffer.reset();
	m_PerFrameUBO.reset();
	m_PerObjectUBO.reset();
	m_LightUBO.reset();
	m_ScreenPipeline.reset();
	m_ScreenPipelineParameters.reset();
	m_RenderPass.reset();
	m_SwapChain.reset();
	m_Device.reset();
	m_Surface.reset();
	m_Instance.reset();

	m_RenderInterface.reset();

	glfwTerminate();
	LOG(logTemp, LogMirageRenderer, "Renderer successfully Shutdown\n");
}

bool Renderer::IsRunning() const { return !glfwWindowShouldClose(m_Window); }

void Renderer::BeginFrame()
{
	m_SwapChain->IsScreenResized(m_Surface);
	uint32_t currentFBO = m_SwapChain->AcquireCurrentFBO();
	m_CommandBuffer->BeginDraw(m_RenderPass, currentFBO);
}

void MirageEngine::Renderer::SetupFrameUBO(std::shared_ptr<Core::ICamera> _camera)
{
	if (_camera)
	{
		MirageRHI::PerFrameData frame{ _camera->GetView(), _camera->GetPerspective(), _camera->GetPosition(), 0.0f };
		m_PerFrameUBO->Upload(&frame, sizeof(MirageRHI::PerFrameData));
		Debug_Draw.UpdateUBO(&frame, sizeof(MirageRHI::PerFrameData));
	}
}

void Renderer::RenderScene(SceneGraph _sceneGraph)
{
	ComputeLights();

	for (std::shared_ptr<GameObject> root : _sceneGraph.GetRootObjects())
	{
		RenderObjectRecursive(root);
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
	if (!glfwInit())
	{
		LOG(logFatal, LogMirageRenderer, "Failed to init GLFW!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Width = 1280;
	m_Height = 720;

	m_Window = glfwCreateWindow(m_Width, m_Height, "Mirage Engine", nullptr, nullptr);
	if (!m_Window)
	{
		glfwTerminate();
		LOG(logFatal, LogMirageRenderer, "Failed to create window!");
	}

	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG(logFatal, LogMirageRenderer, "Failed to init GLAD!");
	}

	glViewport(0, 0, m_Width, m_Height);

	LOG(logTemp, LogMirageRenderer, "Window Created");
}

void Renderer::InitRHI()
{
	// Select RHI API
	APISelection();

	// Instantiate All Classes
	ClassInstantiation();

	// Create All Classes
	ClassCreation();
}

void MirageEngine::Renderer::APISelection()
{
	switch (m_API)
	{
	case MirageRHI::IRenderInterface::API::OPENGL:
		m_RenderInterface = std::make_shared<MirageRHI::OpenGLRenderInterface>();
		LOG(logInfo, LogMirageRenderer, "Selected RHI API : OpenGL");
		break;
	default:
		LOG(logFatal, LogMirageRenderer, "Unsupported API");
	}
}

void MirageEngine::Renderer::ClassInstantiation()
{
	m_Instance = m_RenderInterface->InstantiateInstance();
	m_Surface = m_RenderInterface->InstantiateSurface();
	m_Device = m_RenderInterface->InstantiateDevice();
	m_SwapChain = m_RenderInterface->InstantiateSwapChain();
	m_RenderPass = m_RenderInterface->InstantiateRenderPass();
	m_ScreenPipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
	m_ScreenPipeline = m_RenderInterface->InstantiatePipeline();
	m_PerFrameUBO = m_RenderInterface->InstantiateUniformBuffer();
	m_PerObjectUBO = m_RenderInterface->InstantiateUniformBuffer();
	m_LightUBO = m_RenderInterface->InstantiateUniformBuffer();
	m_CommandBuffer = m_RenderInterface->InstantiateCommandBuffer();
    m_SkySpherePipelineParameters = m_RenderInterface->InstantiatePipelineParameters();
    m_SkySpherePipeline = m_RenderInterface->InstantiatePipeline();

	LOG(logTemp, LogMirageRenderer, "All Classes successfully Instantiated");
}

void MirageEngine::Renderer::ClassCreation()
{
	m_Instance->Create("Mirage");
	m_Surface->Create(m_Window, m_Width, m_Height);
	m_Device->Create();
	m_SwapChain->Create(2, m_Width, m_Height);
	m_RenderPass->Create(Vector4(0.f, 0.5f, 0.5f, 1.0f), 1.0f);
	ScreenPipelineCreation();
	SkySpherePipelineCreation();
	m_PerFrameUBO->Create(MirageRHI::UBOBinding::PER_FRAME, sizeof(MirageRHI::PerFrameData));
	m_PerObjectUBO->Create(MirageRHI::UBOBinding::PER_OBJECT, sizeof(MirageRHI::PerObjectData));
	m_LightUBO->Create(MirageRHI::UBOBinding::LIGHTS, sizeof(MirageRHI::LightUBOData));
	m_CommandBuffer->Create();

	LOG(logTemp, LogMirageRenderer, "All Classes successfully Created");
}

void MirageEngine::Renderer::ScreenPipelineCreation()
{
    m_ScreenPipelineParameters->Create(MirageRHI::CullingType::NONE, false);

	// Creation of single use shader objects for screen rendering pipeline
	std::shared_ptr<MirageRHI::IShader> screenVertShader = m_RenderInterface->InstantiateShader();
	std::shared_ptr<MirageRHI::IShader> screenFragShader = m_RenderInterface->InstantiateShader();

    screenVertShader->Create("Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::VERTEX);
    screenFragShader->Create("Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::FRAGMENT);

    m_ScreenPipeline->Create(screenVertShader, screenFragShader, m_ScreenPipelineParameters);

    // Single use shader objects is destroyed after screen rendering pipeline creation
    screenVertShader->Destroy();
    screenFragShader->Destroy();
}

void MirageEngine::Renderer::SkySpherePipelineCreation()
{
	std::shared_ptr<MirageRHI::IShader> skyBoxVertShader = m_RenderInterface->InstantiateShader();
	std::shared_ptr<MirageRHI::IShader> skyBoxFragShader = m_RenderInterface->InstantiateShader();

	m_SkySpherePipelineParameters->Create(MirageRHI::CullingType::NONE, false);

	skyBoxVertShader->Create("Assets/Shaders/sky_sphere.vert", MirageRHI::IShader::ShaderType::VERTEX);
	skyBoxFragShader->Create("Assets/Shaders/sky_sphere.frag", MirageRHI::IShader::ShaderType::FRAGMENT);

	m_SkySpherePipeline->Create(skyBoxVertShader, skyBoxFragShader, m_SkySpherePipelineParameters, { { "PerFrame",   MirageRHI::UBOBinding::PER_FRAME } });

	skyBoxVertShader->Destroy();
	skyBoxFragShader->Destroy();
}

void Renderer::RenderObjectRecursive(std::shared_ptr<MirageEngine::GameObject> _obj)
{
	if (!_obj)
		return;

	std::shared_ptr<Core::Components::MeshRenderer> meshRenderer = _obj->GetComponent<Core::Components::MeshRenderer>();
	if (meshRenderer)
	{
		// GameObject stores Transform as a value member `Transform` (not as a component in m_Components).
		// Use that value directly.
		DrawMesh(meshRenderer, _obj->Transform);
	}

	for (std::shared_ptr<MirageEngine::GameObject> child : _obj->GetChildren())
	{
		RenderObjectRecursive(child);
	}
}

void MirageEngine::Renderer::RenderLine()
{
    Debug_Draw.Flush(m_CommandBuffer);
}

void MirageEngine::Renderer::RenderSkySphere()
{
	MirageEngine::Core::Resources::ResourceManager& rm = MirageEngine::Core::Resources::ResourceManager::Instance();
    std::shared_ptr<MirageEngine::Core::Resources::Mesh> skySphere = rm.GetMesh("sphere");
    
    m_CommandBuffer->BindPipeline(m_SkySpherePipeline);
    m_CommandBuffer->ApplyPipelineParameters(m_SkySpherePipeline);
    m_CommandBuffer->BindBuffer(skySphere->GetVertexBuffer());
    m_CommandBuffer->ApplyVertexLayout(skySphere->GetVertexLayout());
    m_CommandBuffer->BindBuffer(skySphere->GetIndexBuffer());
    m_CommandBuffer->Draw((uint32_t)skySphere->GetIndicesCount());
}

void Renderer::ComputeLights()
{
	MirageRHI::LightUBOData data{};

	std::vector<std::weak_ptr<DirectionalLight>> dirList = LIGHT_MANAGER.GetDirectionalLights();
	std::vector<std::weak_ptr<PointLight>> pointList = LIGHT_MANAGER.GetPointLights();

	for (std::weak_ptr<DirectionalLight>& weak : dirList)
	{
		if (weak.expired())
			continue;
		std::shared_ptr<DirectionalLight> light = weak.lock();
		MirageRHI::DirectionalLightData& d = data.dirLights[data.dirCount++];
		d.Color = light->GetColor();
		d.Direction = light->GetDirection();
		d.Intensity = light->GetIntensity();
	}

	for (std::weak_ptr<PointLight>& weak : pointList)
	{
		if (weak.expired())
			continue;
		std::shared_ptr<PointLight> light = weak.lock();
		MirageRHI::PointLightData& p = data.pointLights[data.pointCount++];
		p.Color = light->GetColor();
		p.Position = light->GetParentWorldPosition();
		p.Intensity = light->GetIntensity();
	}

	m_LightUBO->Upload(&data, sizeof(MirageRHI::LightUBOData));
}

void Renderer::DrawMesh(std::shared_ptr<MirageEngine::Core::Components::MeshRenderer> _meshRenderer, const MirageEngine::Core::Components::Transform& _meshTransform)
{
	if (!_meshRenderer->GetMesh() || !_meshRenderer->GetMaterial())
		return;

	std::shared_ptr<MirageRHI::IPipeline> pipeline = _meshRenderer->GetMaterial()->GetMaterial()->GetPipeline();
	MirageRHI::PerObjectData obj{ _meshTransform.GetWorldMatrix(), _meshTransform.GetWorldMatrix().Inverse().Transpose() };

	m_CommandBuffer->BindPipeline(pipeline);
	m_CommandBuffer->ApplyPipelineParameters(pipeline);
	_meshRenderer->GetMaterial()->Bind();
	m_PerObjectUBO->Upload(&obj, sizeof(MirageRHI::PerObjectData));
	m_CommandBuffer->BindBuffer(_meshRenderer->GetMesh()->GetVertexBuffer());
	m_CommandBuffer->ApplyVertexLayout(_meshRenderer->GetMesh()->GetVertexLayout());
	m_CommandBuffer->BindBuffer(_meshRenderer->GetMesh()->GetIndexBuffer());
	m_CommandBuffer->Draw((uint32_t)_meshRenderer->GetMesh()->GetIndicesCount());
}
