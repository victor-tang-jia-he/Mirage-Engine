#include "openGL/openGL_render_pass.h"

void MirageRHI::OpenGLRenderPass::Create(Vector4 color, float depth)
{
    clearColor[0] = color.x;
    clearColor[1] = color.y;
    clearColor[2] = color.z;
    clearColor[3] = color.w;
    clearDepth = depth;
}

void MirageRHI::OpenGLRenderPass::Begin()
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClearDepth(clearDepth);

    glClear(clearMask);
}
