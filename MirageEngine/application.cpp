#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include <application.h>

#include <renderer.h>
#include <SceneGraph/scene_graph.h>

// Resources
#include <Resources/resource_manager.h>
#include <Resources/mesh.h>
#include <Resources/material.h>
#include <Resources/material_instance.h>
#include <Resources/texture.h>
#include <Resources/shader.h>
#include <RHI/ubo_structs.h>

// GameObject & Components
#include <GameObject/game_object.h>
#include <Input/input_system.h>
#include <Components/box_collider.h>
#include <Components/capsule_collider.h>
#include <Components/sphere_collider.h>
#include <Components/rigidbody.h>
#include <Components/camera.h>
#include <Components/mesh_renderer.h>

// Physics and physics Components
#include <Physics/physics_manager.h>

// Script Manager
#include <Scripting/script_manager.h>

#include <DebugDraw/debug_draw.h>

// Light include
#include <Light/light_manager.h>
#include <Light/directional_light.h>
#include <Light/spot_light.h>
#include <Light/point_light.h>
#include <Light/i_light.h>
#include <i_uniform_buffer.h>

// AI
#include <AI/influence_grid.h>
#include <AI/ai_action.h>
#include <Assets/Scripts/ActionMoveTo.h> 
#include <Components/nav_agent.h>
#include <Components/utility_agent.h>
#include <Components/influence_emitter.h>

using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Resources;

Application* Application::s_Instance = nullptr;

void Application::InitApp()
{
	s_Instance = this;

	m_Renderer.InitRenderer();

	MirageEngine::Core::Resources::ResourceManager::Instance().Init(m_Renderer.GetRenderInterfaceHandle());
	MirageEngine::Core::Resources::ResourceManager::Instance().PreLoadResources();
	MirageEngine::Core::Physics::PhysicsManager::Instance().Initialize();
	Debug_Draw.Init(m_Renderer.GetRenderInterfaceHandle());

	InitScene();
	InitInput();
	InitGameObject();
}

void Application::InitScene()
{
	m_EngineCamera = std::make_shared<Camera>(1920, 1080, 90.f, 0.1f, 100.f, Vector3(0.f, 0.f, -3.f));
}

void Application::InitInput()
{
	// creating editorMapping
	m_EditorMapping = std::make_shared<InputMapping>();

	// add random function test member of current class
	// m_EditorMapping->Add(GLFW_KEY_W, this, &Application::EditorPrintInput); // template

	// init with previous created map
	InputSystem::Initialize(m_EditorMapping);
	// get and set callback
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetKeyCallback(window, InputSystem::KeyCallback);
	glfwSetMouseButtonCallback(window, InputSystem::MouseButtonCallback);
	glfwSetCursorPosCallback(window, InputSystem::CursorPosCallback);
}

using namespace MirageEngine::Core::Resources;
MeshRenderer* meshRenderer;
MeshRenderer* meshRenderer2;
Camera* camera;

