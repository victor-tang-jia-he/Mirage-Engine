#pragma once

#include <iostream>


DECLARE_LOG_CATEGORY_EXTERN(LogRHICommandBuffer)

namespace MirageRHI
{
	class OpenGLCommandBuffer;
	class OpenGLBuffer;
	class IPipeline;
	class IVertexLayout;
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

		virtual void BeginDraw(std::shared_ptr<IRenderPass> _renderPass, uint32_t _FBO) = 0;
		virtual void EndDraw() = 0;
		virtual void BindPipeline(std::shared_ptr<IPipeline> _pipeline) = 0;
		virtual void BindBuffer(std::shared_ptr<IBuffer> _buffer) = 0;
		virtual void BindTextureFromID(uint32_t _ID) = 0;
		virtual void ApplyPipelineParameters(std::shared_ptr<IPipeline> _pipeline) = 0;
		virtual void ApplyVertexLayout(std::shared_ptr<IVertexLayout> _vertexLayout) = 0;
		virtual void Draw(uint32_t _indexCount) = 0;
		virtual void SubmitAndPresent(std::shared_ptr<ISwapChain> _swapChain, std::shared_ptr<ISurface> _surface) = 0;
	};
}