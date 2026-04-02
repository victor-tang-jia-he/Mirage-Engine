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

	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLCommandBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLCommandBuffer"); }

		virtual void BeginDraw(IRenderPass* _renderPass, uint32_t _FBO) = 0;
		virtual void EndDraw() = 0;
		virtual void BindPipeline(IPipeline* _pipeline) = 0;
		virtual void BindBuffer(IBuffer* _buffer) = 0;
		virtual void BindTextureFromID(uint32_t _ID) = 0;
		virtual void ApplyPipelineParameters(IPipeline* _pipeline) = 0;
		virtual void Draw(uint32_t _indexCount) = 0;
		virtual void SubmitAndPresent(ISwapChain* _swapChain, ISurface* _surface) = 0;
	};
}