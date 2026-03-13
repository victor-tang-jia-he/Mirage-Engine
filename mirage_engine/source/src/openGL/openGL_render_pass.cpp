#include "openGL/openGL_render_pass.h"

void MirageRHI::OpenGLRenderPass::Create(float r, float g, float b, float a, float depth)
{
    clearColor[0] = r;
    clearColor[1] = g;
    clearColor[2] = b;
    clearColor[3] = a;
    clearDepth = depth;
}

void MirageRHI::OpenGLRenderPass::Begin()
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClearDepth(clearDepth);

    glClear(clearMask);
}
