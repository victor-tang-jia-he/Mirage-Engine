#include "OpenGL/openGL_swap_chain.h"
#include "OpenGL/openGL_surface.h"

void MirageRHI::OpenGLSwapChain::IsScreenResized(ISurface* _surface)
{
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(_surface->API_OpenGL().GetContextGLHandle(), &displayWidth, &displayHeight);
    if (_surface->API_OpenGL().GetWidth() != displayWidth || _surface->API_OpenGL().GetHeight() != displayHeight)
    {
        RecreateSwapchain(_surface, displayWidth, displayHeight);
    }
} 

void MirageRHI::OpenGLSwapChain::RecreateSwapchain(ISurface* _surface, int _newWidth, int _newHeight)
{
    _surface->API_OpenGL().SetSize(_newWidth, _newHeight);
    glViewport(0, 0, _newWidth, _newHeight);
}

void MirageRHI::OpenGLSwapChain::Present(OpenGLSurface _surface)
{
    glfwSwapBuffers(_surface.GetContextGLHandle());
}


