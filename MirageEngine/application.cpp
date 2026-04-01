#include <application.h>
#include <OpenGL/openGL_render_interface.h>
#include <debug.h>
#include <Components/camera.h>
#include <Resources/resource_manager.h>
#include <Resources/mesh.h>
#include <Resources/material.h>
#include <Resources/texture.h>
#include <Resources/shader.h>
#include <Components/mesh_renderer.h>
#include <GameObject/game_object.h>

using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Math;

void Application::InitApp()
{
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
    DEBUG_LOG("Render Interface Created");
    ResourceManager::Instance()->Init(m_RenderInterface);

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

    Mesh* mesh = ResourceManager::Instance()->CreateMesh("cube", "Assets/Mesh/Cube.obj");
    DEBUG_LOG("Vertex and Index Data Created");

    // Create All Classes
    m_Instance->Create("Mirage");
    m_Surface->Create(m_Window, m_Width, m_Height);
    m_Device->Create();
    m_SwapChain->Create(2, m_Width, m_Height);
    m_RenderPass->Create(Vector4(0.f, 0.5f, 0.5f, 1.0f), 1.0f);

    ResourceManager::Instance()->CreateShader("defaultVert", "Assets/Shaders/default.vert", MirageRHI::IShader::ShaderType::Vertex);
    ResourceManager::Instance()->CreateShader("defaultFrag", "Assets/Shaders/default.frag", MirageRHI::IShader::ShaderType::Fragment);
    ResourceManager::Instance()->CreateShader("screenVert", "Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
    ResourceManager::Instance()->CreateShader("screenFrag", "Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);
    m_PipelineParameters->Create(MirageRHI::VertexType::PBR, MirageRHI::CullingType::None, true);
    m_ScreenPipelineParameters->Create(MirageRHI::VertexType::None, MirageRHI::CullingType::None, false);
    m_Pipeline->Create(ResourceManager::Instance()->Get<Shader>("defaultVert")->ShaderBind, ResourceManager::Instance()->Get<Shader>("defaultFrag")->ShaderBind, m_PipelineParameters);

    MirageRHI::IShader* Vert = new OpenGLShader();
    Vert->Create("Assets/Shaders/screen.vert", MirageRHI::IShader::ShaderType::Vertex);
    MirageRHI::IShader* Frag = new OpenGLShader();
    Frag->Create("Assets/Shaders/screen.frag", MirageRHI::IShader::ShaderType::Fragment);

    m_ScreenPipeline->Create(Vert, Frag, m_ScreenPipelineParameters);

    // m_ScreenPipeline->Create(ResourceManager::Instance()->Get<Shader>("screenVert")->ShaderBind, ResourceManager::Instance()->Get<Shader>("screenFrag")->ShaderBind, m_ScreenPipelineParameters);
    ResourceManager::Instance()->CreateTexture("Cat", "Assets/Textures/cat.png")->Bind();
    ResourceManager::Instance() -> CreateTexture("Mirage", "Assets/Textures/Mirage.png")->Bind();
    mesh->Bind(); // upload vertex and index data to GPU, then clear CPU-side data to save memory
    MeshRenderer one = MeshRenderer(mesh, ResourceManager::Instance()->CreateMaterial("DefaultMaterial"));
    MeshRenderer two = MeshRenderer(mesh, ResourceManager::Instance()->Get<Material>("DefaultMaterial"));
    one.Material->SetTexture("Cat", ResourceManager::Instance()->Get<Texture>("Cat"));
    two.Material->SetTexture("Mirage", ResourceManager::Instance()->Get<Texture>("Mirage"));
    two.SetPos(Vector3(2.f, 2.f, 0.f));

    Vector3 rotation = Vector3(0.f, 180.f, 0.f);
    two.SetRotation(Quaternion(rotation));
    m_meshRenderer.push_back(std::make_shared<MeshRenderer>(one));
    m_meshRenderer.push_back(std::make_shared<MeshRenderer>(two));
    Material* material = nullptr;
    if (ResourceManager::Instance()->TryGet<Material>("DefaultMaterial", material)) { material->SetShader(m_Pipeline); }
    m_UniformBuffer->Create();
    m_CommandBuffer->Create();
    DEBUG_LOG("All Classes Created");
}

void Application::InitScene() { m_EngineCamera = new MirageEngine::Core::Components::Camera(1920, 1080, 90.f, 0.1f, 100.f, Vector3(0.f, 0.f, -3.f)); }

void Application::InitInput()
{
    // creating editorMapping
    m_EditorMapping                  = new InputMapping();
    m_EditorMapping->Map[GLFW_KEY_W] = InputBind{};
    m_EditorMapping->Map[GLFW_KEY_A] = InputBind{};
    m_EditorMapping->Map[GLFW_KEY_S] = InputBind{};
    m_EditorMapping->Map[GLFW_KEY_D] = InputBind{};
    m_EditorMapping->Map[GLFW_KEY_R] = InputBind{};
    m_EditorMapping->Map[GLFW_KEY_Q] = InputBind{};
    m_EditorMapping->Map[GLFW_KEY_E] = InputBind{};

    m_PlayMapping                  = new InputMapping();
    m_PlayMapping->Map[GLFW_KEY_W] = InputBind{};
    m_PlayMapping->Map[GLFW_KEY_A] = InputBind{};
    m_PlayMapping->Map[GLFW_KEY_S] = InputBind{};
    m_PlayMapping->Map[GLFW_KEY_D] = InputBind{};
    m_PlayMapping->Map[GLFW_KEY_R] = InputBind{};

    // add random function test member of current class
    m_EditorMapping->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
    m_EditorMapping->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
    m_EditorMapping->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
    m_EditorMapping->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EditorPrintInput(info); }));
    m_EditorMapping->Map[GLFW_KEY_R].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { StartPlay(info); }));

    m_PlayMapping->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
    m_PlayMapping->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
    m_PlayMapping->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
    m_PlayMapping->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { PrintInput(info); }));
    m_PlayMapping->Map[GLFW_KEY_R].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) { EndPlay(info); }));

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
    MirageEngine::GameObject* obj  = new MirageEngine::GameObject(root, "Commelin");
    MirageEngine::GameObject* obj2 = new MirageEngine::GameObject(obj, "Diego");
    root->AddChild(obj);
    obj->AddChild(obj2);

    obj->GetParent() ? obj->GetParent()->Transform.SetPosition(Vector3(0, 67, 0)) : void();
    obj2->GetParent() ? obj2->GetParent()->Transform.SetPosition(Vector3(0, 67, 0)) : void();
}

