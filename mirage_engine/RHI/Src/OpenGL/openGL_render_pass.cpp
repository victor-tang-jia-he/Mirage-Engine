#include "OpenGL/openGL_render_pass.h"

void MirageRHI::OpenGLRenderPass::Create(Vector4 _color, float _depth)
{
    m_ClearColor[0] = _color.x;
    m_ClearColor[1] = _color.y;
    m_ClearColor[2] = _color.z;
    m_ClearColor[3] = _color.w;
    m_ClearDepth = _depth;
}

void MirageRHI::OpenGLRenderPass::Begin()
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    glClearDepth(m_ClearDepth);

    glClear(m_ClearMask);
}
