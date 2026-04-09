// MirageEngine.cpp : Defines the entry point for the application.

#include "UI_wrapper.h"
#include "editor_app.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "time_manager.h"
#include "Physics/physics_manager.h"

int main()
{
    Application       application;
    Mirage::EditorApp editor;

    application.InitApp();
    editor.Init(application.m_Renderer.GetWindowHandle(), application.GetSceneGraph());
    MirageEngine::TimeManager::Init();
    while (application.m_Renderer.IsRunning())
    {
        MirageEngine::TimeManager::Update();
        MirageEngine::Core::Input::InputSystem::UpdateHeldInput();
        MirageEngine::Core::Physics::PhysicsManager::Instance()->Update(MirageEngine::TimeManager::GetDeltaTime());
		application.GetSceneGraph()->UpdateAllWorldMatrices();
        application.m_Renderer.BeginFrame();
        application.m_Renderer.RenderScene(application.m_SceneGraph);
        application.m_Renderer.RenderScreen();
        uint32_t textureID = application.m_Renderer.GetFramebufferTextureID();
        editor.Update(textureID);
        application.m_Renderer.EndFrame();
    }

    editor.Shutdown();
    application.ShutDown();
    return 0;
}
