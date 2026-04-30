// MirageEngine.cpp : Defines the entry point for the application.

#include "UI_wrapper.h"
#include "editor_app.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include <time_manager.h>
#include "Physics/physics_manager.h"
#include "Reflection/register_meta.h"

#include <Scripting/script_manager.h>

int main() 
{
	Application       application;
	Mirage::EditorApp editor;
	MirageEngine::Core::Scripting::ScriptManager::Instance().Init();
	MirageEngine::Core::Scripting::ScriptManager::Instance().ReloadScript();
	
	application.InitApp();
	MirageEngine::RegisterMeta().RegisterAllMeta();
	editor.Init(application.m_Renderer.GetWindowHandle(), &application.m_Renderer, application.GetSceneGraph());
	MirageEngine::TimeManager::Init();
	float dt ;
	while (application.m_Renderer.IsRunning())
	{
		glfwPollEvents();
		MirageEngine::TimeManager::Update();
		dt = MirageEngine::TimeManager::GetDeltaTime();
		MirageEngine::Core::Input::InputSystem::UpdateHeldInput();
		MirageEngine::Core::Physics::PhysicsManager::Instance().Update(dt);
		application.GetSceneGraph()->CallAllUpdate(dt);
		application.GetSceneGraph()->CallAllLateUpdate(dt);
		application.GetSceneGraph()->CallAllFixedUpdate(dt);
		application.GetSceneGraph()->UpdateAllWorldMatrices();
		application.m_Renderer.BeginFrame();
		application.m_Renderer.RenderScene(application.m_SceneGraph);
		application.m_Renderer.RenderScreen();
		editor.Update(application.m_Renderer.GetFramebufferTextureID());
		application.m_Renderer.EndFrame();
	}
	
	editor.Shutdown();
	application.ShutDown();
	MirageEngine::Core::Scripting::ScriptManager::Instance().ShutDown();
	return 0;
}