void Application::PrintInput(MirageEngine::Core::Input::CallbackInputInfo info)
{
    if (info.Pressed) { printf("Pressed\n"); }
    if (info.Released) { printf("Released\n"); }
}

void Application::EditorPrintInput(MirageEngine::Core::Input::CallbackInputInfo info)
{
    if (info.Pressed) { printf("Editor Pressed\n"); }
    if (info.Released) { printf("Editor Released\n"); }
}

void Application::StartPlay(MirageEngine::Core::Input::CallbackInputInfo info)
{
    if (info.Pressed)
    {
        printf("Editor -> Play\n");
        InputSystem::SetCurrentMapping(m_PlayMapping);
    }
}

void Application::EndPlay(MirageEngine::Core::Input::CallbackInputInfo info)
{
    if (info.Pressed)
    {
        printf("Play -> Editor\n");
        InputSystem::SetCurrentMapping(m_EditorMapping);
    }
}

bool Application::IsRunning() const { return !glfwWindowShouldClose(m_Window); }

void Application::BeginFrame()
{
    glfwPollEvents();
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(m_Window, true);

    InputSystem::UpdateHeldInput();
    uint32_t currentFBO = m_SwapChain->AcquireCurrentFBO();
    m_CommandBuffer->BeginRenderPass(m_RenderPass, currentFBO);
    m_CommandBuffer->BeginDraw();
}

void Application::RenderScene()
{
    // using namespace MirageEngine::Core::Resources;

    m_CommandBuffer->BindPipeline(m_Pipeline);
    m_CommandBuffer->ApplyPipelineParameters(m_Pipeline);
    m_UniformBuffer->SetView(m_EngineCamera->GetView());
    m_UniformBuffer->SetProjection(m_EngineCamera->GetPerspective());
    m_UniformBuffer->UpdateUniformBuffer(m_Pipeline, Matrix4::CreateTransformMatrix(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1))); // view/proj

    for (std::shared_ptr<MirageEngine::Core::Components::MeshRenderer> meshRenderer : m_meshRenderer) { meshRenderer->Render(m_CommandBuffer, m_Pipeline, m_UniformBuffer); }

    m_CommandBuffer->EndRenderPass();

    m_CommandBuffer->BeginRenderPass(m_RenderPass, 0);
    m_CommandBuffer->BindPipeline(m_ScreenPipeline);
    m_CommandBuffer->ApplyPipelineParameters(m_ScreenPipeline);
    m_CommandBuffer->BindTextureFromID(m_SwapChain->AcquireCurrentImage());
    m_CommandBuffer->Draw(3);
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
    delete m_EditorMapping;
    delete m_PlayMapping;
    delete m_EngineCamera;
}
