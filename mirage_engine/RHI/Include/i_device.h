#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLDevice& API_OpenGl() { throw std::runtime_error("Bad API Call : object is not a OpenGlBuffer"); }

		virtual void WaitIdle() = 0;
	};
}