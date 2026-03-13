#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IRenderPass
	{
	public:
		virtual ~IRenderPass() = default;
		virtual void Create(float r, float g, float b, float a, float depth) = 0;
		virtual void Destroy() = 0;
		virtual OpenGLRenderPass& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLRenderPass"); }
		virtual void Begin() = 0;
		virtual void End() = 0;
	};
}