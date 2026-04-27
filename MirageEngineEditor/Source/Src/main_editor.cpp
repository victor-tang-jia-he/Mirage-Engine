// MirageEngine.cpp : Defines the entry point for the application.

#include "UI_wrapper.h"
#include "editor_app.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "time_manager.h"
#include "Physics/physics_manager.h"
#include "Reflection/register_meta.h"

#include <Scripting/script_api.h>
#include <vector>
#include <memory>
#include <cstdio>

struct DllDeleter {
    void operator()(MirageEngine::Core::Scripting::IScript* s) const {
        if (s) {
            s->OnDestroy();
            DestroyScript(s);
        }
    }
};

using ScriptPtr = std::unique_ptr<MirageEngine::Core::Scripting::IScript, DllDeleter>;

int main() {
    LoadLibraryA("GameScripts.dll");

    // R¨¦cup¨¨re tous les scripts de la DLL
    std::vector<ScriptPtr> scripts;

    int count = GetScriptCount();
    printf("=== %d script(s) trouv¨¦(s) ===\n\n", count);

    for (int i = 0; i < count; ++i) {
        const char* name = GetScriptName(i);
        std::cout << name << std::endl;
        MirageEngine::Core::Scripting::IScript* s = CreateScriptByName(name);
        if (s) scripts.emplace_back(s);
    }

    // onStart
    printf("--- onStart ---\n");
    for (auto& s : scripts)
        s->OnStart();

    // onUpdate x3
    printf("\n--- onUpdate ---\n");
    for (int frame = 0; frame < 3; ++frame)
        for (auto& s : scripts)
            s->OnUpdate(0.016f);

    // onDestroy ¡ª appel¨¦ automatiquement par le DllDeleter
    printf("\n--- onDestroy (destruction du vector) ---\n");
    scripts.clear();

    return 0;
 //   Application       application;
 //   Mirage::EditorApp editor;

	//application.InitApp();
	//MirageEngine::RegisterMeta().RegisterAllMeta();
	//editor.Init(application.m_Renderer.GetWindowHandle(), &application.m_Renderer, application.GetSceneGraph());
	//MirageEngine::TimeManager::Init();
	//while (application.m_Renderer.IsRunning())
	//{
	//	MirageEngine::TimeManager::Update();
	//	MirageEngine::Core::Input::InputSystem::UpdateHeldInput();
	//	MirageEngine::Core::Physics::PhysicsManager::Instance().Update(MirageEngine::TimeManager::GetDeltaTime());
	//	application.GetSceneGraph()->UpdateAllWorldMatrices();
	//	application.m_Renderer.BeginFrame();
	//	application.m_Renderer.RenderScene(application.m_SceneGraph);
	//	application.m_Renderer.RenderScreen();
	//	editor.Update(application.m_Renderer.GetFramebufferTextureID());
	//	application.m_Renderer.EndFrame();
	//}

 //   editor.Shutdown();
 //   application.ShutDown();
    return 0;
}
