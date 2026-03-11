#include "../../include/openGL/openGL_instance.h"

void MirageRHI::OpenGLInstance::Create()
{
    if (!glfwInit())
        throw std::runtime_error("GLFW init failed");
}

void MirageRHI::OpenGLInstance::Destroy()
{
    glfwTerminate();
}
