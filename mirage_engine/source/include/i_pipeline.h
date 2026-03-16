#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual void Create(const char* _vertPath, const char* _fragPath) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLPipeline& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipeline"); }
	};
}
