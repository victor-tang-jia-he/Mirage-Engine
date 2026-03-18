#include "OpenGL/openGL_command_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_render_pass.h"
#include "OpenGL/openGL_swap_chain.h"
#include "OpenGL/openGL_surface.h"

void MirageRHI::OpenGLCommandBuffer::BindPipeline(IPipeline* _pipeline)
{
	_pipeline->API_OpenGL().Bind();
}

void MirageRHI::OpenGLCommandBuffer::BindBuffers(IBuffer* _indexBuffer, IBuffer* _vertexBuffer)
{
	BindVertexBuffer(_vertexBuffer->API_OpenGL());
	BindIndexBuffer(_indexBuffer->API_OpenGL());
}

void MirageRHI::OpenGLCommandBuffer::BindVertexBuffer(OpenGLBuffer _buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffer.GetGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::BindIndexBuffer(OpenGLBuffer _buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer.GetGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::ApplyPipelineParameters(IPipeline* _pipeline)
{
	_pipeline->API_OpenGL().ApplyParameters();
}

void MirageRHI::OpenGLCommandBuffer::BeginRenderPass(IRenderPass* _renderPass)
{
	_renderPass->API_OpenGL().Begin();
}

void MirageRHI::OpenGLCommandBuffer::DrawIndexed(uint32_t _indexCount)
{
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}

void MirageRHI::OpenGLCommandBuffer::SubmitAndPresent(ISwapChain* _swapChain, ISurface* _surface)
{
	_swapChain->API_OpenGL().Present(_surface->API_OpenGL());
	_swapChain->IsScreenResized(_surface);
}
