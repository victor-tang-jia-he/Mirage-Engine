#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLPipeline;
	class IShader;
}

namespace MirageRHI
{
	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual void Create(IShader* _vertShader, IShader* _fragShader) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLPipeline& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipeline"); }
	};
}
