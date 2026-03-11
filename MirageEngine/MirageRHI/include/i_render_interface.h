#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IRenderInterface
	{
	public:
		enum API
		{
			VULKAN
		};

		virtual ~IRenderInterface() = default;

		virtual IInstance* InstantiateInstance() = 0;
		virtual ISurface* InstantiateSurface() = 0;
		virtual IDevice* InstantiateDevice() = 0;
		virtual ISwapChain* InstantiateSwapChain() = 0;
		virtual IRenderPass* InstantiateRenderPass() = 0;
		virtual IPipeline* InstantiatePipeline() = 0;
		virtual IFramebuffers* InstantiateFramebuffer() = 0;
		virtual ITexture* InstantiateTexture() = 0;
		virtual IBuffers* InstantiateBuffer() = 0;
		virtual IUniformBuffer* InstantiateUniformBuffer() = 0;
		virtual IDescriptors* InstantiateDescriptor() = 0;
		virtual ICommandBuffers* InstantiateCommandBuffer() = 0;

		/*virtual void DeleteInstance(IInstance* _instance) { delete _instance; }
		virtual void DeleteSurface(ISurface* _surface) { delete _surface; }
		virtual void DeleteDevice(IDevice* _device) { delete _device; }
		virtual void DeleteSwapChain(ISwapChain* _swapChain) { delete _swapChain; }
		virtual void DeleteRenderPass(IRenderPass* _renderPass) { delete _renderPass; }
		virtual void DeletePipeline(IPipeline* _pipeline) { delete _pipeline; }
		virtual void DeleteFramebuffer(IFramebuffers* _framebuffer) { delete _framebuffer; }
		virtual void DeleteTexture(ITexture* _texture) { delete _texture; }
		virtual void DeleteBuffer(IBuffers* _buffer) { delete _buffer; }
		virtual void DeleteUniformBuffer(IUniformBuffer* _uniformBuffer) { delete _uniformBuffer; }
		virtual void DeleteDescriptor(IDescriptors* _descriptor) { delete _descriptor; }
		virtual void DeleteCommandBuffer(ICommandBuffers* _commandBuffer) { delete _commandBuffer; }*/

		//virtual VulkanRenderInterface& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanRendererInterface"); }
	};
}