#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLSwapChain& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLSwapChain"); }

		virtual void IsScreenResized(ISurface* surface) = 0;
		virtual void RecreateSwapchain(ISurface* surface, int newWidth, int newHight) = 0;

		virtual uint32_t AcquireNextImage() = 0;
	};
}