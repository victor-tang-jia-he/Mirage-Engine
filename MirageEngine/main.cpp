// MirageEngine.cpp : Defines the entry point for the application.
//

// #include <glad/glad.h> 
#include "GLFW/glfw3.h"

#include "main.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Hello", NULL, NULL);
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwMakeContextCurrent(window);

	glfwTerminate();
	return 0;
}