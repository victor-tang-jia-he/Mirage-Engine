#include "../../include/openGL/openGL_surface.h"

void MirageRHI::OpenGLSurface::Create(int w, int h, const char* title)
{
    width = w;
    height = h;

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
        throw std::runtime_error("Failed to create window");

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");
}

void MirageRHI::OpenGLSurface::Destroy()
{
    glfwDestroyWindow(window);
    window = nullptr;
}
