#include "OpenGL/openGL_render_pass.h"

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLRenderPass::Create(Vector4 _color, float _depth)
{
    m_ClearColor[0] = _color.X;
    m_ClearColor[1] = _color.Y;
    m_ClearColor[2] = _color.Z;
    m_ClearColor[3] = _color.W;
    m_ClearDepth = _depth;
}

void MirageRHI::OpenGLRenderPass::Begin()
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    glClearDepth(m_ClearDepth);

    glClear(m_ClearMask);
}