void Application::InitGameObject()
{
	using namespace MirageEngine;
	using namespace MirageEngine::Core::Physics;

	PhysicsManager& physics = PhysicsManager::Instance();

    // =========================
    //  Physics setup
    // =========================
    std::shared_ptr<SphereCollider> dynamicCollider = std::make_shared<SphereCollider>();
    std::shared_ptr<Rigidbody> dynamicRB = std::make_shared<Rigidbody>(dynamicCollider.get(), ERigidBodyType::DYNAMIC);

    dynamicRB->SetCollisionGroup(ECollisionGroup::GROUP_1);
    dynamicRB->SetCollisionMask(ECollisionGroup::GROUP_1 | ECollisionGroup::GROUP_2 | ECollisionGroup::GROUP_3);
    dynamicRB->SetCollisionResponse(ECollisionResponse::BLOCK);

    std::shared_ptr<BoxCollider> staticCollider = std::make_shared<BoxCollider>();
    std::shared_ptr<Rigidbody> staticRB = std::make_shared<Rigidbody>(staticCollider.get(), ERigidBodyType::STATIC);

    staticRB->SetCollisionGroup(ECollisionGroup::GROUP_2);
    staticRB->SetCollisionMask(ECollisionGroup::ALL);

	// =========================
	//  Scene graph
	// =========================
	std::shared_ptr<GameObject> root = m_SceneGraph.CreateGameObject("Root", nullptr);
	std::shared_ptr<GameObject> cameraGO = m_SceneGraph.CreateGameObject("Commelin", root);
	std::shared_ptr<GameObject> floorGO = m_SceneGraph.CreateGameObject("Lin", root);
	std::shared_ptr<GameObject> child1 = m_SceneGraph.CreateGameObject("Monin", floorGO);
	std::shared_ptr<GameObject> child2 = m_SceneGraph.CreateGameObject("Tang", child1);
	ResourceManager::Instance().GetMesh("cube")->Bind();
	ResourceManager::Instance().GetMesh("sphere")->Bind();
	ResourceManager::Instance().GetMesh("cylinder")->Bind();
	ResourceManager::Instance().GetMesh("plane")->Bind();
	ResourceManager::Instance().GetMesh("cone")->Bind();

	// =========================
	//  Script
	// =========================
	MirageEngine::Core::Scripting::ScriptManager& scriptManager = MirageEngine::Core::Scripting::ScriptManager::Instance();

    if (const char* name = scriptManager.GetScriptName(0))
    {
        std::unique_ptr<Core::Scripting::IScript, Core::Scripting::ScriptManager::DllDeleter> script = scriptManager.CreateScriptByName(name);
        floorGO->AddComponent(std::move(script));
    }
    // =========================
    //  Camera object
    // =========================
    cameraGO->AddComponent<Camera>();
    cameraGO->AddComponent<InputMapping>();
    cameraGO->AddComponent<MeshRenderer>();
    cameraGO->AddComponent(dynamicCollider);
    cameraGO->AddComponent<Rigidbody>(dynamicRB);

	std::shared_ptr<Camera> cam = cameraGO->GetComponent<Camera>();
	std::shared_ptr<InputMapping> input = cameraGO->GetComponent<InputMapping>();
	std::shared_ptr<MeshRenderer> renderer1 = cameraGO->GetComponent<MeshRenderer>();

	m_SceneGraph.SetEditorCamera(cam);

	input->RegisterInput("Reload", GLFW_KEY_R);
	std::function<void(CallbackInputInfo)> func = [](CallbackInputInfo info)
	{
		if (info.Pressed)
		{
			MirageEngine::Core::Scripting::ScriptManager::Instance().ReloadScript();
		}
	};

	input->RegisterAction("Reload", func);

	cam->SetNear(0.1f);
	cam->SetFar(100.f);
	cam->SetFov(60.f);
	cam->SetWidth(1920);
	cam->SetHeight(1080);

	InputSystem::SetCurrentMapping(input);

	// =========================
	//  Floor object
	// =========================
	floorGO->AddComponent<BoxCollider>(staticCollider);
	floorGO->AddComponent<Rigidbody>(staticRB);
	floorGO->AddComponent<MeshRenderer>();

	std::shared_ptr<MeshRenderer> renderer2 = floorGO->GetComponent<MeshRenderer>();

	Quaternion rotationQ(Vector3(1.f, 0.f, 0.f), PI / 12.f);

	floorGO->SetPosition(Vector3(0, -3, 0));
	floorGO->SetRotation(rotationQ);
	// floorGO->SetScale(Vector3(10, 1, 10));

	// =========================
	//  Light object
	// =========================

	std::shared_ptr<PointLight> light1 = child2->AddComponent<PointLight>();
	light1->SetIntensity(0.5f);
	light1->SetColor({ 1, 1, 1, 1 });

	std::shared_ptr<DirectionalLight> light2 = child2->AddComponent<DirectionalLight>();
	light2->SetIntensity(0.5f);
	light2->SetColor({ 1, 1, 1, 1 });

	// =========================
	//  Resources
	// =========================
	ResourceManager& rm = ResourceManager::Instance();

	std::shared_ptr<Texture> texCat = rm.GetTexture("cat");
	std::shared_ptr<Texture> texMirage = rm.GetTexture("Mirage");

    std::shared_ptr<Material> mat = rm.CreateMaterial(
        "default", "Assets/Shaders/light.vert", "Assets/Shaders/light.frag", 0,
        {
            { "PerFrame",   MirageRHI::UBOBinding::PER_FRAME },
            { "PerObject",  MirageRHI::UBOBinding::PER_OBJECT },
            { "LightBlock", MirageRHI::UBOBinding::LIGHTS },
    });

	std::shared_ptr<MaterialInstance> mat1 = rm.CreateMaterialInstance("TestMaterialinstance", mat);
	mat1->SetTexture("normalMap", texCat);

	// Set PBR Setting
	mat1->SetVec3("albedo", { 1, 1, 1 });
	mat1->SetFloat("metallic", 1);
	mat1->SetFloat("roughness", 1);
	mat1->SetFloat("ao", 1);

	std::shared_ptr<MaterialInstance> mat2 = rm.CreateMaterialInstance("TestMaterialinstance2", mat);

	texCat->Bind();
	mat2->SetTexture("normalMap", texMirage);

	renderer1->SetMaterial(mat1);
	renderer2->SetMaterial(mat2);

	std::shared_ptr<Mesh> cubeMesh = rm.GetMesh("cube");
	cubeMesh->Bind();

	renderer1->SetMesh(cubeMesh);
	renderer2->SetMesh(cubeMesh);

	// =========================
	//  Root transform
	// =========================
	root->Transform.SetPosition(Vector3(0, 0, -10));

	// =========================
	//  Cache members
	// =========================
	camera = cam.get();
	meshRenderer = renderer1.get();
	meshRenderer2 = renderer2.get();

	// ========================================================
	//  AI SYSTEM DEMO SETUP (HARDCODED)
	// ========================================================
	
	std::shared_ptr<GameObject> gridGO = m_SceneGraph.CreateGameObject("AI_Grid");
	auto globalGrid = gridGO->AddComponent<InfluenceGrid>();
	globalGrid->SetWidth(55.0f);
	globalGrid->SetHeight(55.0f);
	globalGrid->SetCellSize(1.0f);
	globalGrid->AddChannel("Fire");
	globalGrid->Bake();

	std::shared_ptr<GameObject> aiAgentGO = m_SceneGraph.CreateGameObject("Pompier");
	aiAgentGO->SetPosition(Vector3(5.0f, 0.0f, 5.0f));

	auto aiRenderer = aiAgentGO->AddComponent<MeshRenderer>();
	aiRenderer->SetMesh(rm.GetMesh("cube"));
	aiRenderer->SetMaterial(mat1);


	auto nav = aiAgentGO->AddComponent<MirageEngine::Core::Components::NavAgent>();
	auto utility = aiAgentGO->AddComponent<UtilityAgent>();

	AI::NavProfile myProfile;
	myProfile.InfluenceWeights.push_back(100.0f);
	nav->SetProfile(myProfile);

	auto script = scriptManager.CreateScriptByName("ActionMoveTo");
	aiAgentGO->AddComponent(std::move(script));
	auto moveAction = aiAgentGO->GetComponent<ActionMoveTo>();

	if (moveAction) {
		moveAction->SetTarget(Vector3(25.0f, 0.0f, 25.0f));

		Consideration fireRule;
		fireRule.source = VariableSource::Grid;
		fireRule.variableName = "Fire";
		fireRule.responseCurve.SetType(AI::ResponsiveCurve::CurveType::LINEAR);
		fireRule.responseCurve.SetSlope(1.0f);

		moveAction->GetConsiderations().push_back(fireRule);
		utility->AddAction(moveAction.get());
	}

	std::shared_ptr<GameObject> fireGO = m_SceneGraph.CreateGameObject("Feu");
	fireGO->SetPosition(Vector3(15.0f, 0.0f, 15.0f)); 
	auto emitter = fireGO->AddComponent<InfluenceEmitter>();
	emitter->SetTargetGrid(globalGrid.get());
	emitter->SetInfluenceType("Fire");
	emitter->SetStrength(100.0f);
	emitter->SetRadius(10.0f);
}

