#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IRenderPass
	{
	public:
		virtual ~IRenderPass() = default;
		virtual void Create(IDevice* device, ISwapChain* swapChain) = 0;
		virtual void Destroy(IDevice* device) = 0;
		//virtual VulkanRenderPass& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanRenderPass"); }
	};
}