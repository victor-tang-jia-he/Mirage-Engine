// MirageEngine.cpp : Defines the entry point for the application.
//
  
#include "UI_wrapper.h"
#include "editor_app.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"

//int main()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
//
//	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Mirage Enginexc", NULL, NULL);
//	glfwMakeContextCurrent(window);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		return -1;
//	}
//
//	Mirage::EditorApp myEditor;
//	myEditor.Init(window);
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glfwPollEvents();
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		myEditor.Update();
//
//		glfwSwapBuffers(window);
//	}
//
//	myEditor.Shutdown();
//	glfwTerminate();
//	return 0;
//}

int main()
{
	Application application;
	Mirage::EditorApp editor;

	application.InitApp();
	editor.Init(application.GetWindowHandle());

	while (application.IsRunning())
	{
		application.BeginFrame();
		editor.Update();
		application.RenderScene();
		application.EndFrame();
	}

	editor.Shutdown();
	application.ShutDown();
	return 0;
}