#include "OpenGL/openGL_command_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_render_pass.h"
#include "OpenGL/openGL_swap_chain.h"
#include "OpenGL/openGL_surface.h"

void MirageRHI::OpenGLCommandBuffer::BindPipeline(IPipeline* _pipeline)
{
	glUseProgram(_pipeline->API_OpenGL().GetProgramGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::BindBuffers(IBuffer* _indexBuffer, IBuffer* _vertexBuffer, IBuffer* _VAO)
{
	BindVertexBuffer(_vertexBuffer->API_OpenGL());
	BindIndexBuffer(_indexBuffer->API_OpenGL());
	SetVertAttrLayout(_VAO->API_OpenGL());
}

void MirageRHI::OpenGLCommandBuffer::BindVertexBuffer(OpenGLBuffer _buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffer.GetGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::BindIndexBuffer(OpenGLBuffer _buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer.GetGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::SetVertAttrLayout(OpenGLBuffer _VAO)
{
	_VAO.SetLayout();
}

void MirageRHI::OpenGLCommandBuffer::BeginRenderPass(IRenderPass* _renderPass)
{
	_renderPass->API_OpenGL().Begin();
}

void MirageRHI::OpenGLCommandBuffer::DrawIndexed(uint32_t _indexCount)
{
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}

void MirageRHI::OpenGLCommandBuffer::SubmitAndPresent(ISwapChain* _swapchain, ISurface* _surface)
{
	_swapchain->API_OpenGL().Present(_surface->API_OpenGL());
	_swapchain->IsScreenResized(_surface);
}
