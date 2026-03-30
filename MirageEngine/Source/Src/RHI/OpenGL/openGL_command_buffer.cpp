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

void MirageRHI::OpenGLCommandBuffer::BindBuffer(IBuffer* _buffer)
{
	_buffer->API_OpenGL().Bind();
}

void MirageRHI::OpenGLCommandBuffer::ApplyPipelineParameters(IPipeline* _pipeline)
{
	_pipeline->API_OpenGL().ApplyParameters();
}

void MirageRHI::OpenGLCommandBuffer::BeginRenderPass(IRenderPass* _renderPass, ISwapChain* _swapchain)
{
	_renderPass->API_OpenGL().Begin(_swapchain);
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
