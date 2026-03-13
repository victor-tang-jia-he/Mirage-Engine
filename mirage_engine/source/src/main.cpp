// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ImGUI
#include <imgui.h>

// ImGUI Backends
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Includes
#include "test_engine.hpp"

int main()
{
	TestEngine testEngine;
	testEngine.PrintTest();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Mirage Enginexc", NULL, NULL);
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwMakeContextCurrent(window);

	glfwTerminate();
	return 0;
}