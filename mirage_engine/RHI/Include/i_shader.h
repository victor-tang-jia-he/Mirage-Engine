#pragma once

#include <iostream>

namespace MirageRHI
{
	class OpenGLShader;
}

namespace MirageRHI
{
	class IShader
	{
	public:
		enum ShaderType
		{
			Vertex,
			Fragment
		};

		virtual ~IShader() = default;

		virtual void Create(const char* _shaderPath, ShaderType _type) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLShader& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLShader"); }
	};
}