void Application::RuntimeUpdate(float _dt)
{
	m_AccumulatedTime += _dt;

	m_SceneGraph.CallAllOnStart();

	// FixedUpdate is called in a loop to ensure that it is called at a consistent rate, regardless of the frame rate.
	while (m_AccumulatedTime >= m_FixedDeltaTime)
	{
		m_SceneGraph.CallAllFixedUpdate(m_FixedDeltaTime);
		m_AccumulatedTime -= m_FixedDeltaTime;
	}

	m_SceneGraph.CallAllUpdate(_dt);
	m_SceneGraph.CallAllLateUpdate(_dt);
}

void Application::EditorUpdate()
{
	m_SceneGraph.CallAllDrawLine();
	m_SceneGraph.UpdateAllWorldMatrices();
}

void Application::BeginRender(std::shared_ptr<MirageEngine::Core::ICamera> camera)
{
	m_Renderer.BeginFrame();
	m_Renderer.RenderScene(m_SceneGraph, camera);
	m_Renderer.RenderLine();
	m_Renderer.RenderScreen();
}
void Application::EndRender()
{
	m_Renderer.EndFrame();
}
void Application::ShutDown()
{
	m_SceneGraph.ClearScene();
	m_EditorMapping.reset();
	m_PlayMapping.reset();
	m_EngineCamera.reset();
	MirageEngine::Core::Input::InputSystem::Shutdown();
	MirageEngine::Core::Resources::ResourceManager::Instance().Shutdown();
	MirageEngine::Core::Physics::PhysicsManager::Instance().Shutdown();
	Debug_Draw.ShutDown();
	m_Renderer.Shutdown();
	MirageEngine::Core::Scripting::ScriptManager::Instance().ShutDown();
}
