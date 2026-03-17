#include "openGL/openGL_render_pass.h"

void MirageRHI::OpenGLRenderPass::Create(Vector4 color, float depth)
{
    clearColor[0] = color.X;
    clearColor[1] = color.Y;
    clearColor[2] = color.Z;
    clearColor[3] = color.W;
    clearDepth = depth;
}

void MirageRHI::OpenGLRenderPass::Begin()
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClearDepth(clearDepth);

    glClear(clearMask);
}
