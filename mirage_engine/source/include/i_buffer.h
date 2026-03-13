#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IBuffer
	{
	public:
		virtual ~IBuffer() = default;

		virtual void Create(const void* data, size_t size) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLBuffer"); }
	};
}