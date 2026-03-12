#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IInstance
	{
	public:
		virtual ~IInstance() = default;

		virtual void Create(std::string _name) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLInstance& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLInstance"); }
	};
}