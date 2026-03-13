#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = default;

		virtual void Create(IDevice* device, ISwapChain* swapChain, IRenderPass* renderPass) = 0;
		virtual void Destroy(IDevice* device) = 0;

		//virtual VulkanFramebuffers& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanFramebuffer"); }
	};
}