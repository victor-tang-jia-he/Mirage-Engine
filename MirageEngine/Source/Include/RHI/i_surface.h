#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLSurface;
}

struct GLFWwindow;

namespace MirageRHI
{
	class ISurface
	{
	public:
		virtual ~ISurface() = default;

		virtual void Create(GLFWwindow* _context, int _width, int _height) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLSurface& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLSurface"); }
	};
}