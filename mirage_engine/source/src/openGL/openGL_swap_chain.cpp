#include "../../include/openGL/openGL_swap_chain.h"
#include "../../include/openGL/openGL_surface.h"

void MirageRHI::OpenGLSwapChain::IsScreenResized(ISurface* surface)
{
    int displayWidth, displayHight;
    glfwGetFramebufferSize(surface->API_OpenGL().GetContextGLHandle(), &displayWidth, &displayHight);
    if (surface->API_OpenGL().GetWidth() != displayWidth || surface->API_OpenGL().GetHight() != displayHight) {
        RecreateSwapchain(surface, displayWidth, displayHight);
    }
}

void MirageRHI::OpenGLSwapChain::RecreateSwapchain(ISurface* surface, int newWidth, int newHight)
{
    surface->API_OpenGL().SetSize(newWidth, newHight);
    glViewport(0, 0, newWidth, newHight);
}

void MirageRHI::OpenGLSwapChain::Present(OpenGLSurface surface)
{
    glfwSwapBuffers(surface.GetContextGLHandle());
}

