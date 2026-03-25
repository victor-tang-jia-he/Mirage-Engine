#pragma once

#include <iostream>
#include "vector3.h"
#include "i_pipeline.h"
#include "../../Components/camera.h"

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
			const MirageEngine::Core::Components::Camera* cam,
			MirageEngine::Core::Components::Transform& transform) = 0;
	};
}