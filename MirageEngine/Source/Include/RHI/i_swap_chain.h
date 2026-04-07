#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLSwapChain;
	class ISurface;

	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;

		virtual void Create(uint32_t _frameAmount, uint32_t _width, uint32_t _height) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLSwapChain& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLSwapChain"); }

		virtual void IsScreenResized(ISurface* _surface) = 0;
		virtual void RecreateSwapchain(ISurface* _surface, int _newWidth, int _newHeight) = 0;

		virtual uint32_t AcquireCurrentFBO() = 0;
		virtual uint32_t AcquireCurrentImage() = 0;
	};
}