#pragma once

// #include "general_include.h"
#include <iostream>

namespace MirageRHI
{
	class OpenGLUniformBuffer;
}

namespace MirageRHI
{
	class IUniformBuffer
	{
	public:
		virtual ~IUniformBuffer() = default;

		virtual void Create(IDevice* device, ISwapChain* swapChain) = 0;
		virtual void Destroy(IDevice* device, ISwapChain* swapChain) = 0;

		//virtual VulkanUniformBuffer& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanUniformBuffer"); }

		virtual void UpdateUniformBuffer(uint32_t currentFrame, ISwapChain* swapChain) = 0;
	};
}