#include "../include/general_include.h"
#include <iostream>

int main()
{
	if (!glfwInit()) {
		DEBUG_THROW("Failed to init GLFW\n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float width = 1920;
	float height = 1080;

	GLFWwindow* window = glfwCreateWindow(width, height, "Mirage Engine", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		DEBUG_THROW("Failed to create window\n");
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		DEBUG_THROW("Failed to init GLAD\n");
	}

	glViewport(0, 0, width, height);



	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwMakeContextCurrent(window);

	glfwTerminate();
	return 0;
}