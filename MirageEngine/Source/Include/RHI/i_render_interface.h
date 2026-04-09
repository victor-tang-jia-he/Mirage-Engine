#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLRenderInterface;
	class IInstance;
	class ISurface;
	class IDevice;
	class ISwapChain;
	class IRenderPass;
	class IPipeline;
	struct IPipelineParameters;
	class IShader;
	class IFramebuffer;
	class IBuffer;
    class ITexture;
	class ICommandBuffer;
	class IUniformBuffer;

	class IRenderInterface
	{
	public:
		enum class API
		{
			OPENGL
		};

		virtual ~IRenderInterface() = default;

		virtual OpenGLRenderInterface& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLRenderInterface"); }

		virtual IInstance* InstantiateInstance() = 0;
		virtual ISurface* InstantiateSurface() = 0;
		virtual IDevice* InstantiateDevice() = 0;
		virtual ISwapChain* InstantiateSwapChain() = 0;
		virtual IRenderPass* InstantiateRenderPass() = 0;
		virtual IPipeline* InstantiatePipeline() = 0;
		virtual IPipelineParameters* InstantiatePipelineParameters() = 0;
		virtual IShader* InstantiateShader() = 0;
		virtual IFramebuffer* InstantiateFramebuffer() = 0;
		virtual IBuffer* InstantiateBuffer() = 0;
		virtual ITexture* InstantiateTexture() = 0;
		virtual IUniformBuffer* InstantiateUniformBuffer() = 0;
		virtual ICommandBuffer* InstantiateCommandBuffer() = 0;
	};
}