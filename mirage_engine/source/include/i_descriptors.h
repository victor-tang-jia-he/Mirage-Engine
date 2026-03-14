#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IDescriptors
	{
	public:
		virtual ~IDescriptors() = default;

		virtual void Create(IDevice* device, ISwapChain* swapchain, IPipeline* pipeline, IUniformBuffer* uniformBuffer, ITexture* texture) = 0;
		virtual void Destroy(IDevice* device) = 0;

		//virtual VulkanDescriptors& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanDescriptor"); }
	};
}