// MirageEngine.cpp : Defines the entry point for the application.
//

// 1. OpenGL loader FIRST - always before any OpenGL headers
#include <glad/glad.h>              // ⬅️ MUST BE FIRST!

// 2. GLFW (may include OpenGL headers, but glad is already loaded)
#include <GLFW/glfw3.h>

// 3. ImGui core
#include <imgui.h>

// 4. ImGui backends (depend on both GLFW and OpenGL)
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>      // This needs glad to be already included!

#include "main.h"

int main()
{
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