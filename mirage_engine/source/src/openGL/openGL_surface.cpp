#include "openGL/openGL_surface.h"

void MirageRHI::OpenGLSurface::Create(GLFWwindow* _context, int _width, int _height)
{
    context = _context;
    SetSize(_width, _height);
}

void MirageRHI::OpenGLSurface::SetSize(int _width, int _height)
{
    width = _width;
    height = _height;
}
