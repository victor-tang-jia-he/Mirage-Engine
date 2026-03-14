#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLCommandBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLCommandBuffer"); }

		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;
		virtual void BindPipeline(IPipeline* pipeline) = 0;
		virtual void BindBuffers(IBuffer* indexBuffer, IBuffer* vertexBuffer, IPipeline* pipeline) = 0;
		virtual void BeginRenderPass(IRenderPass* renderPass) = 0;
		virtual void EndRenderPass() = 0;
		virtual void DrawIndexed(uint32_t indexCount) = 0;
		virtual void SubmitAndPresent(ISwapChain* swapchain, ISurface* surface) = 0;
	};
}