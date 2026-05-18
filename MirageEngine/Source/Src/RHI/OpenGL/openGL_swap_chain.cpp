#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <OpenGL/openGL_swap_chain.h>
#include <OpenGL/openGL_surface.h>
#include <OpenGL/openGL_framebuffer.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHISwapChain)

void MirageRHI::OpenGLSwapChain::Create(uint32_t _frameAmount, uint32_t _width, uint32_t _height)
{
    if (_frameAmount <= 0)
        LOG(logWarning, LogRHISwapChain, "Empty Swapchain !");

    for (uint32_t i = 0; i < _frameAmount; ++i)
    {
        OpenGLFramebuffer* newFrameBuffer = new OpenGLFramebuffer();
        newFrameBuffer->Create(_width, _height);
        m_FrameBuffers.push_back(newFrameBuffer);
    }

    LOG(logTemp, LogRHISwapChain, "SwapChain Created with ", _frameAmount, " framebuffers of size: ", _width, "x", _height);
}

void MirageRHI::OpenGLSwapChain::Destroy()
{
    for (OpenGLFramebuffer* frameBuffer : m_FrameBuffers)
    {
        frameBuffer->Destroy();
        delete frameBuffer;
    }
    m_FrameBuffers.clear();

    LOG(logTemp, LogRHISwapChain, "SwapChain Destroyed and all framebuffers released.");
}

void MirageRHI::OpenGLSwapChain::IsScreenResized(std::shared_ptr<ISurface> _surface)
{
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(_surface->API_OpenGL().GetContextGLHandle(), &displayWidth, &displayHeight);
    if (_surface->API_OpenGL().GetWidth() != displayWidth || _surface->API_OpenGL().GetHeight() != displayHeight)
    {
        RecreateSwapchain(_surface, displayWidth, displayHeight);
    }
}

void MirageRHI::OpenGLSwapChain::RecreateSwapchain(std::shared_ptr<ISurface> _surface, int _newWidth, int _newHeight)
{
    _surface->API_OpenGL().SetSize(_newWidth, _newHeight);
    for (OpenGLFramebuffer* frameBuffer : m_FrameBuffers)
    {
        frameBuffer->RescaleFrameBuffer(_newWidth, _newHeight);
    }
    glViewport(0, 0, _newWidth, _newHeight);

    LOG(logTemp, LogRHISwapChain, "SwapChain Recreated with new size: ", _newWidth, "x", _newHeight);
}

uint32_t MirageRHI::OpenGLSwapChain::AcquireCurrentFBO() { return m_FrameBuffers.at(currentFrame)->GetOpenGLHandle(); }

uint32_t MirageRHI::OpenGLSwapChain::AcquireCurrentImage() { return m_FrameBuffers.at(currentFrame)->GetFrameTextureID(); }

void MirageRHI::OpenGLSwapChain::Present(OpenGLSurface _surface)
{
    glfwSwapBuffers(_surface.GetContextGLHandle());

    currentFrame = (currentFrame + 1) % m_FrameBuffers.size();
}
