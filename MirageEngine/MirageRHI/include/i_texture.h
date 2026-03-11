#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Create(IDevice* device) = 0;
		virtual void Destroy(IDevice* device) = 0;

		//virtual VulkanTexture& API_Vulkan() { throw std::runtime_error("Bad API Call : object is not a VulkanTexture"); }
	};
}