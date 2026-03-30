#include "OpenGL/openGL_swap_chain.h"
#include "OpenGL/openGL_surface.h"
#include "OpenGL/openGL_framebuffer.h"
#include "debug.h"

void MirageRHI::OpenGLSwapChain::Create(uint32_t _frameAmount, uint32_t _width, uint32_t _height)
{
    if (_frameAmount <= 0)
        DEBUG_THROW("Empty Swapchain !");

    for (uint32_t i = 0; i < _frameAmount; ++i)
    {
        OpenGLFramebuffer* newFrameBuffer = new OpenGLFramebuffer();
        newFrameBuffer->Create(_width, _height);
        m_FrameBuffers.push_back(newFrameBuffer);
    }
}

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
    for (OpenGLFramebuffer* frameBuffer : m_FrameBuffers)
    {
        frameBuffer->RescaleFrameBuffer(_newWidth, _newHeight);
    }
    glViewport(0, 0, _newWidth, _newHeight);
}

uint32_t MirageRHI::OpenGLSwapChain::AcquireNextImage()
{
    return m_FrameBuffers.at((currentFrame + 1) % m_FrameBuffers.size())->GetFrameTexture();
}

void MirageRHI::OpenGLSwapChain::Present(OpenGLSurface _surface)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffers.at(currentFrame)->GetOpenGLHandle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, _surface.GetWidth(), _surface.GetHeight(),
        0, 0, _surface.GetWidth(), _surface.GetHeight(),
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glfwSwapBuffers(_surface.GetContextGLHandle());

    currentFrame = (currentFrame + 1) % m_FrameBuffers.size();
    //m_FrameBuffers.at(currentFrame)->Bind();
}


