// MirageEngine.cpp : Defines the entry point for the application.

#include "UI_wrapper.h"
#include "editor_app.h"
#include "Glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include <time_manager.h>
#include "Physics/physics_manager.h"
#include "Reflection/register_meta.h"
#include <Scripting/script_manager.h>
#include <Logs/log.h>
#include <DebugDraw/debug_draw.h>
#include <Components/camera.h>

int main()
{
    Loggers::EnableImmediateFlush(true);   // Enable immediate flushing of log messages for real-time output during initialization
    Loggers::EnableFileWriting(true);      // Enable writing logs to file during initialization to capture any issues that may occur before the main loop starts.
    Loggers::EnableTimeOnEveryLog(false);  // Disable time show as it take verbosal the log.
    Loggers::EnableFileNameAndLine(false); // Disable file name and line of the log , can be enable if necessary.
    Application application;               // Temp, will be replaced by Engine Handler
    Mirage::EditorApp editor;
    // --- Initialization ---
    MirageEngine::Core::Scripting::ScriptManager::Instance().Init();
    std::cout << "Context address: " << &entt::locator<entt::meta_ctx>::value_or() << std::endl;
    MirageEngine::Core::Scripting::ScriptManager::Instance().SetMetaContext(entt::locator<entt::meta_ctx>::handle());
    application.InitApp();
    MirageEngine::RegisterMeta().RegisterAllMeta(entt::locator<entt::meta_ctx>::handle());
    editor.Init(application.GetWindowHandle(), application.GetRenderer(), application.GetSceneGraph());
    
	MirageEngine::TimeManager::Init();
	while (application.IsRunning())
	{
		glfwPollEvents();
		MirageEngine::TimeManager::Update();
		MirageEngine::Core::Input::InputSystem::UpdateHeldInput();
		if (editor.IsPlaying() && !editor.IsPaused())
		{
			MirageEngine::Core::Physics::PhysicsManager::Instance().Update(MirageEngine::TimeManager::GetDeltaTime());
			application.RuntimeUpdate(MirageEngine::TimeManager::GetDeltaTime());
		}
		application.EditorUpdate();
		application.BeginRender(editor.GetEditorCamera());

		// Display the editor panels and pass the framebuffer texture ID to the viewport for rendering the scene
		editor.OnUpdate(application.GetFramebufferTextureID());
		application.EndRender();
	}

	// --- Shutdown ---
	editor.Shutdown();
	application.ShutDown();
	return 0;
}
