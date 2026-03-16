#include "openGL/openGL_command_buffer.h"
#include "openGL/openGL_pipeline.h"
#include "openGL/openGL_buffer.h"
#include "openGL/openGL_render_pass.h"
#include "openGL/openGL_swap_chain.h"
#include "openGL/openGL_surface.h"

void MirageRHI::OpenGLCommandBuffer::BindPipeline(IPipeline* pipeline)
{
	glUseProgram(pipeline->API_OpenGL().GetProgramGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::BindBuffers(IBuffer* indexBuffer, IBuffer* vertexBuffer, IPipeline* pipeline)
{
	BindVertexBuffer(vertexBuffer->API_OpenGL());
	BindIndexBuffer(indexBuffer->API_OpenGL());
	SetVertAttrLayout(pipeline->API_OpenGL());
}

void MirageRHI::OpenGLCommandBuffer::BindVertexBuffer(OpenGLBuffer buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer.GetGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::BindIndexBuffer(OpenGLBuffer buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.GetGLHandle());
}

void MirageRHI::OpenGLCommandBuffer::SetVertAttrLayout(OpenGLPipeline pipeline)
{
	pipeline.SetLayout();
}

void MirageRHI::OpenGLCommandBuffer::BeginRenderPass(IRenderPass* renderPass)
{
	renderPass->API_OpenGL().Begin();
}

void MirageRHI::OpenGLCommandBuffer::DrawIndexed(uint32_t indexCount)
{
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void MirageRHI::OpenGLCommandBuffer::SubmitAndPresent(ISwapChain* swapchain, ISurface* surface)
{
	swapchain->API_OpenGL().Present(surface->API_OpenGL());
	swapchain->IsScreenResized(surface);
}
