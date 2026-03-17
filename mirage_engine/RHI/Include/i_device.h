#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLDevice;
}

namespace MirageRHI
{
	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLDevice& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLDevice"); }

		virtual void WaitIdle() = 0;
	};
}