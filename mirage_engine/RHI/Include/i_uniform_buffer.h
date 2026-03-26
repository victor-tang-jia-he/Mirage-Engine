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

		virtual void SetView(MirageEngine::Core::Math::Matrix4 _view) = 0;
		virtual void SetProjection(MirageEngine::Core::Math::Matrix4 _proj) = 0;
		virtual void UpdateUniformBuffer(IPipeline* _pipeline, MirageEngine::Core::Math::Matrix4 _model) = 0;
	};
}