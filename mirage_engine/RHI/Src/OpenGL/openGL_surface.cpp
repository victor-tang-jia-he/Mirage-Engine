#include "OpenGL/openGL_surface.h"

void MirageRHI::OpenGLSurface::Create(GLFWwindow* _context, int _width, int _height)
{
    m_Context = _context;
    SetSize(_width, _height);
}

void MirageRHI::OpenGLSurface::SetSize(int _width, int _height)
{
    m_Width = _width;
    m_Height = _height;
}
