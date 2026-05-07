#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include <DebugDraw/debug_draw.h>

#include <Logs/log.h>
#include <OpenGL/openGL_render_interface.h>
#include <SceneGraph/scene_graph.h>
#include <Components/mesh_renderer.h>
#include <Components/transform.h> // needed because DrawMesh now receives a Transform reference
// #include "GameObject/game_object.h"

#include <Resources/material.h>
#include <Resources/material_instance.h>

// Light include
#include <Light/light_manager.h>
#include <Light/directional_light.h>
#include <Light/spot_light.h>
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
    m_UniformBuffer->Destroy();
    m_ScreenPipeline->Destroy();
    m_ScreenPipelineParameters->Destroy();
    m_RenderPass->Destroy();
    m_SwapChain->Destroy();
    m_Device->Destroy();
    m_Surface->Destroy();
    m_Instance->Destroy();

    m_RenderInterface.reset();
    m_CommandBuffer.reset();
    m_UniformBuffer.reset();
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
    uint32_t currentFBO = m_SwapChain->AcquireCurrentFBO();
    m_CommandBuffer->BeginDraw(m_RenderPass, currentFBO);
}

void Renderer::RenderScene(SceneGraph _sceneGraph)
{
    if (_sceneGraph.GetActiveCamera())
    {
        m_UniformBuffer->SetView(_sceneGraph.GetActiveCamera()->GetView());
        m_UniformBuffer->SetProjection(_sceneGraph.GetActiveCamera()->GetPerspective());
        m_UniformBuffer->SetCamPos(_sceneGraph.GetActiveCamera()->Transform().GetPosition());
    }

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Width = 1920;
    m_Height = 1080;

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
    m_UniformBuffer = m_RenderInterface->InstantiateUniformBuffer();
    m_CommandBuffer = m_RenderInterface->InstantiateCommandBuffer();

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
    m_UniformBuffer->Create();
    m_CommandBuffer->Create();

    LOG(logTemp, LogMirageRenderer, "All Classes successfully Created");
}

void MirageEngine::Renderer::ScreenPipelineCreation()
{
    m_ScreenPipelineParameters->Create(MirageRHI::CullingType::None, false);

    // Creation of single use shader objects for screen rendering pipeline
    std::shared_ptr<MirageRHI::IShader> screenVertShader = m_RenderInterface->InstantiateShader();
    std::shared_ptr<MirageRHI::IShader> screenFragShader = m_RenderInterface->InstantiateShader();

    screenVertShader->Create("Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
    screenFragShader->Create("Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);

    m_ScreenPipeline->Create(screenVertShader, screenFragShader, m_ScreenPipelineParameters);

    // Single use shader objects is destroyed after screen rendering pipeline creation
    screenVertShader->Destroy();
    screenFragShader->Destroy();
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
    m_CommandBuffer->BindPipeline(Debug_Draw.m_Pipeline);
    m_CommandBuffer->ApplyPipelineParameters(Debug_Draw.m_Pipeline);
    Debug_Draw.UpdateVertexBuffer();
    m_UniformBuffer->UpdateUniformBuffer(Debug_Draw.m_Pipeline, Matrix4::Identity());
    m_CommandBuffer->BindBuffer(Debug_Draw.m_VertexBufferObject);
    m_CommandBuffer->ApplyVertexLayout(Debug_Draw.m_VertexAttributeObject);
    m_CommandBuffer->Draw((uint32_t)Debug_Draw.m_Lines.size(), MirageRHI::DrawMode::Lines);
    Debug_Draw.Flush();
}

void MirageEngine::Renderer::ComputeLights(std::shared_ptr<MirageRHI::IPipeline> _pipeline)
{
    int directionalLightCounter = 0;
    int pointLightCounter = 0;
    int spotLightCounter = 0;

    std::vector<std::weak_ptr<MirageEngine::DirectionalLight>> dirList = Light_Manager.GetDirectionalLights();
    std::vector<std::weak_ptr<MirageEngine::PointLight>> pointList = Light_Manager.GetPointLights();
    std::vector<std::weak_ptr<MirageEngine::SpotLight>> spotList = Light_Manager.GetSpotLights();

    for (auto it = dirList.begin(); it != dirList.end(); ++it)
    {
        if (it->expired())
            continue;

        std::shared_ptr<MirageEngine::DirectionalLight> light = it->lock();
        std::string base = "DirectionalLights[" + std::to_string(directionalLightCounter) + "]";
        if (light)
        {
            _pipeline->SetVec3(base + ".direction", light->Direction);
            _pipeline->SetVec4(base + ".color", light->Color);
            _pipeline->SetFloat(base + ".intensity", light->Intensity);
        }

        directionalLightCounter++;
    }

    for (auto it = pointList.begin(); it != pointList.end(); ++it)
    {
        if (it->expired())
            continue;

        std::shared_ptr<MirageEngine::PointLight> light = it->lock();
        std::string base = "PointLights[" + std::to_string(pointLightCounter) + "]";
        if (light)
        {
            _pipeline->SetVec3(base + ".position", light->GetOwner()->GetPosition());
            _pipeline->SetVec4(base + ".color", light->Color);
            _pipeline->SetFloat(base + ".intensity", light->Intensity);
        }

        pointLightCounter++;
    }

    for (auto it = spotList.begin(); it != spotList.end(); ++it)
    {
        if (it->expired())
            continue;

        std::shared_ptr<SpotLight> light = it->lock();
        std::string base = "SpotLights[" + std::to_string(spotLightCounter) + "]";
        if (light)
        {
            _pipeline->SetVec3(base + ".position", light->Position);
            _pipeline->SetVec3(base + ".direction", light->Direction);
            _pipeline->SetVec4(base + ".color", light->Color);
            _pipeline->SetFloat(base + ".intensity", light->Intensity);
            _pipeline->SetFloat(base + ".innerCutoff", light->InnerCutoff);
            _pipeline->SetFloat(base + ".outerCutoff", light->OuterCutoff);
        }
        spotLightCounter++;
    }
    _pipeline->SetInt("DirectionalLightCount", directionalLightCounter);
    _pipeline->SetInt("PointLightCount", pointLightCounter);
    _pipeline->SetInt("SpotLightCount", spotLightCounter);

    LOG(logInfo, LogMirageRenderer, "Light count : ", directionalLightCounter + pointLightCounter + spotLightCounter);
}

void Renderer::DrawMesh(std::shared_ptr<MirageEngine::Core::Components::MeshRenderer> _meshRenderer, const MirageEngine::Core::Components::Transform& _meshTransform)
{
    if (!_meshRenderer->GetMesh() || !_meshRenderer->GetMaterial())
        return;

    std::shared_ptr<MirageRHI::IPipeline> pipeline = _meshRenderer->GetMaterial()->GetMaterial()->GetPipeline();

    m_CommandBuffer->BindPipeline(pipeline);
    m_CommandBuffer->ApplyPipelineParameters(pipeline);
    _meshRenderer->GetMaterial()->Bind();
    ComputeLights(pipeline);
    m_UniformBuffer->UpdateUniformBuffer(pipeline, _meshTransform.GetWorldMatrix());
    m_CommandBuffer->BindBuffer(_meshRenderer->GetMesh()->GetVertexBuffer());
    m_CommandBuffer->ApplyVertexLayout(_meshRenderer->GetMesh()->GetVertexLayout());
    m_CommandBuffer->BindBuffer(_meshRenderer->GetMesh()->GetIndexBuffer());
    m_CommandBuffer->Draw((uint32_t)_meshRenderer->GetMesh()->GetIndicesCount());
}
