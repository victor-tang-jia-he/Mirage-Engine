// MirageEngine.cpp : Defines the entry point for the application.

#include "UI_wrapper.h"
#include "editor_app.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "time_manager.h"

int main()
{
	Application application;
	Mirage::EditorApp editor;

	application.InitApp();
	editor.Init(application.GetWindowHandle());
	MirageEngine::TimeManager::Init();
	while (application.IsRunning())
	{
		MirageEngine::TimeManager::Update();
		application.BeginFrame();
		application.RenderScene();
		uint32_t textureID = application.GetFramebufferTextureID();
		editor.Update(textureID);
		application.EndFrame();
	}

	editor.Shutdown();
	application.ShutDown();
	return 0;
}