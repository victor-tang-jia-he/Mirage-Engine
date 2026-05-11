#include "OpenGL/openGL_surface.h"
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHISurface)

void MirageRHI::OpenGLSurface::Create(GLFWwindow* _context, int _width, int _height)
{
    m_Context = _context;
    SetSize(_width, _height);

    LOG(logTemp, LogRHISurface, "Surface Created with Context: ", (void*)m_Context, " Size: ", m_Width, "x", m_Height);
}

void MirageRHI::OpenGLSurface::SetSize(int _width, int _height)
{
    m_Width = _width;
    m_Height = _height;

    LOG(logTemp, LogRHISurface, "Surface Resized to: ", m_Width, "x", m_Height);
}
