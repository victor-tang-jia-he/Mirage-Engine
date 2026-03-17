#pragma once

#include <iostream>

#include <vector4.h>

using namespace MirageEngine::Core::Maths;

namespace MirageRHI
{
	class OpenGLRenderPass;
}

namespace MirageRHI
{
	class IRenderPass
	{
	public:
		virtual ~IRenderPass() = default;

		virtual void Create(Vector4 _color, float _depth) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLRenderPass& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLRenderPass"); }

		virtual void Begin() = 0;
		virtual void End() = 0;
	};
}