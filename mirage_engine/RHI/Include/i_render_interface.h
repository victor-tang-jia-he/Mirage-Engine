#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IRenderInterface
	{
	public:
		enum API
		{
			OPENGL
		};

		virtual ~IRenderInterface() = default;

		virtual IInstance* InstantiateInstance() = 0;
		virtual ISurface* InstantiateSurface() = 0;
		virtual IDevice* InstantiateDevice() = 0;
		virtual ISwapChain* InstantiateSwapChain() = 0;
		virtual IRenderPass* InstantiateRenderPass() = 0;
		virtual IPipeline* InstantiatePipeline() = 0;
		virtual IShader* InstantiateShader() = 0;
		virtual IFramebuffer* InstantiateFramebuffer() = 0;
		virtual IBuffer* InstantiateBuffer() = 0;
		//virtual IUniformBuffer* InstantiateUniformBuffer() = 0;
		virtual ICommandBuffer* InstantiateCommandBuffer() = 0;

		virtual OpenGLRenderInterface& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLRenderInterface"); }
	};
}