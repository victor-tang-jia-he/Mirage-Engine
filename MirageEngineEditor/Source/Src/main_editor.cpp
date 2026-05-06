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
    application.InitApp();
    MirageEngine::RegisterMeta().RegisterAllMeta();
    editor.Init(application.m_Renderer.GetWindowHandle(), &application.m_Renderer, application.GetSceneGraph());
    MirageEngine::TimeManager::Init();
    Debug_Draw.Init(application.m_Renderer.GetRenderInterfaceHandle());
    float dt;
    while (application.m_Renderer.IsRunning())
    {
        Debug_Draw.Line({ 0, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0, 1 }); // ligne rouge
        Debug_Draw.Line({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 1, 0, 1 }); // ligne verte

        glfwPollEvents();
        MirageEngine::TimeManager::Update();
        dt = MirageEngine::TimeManager::GetDeltaTime();
        MirageEngine::Core::Input::InputSystem::UpdateHeldInput();
        if (editor.IsPlaying() && !editor.IsPaused())
        {
            MirageEngine::Core::Physics::PhysicsManager::Instance().Update(MirageEngine::TimeManager::GetDeltaTime());
            application.GetSceneGraph()->CallAllUpdate(dt);
            application.GetSceneGraph()->CallAllLateUpdate(dt);
            application.GetSceneGraph()->CallAllFixedUpdate(dt);
        }
        application.GetSceneGraph()->UpdateAllWorldMatrices();

        // --- Render ---
        application.m_Renderer.BeginFrame();

        application.m_Renderer.RenderScene(application.m_SceneGraph);
        application.m_Renderer.RenderLine();
        application.m_Renderer.RenderScreen();

        // Used to display the scene in the editor viewport
        uint32_t textureID = application.m_Renderer.GetFramebufferTextureID();
        // Display the editor panels and pass the framebuffer texture ID to the viewport for rendering the scene
        editor.Update(textureID);

        application.m_Renderer.EndFrame();
    }

    // --- Shutdown ---
    editor.Shutdown();
    application.ShutDown();
    MirageEngine::Core::Scripting::ScriptManager::Instance().ShutDown();
    return 0;
}
