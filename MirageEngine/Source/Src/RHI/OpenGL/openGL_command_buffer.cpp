#include "OpenGL/openGL_command_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_render_pass.h"
#include "OpenGL/openGL_swap_chain.h"
#include "OpenGL/openGL_surface.h"
#include "OpenGL/openGL_vertex_layout.h"
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHICommandBuffer)

void MirageRHI::OpenGLCommandBuffer::BeginDraw(std::shared_ptr<IRenderPass> _renderPass, uint32_t _FBO)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    _renderPass->API_OpenGL().Begin();
}

void MirageRHI::OpenGLCommandBuffer::BindPipeline(std::shared_ptr<IPipeline> _pipeline) { _pipeline->API_OpenGL().Bind(); }

void MirageRHI::OpenGLCommandBuffer::BindBuffer(std::shared_ptr<IBuffer> _buffer) { _buffer->API_OpenGL().Bind(); }

void MirageRHI::OpenGLCommandBuffer::BindTextureFromID(uint32_t _ID)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _ID);
}

void MirageRHI::OpenGLCommandBuffer::ApplyPipelineParameters(std::shared_ptr<IPipeline> _pipeline) { _pipeline->API_OpenGL().ApplyParameters(); }

void MirageRHI::OpenGLCommandBuffer::ApplyVertexLayout(std::shared_ptr<IVertexLayout> _vertexLayout) { _vertexLayout->API_OpenGL().SetLayout(); }

void MirageRHI::OpenGLCommandBuffer::Draw(uint32_t _indexCount, DrawMode _mode)
{
    GLint ebo = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

    GLenum glMode;
    switch (_mode)
    {
        case DrawMode::LINES:
            glMode = GL_LINES;
            break;
        case DrawMode::POINTS:
            glMode = GL_POINTS;
            break;
        default:
            glMode = GL_TRIANGLES;
            break;
    }

    // If no element array buffer is bound, assume non-indexed draw (ex : fullscreen triangle)
    if (ebo == 0)
    {
        // Fallback to non-indexed draw to avoid driver accessing null EBO
        glDrawArrays(glMode, 0, (GLsizei)_indexCount);
    }
    else
    {
        glDrawElements(glMode, (GLsizei)_indexCount, GL_UNSIGNED_INT, (const void*)0);
    }
}

void MirageRHI::OpenGLCommandBuffer::SubmitAndPresent(std::shared_ptr<ISwapChain> _swapChain, std::shared_ptr<ISurface> _surface)
{
    _swapChain->API_OpenGL().Present(_surface->API_OpenGL());
    _swapChain->IsScreenResized(_surface);
}
