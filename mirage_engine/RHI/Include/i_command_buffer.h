#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLCommandBuffer;
	class OpenGLBuffer;
	class IPipeline;
	class IBuffer;
	class IRenderPass;
	class ISwapChain;
	class ISurface;
}

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
		virtual void BindPipeline(IPipeline* _pipeline) = 0;
		virtual void BindBuffer(IBuffer* _buffer) = 0;
		virtual void ApplyPipelineParameters(IPipeline* _pipeline) = 0;
		virtual void BeginRenderPass(IRenderPass* _renderPass) = 0;
		virtual void EndRenderPass() = 0;
		virtual void DrawIndexed(uint32_t _indexCount) = 0;
		virtual void SubmitAndPresent(ISwapChain* _swapChain, ISurface* _surface) = 0;
	};
}