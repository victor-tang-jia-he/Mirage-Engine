#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLFramebuffer;

	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLFramebuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLFramebuffer"); }
	};
}