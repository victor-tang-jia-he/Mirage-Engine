#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;

		virtual void Create(IDevice* device, IInstance* instance, ISurface* surface, GLFWwindow* window) = 0;
		virtual void Destroy(IDevice* device) = 0;

		//virtual VulkanSwapChain& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanSwapChain"); }

		virtual void RecreateSwapchain(IDevice* device, IInstance* instance, ISurface* surface, IFramebuffers* framebuffers, IRenderPass* renderPass, GLFWwindow* window) = 0;

		virtual uint32_t AcquireNextImage(uint32_t currentFrame, IDevice* device) = 0;
	};
}