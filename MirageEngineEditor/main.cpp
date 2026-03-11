// MirageEngine.cpp : Defines the entry point for the application.
//

#include "main.h"
#include "Include/UIWrapper.h"
#include "../Libraries/GLFW/deps/glad/gl.h"
#include "../Libraries/GLFW/include/GLFW/glfw3.h"


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Mirage Enginexc", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		return -1;
	}
	Mirage::UIWrapper::Init(window);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Mirage::UIWrapper::BeginFrame();
		Mirage::UIWrapper::EndFrame();	
		glfwSwapBuffers(window);
	}

	Mirage::UIWrapper::Shutdown();
	glfwTerminate();
	return 0;
}