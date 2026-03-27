#pragma once

#include <iostream>
#include <Resources/resource_interface.h>

namespace MirageRHI
{
	class OpenGLShader;
}

namespace MirageRHI
{
	class IShader : public MirageEngine::Core::Resources::IResource
	{
	public:
		enum class ShaderType
		{
			None,
			Vertex,
			Fragment
		};

		virtual ~IShader() = default;

		virtual void Create(const char* _shaderPath, ShaderType _type) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLShader& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLShader"); }
	};
}