#pragma once

#include "general_include.h"

namespace MirageRHI
{
	class IBuffer
	{
	public:
		enum BufferType
		{
			Vertices,
			Indices,
			VertexAttributes,
			Texture
		};

		virtual ~IBuffer() = default;

		virtual void Create(const void* _data, size_t _size, BufferType _type) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLBuffer"); }
	};
}