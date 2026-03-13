#include "openGL/openGL_swap_chain.h"
#include "openGL/openGL_surface.h"

void MirageRHI::OpenGLSwapChain::IsScreenResized(ISurface* surface)
{
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(surface->API_OpenGL().GetContextGLHandle(), &displayWidth, &displayHeight);
    if (surface->API_OpenGL().GetWidth() != displayWidth || surface->API_OpenGL().GetHeight() != displayHeight) {
        RecreateSwapchain(surface, displayWidth, displayHeight);
    }
}

void MirageRHI::OpenGLSwapChain::RecreateSwapchain(ISurface* surface, int newWidth, int newHeight)
{
    surface->API_OpenGL().SetSize(newWidth, newHeight);
    glViewport(0, 0, newWidth, newHeight);
}

void MirageRHI::OpenGLSwapChain::Present(OpenGLSurface surface)
{
    glfwSwapBuffers(surface.GetContextGLHandle());
}

