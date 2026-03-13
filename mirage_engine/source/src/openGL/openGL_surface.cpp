#include "../../include/openGL/openGL_surface.h"

void MirageRHI::OpenGLSurface::Create(GLFWwindow* _context, int _width, int _hight)
{
    context = _context;
    SetSize(_width, _hight);
}

void MirageRHI::OpenGLSurface::SetSize(int _width, int _hight)
{
    width = _width;
    hight = _hight;
}
