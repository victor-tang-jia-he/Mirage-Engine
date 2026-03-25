#pragma once

#include <iostream>
#include "vector3.h"
#include "i_pipeline.h"

namespace MirageRHI
{
	class OpenGLUniformBuffer;

	class IUniformBuffer
	{
	public:
		virtual ~IUniformBuffer() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual OpenGLUniformBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLUniformBuffer"); }

		virtual void UpdateUniformBuffer(
			IPipeline* _pipeline,
			MirageEngine::Core::Math::Vector3 _position,
			MirageEngine::Core::Math::Vector3 _scale,
			MirageEngine::Core::Math::Vector3 _rotation) = 0;
	};
}