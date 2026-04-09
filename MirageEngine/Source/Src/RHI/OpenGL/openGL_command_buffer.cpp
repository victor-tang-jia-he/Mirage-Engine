#include "OpenGL/openGL_command_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_render_pass.h"
#include "OpenGL/openGL_swap_chain.h"
#include "OpenGL/openGL_surface.h"

DEFINE_LOG_CATEGORY(LogRHICommandBuffer)

void MirageRHI::OpenGLCommandBuffer::BeginDraw(IRenderPass* _renderPass, uint32_t _FBO)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    _renderPass->API_OpenGL().Begin();
}

void MirageRHI::OpenGLCommandBuffer::BindPipeline(IPipeline* _pipeline)
{
	_pipeline->API_OpenGL().Bind();
}

void MirageRHI::OpenGLCommandBuffer::BindBuffer(IBuffer* _buffer)
{
	_buffer->API_OpenGL().Bind();
}

void MirageRHI::OpenGLCommandBuffer::BindTextureFromID(uint32_t _ID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _ID);
}

void MirageRHI::OpenGLCommandBuffer::ApplyPipelineParameters(IPipeline* _pipeline)
{
	_pipeline->API_OpenGL().ApplyParameters();
}

void MirageRHI::OpenGLCommandBuffer::Draw(uint32_t _indexCount)
{
    // Note : Made with the help of Copilot
    // Diagnostic: capture GL state to logs / break early in debug builds
    GLenum err = glGetError(); // clear previous error
    GLint vao = 0;
    GLint ebo = 0;
    GLint prog = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

    // If no element array buffer is bound, assume non-indexed draw (ex : fullscreen triangle)
    if (ebo == 0)
    {
        // Fallback to non-indexed draw to avoid driver accessing null EBO
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_indexCount);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, (GLsizei)_indexCount, GL_UNSIGNED_INT, (const void*)0);
    }

    // Check for errors after draw (helps catch invalid state)
    GLenum postErr = glGetError();
    if (postErr != GL_NO_ERROR)
        LOG(logWarning, LogRHICommandBuffer, "glDraw error: 0x%X", postErr);
}

void MirageRHI::OpenGLCommandBuffer::SubmitAndPresent(ISwapChain* _swapChain, ISurface* _surface)
{
	_swapChain->API_OpenGL().Present(_surface->API_OpenGL());
	_swapChain->IsScreenResized(_surface);
}
