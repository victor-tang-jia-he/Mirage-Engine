#pragma once

#include <iostream>
#include <filesystem>

namespace MirageRHI
{
	class OpenGLBuffer;

	class IBuffer
	{
	public:
		enum BufferType
		{
			None,
			Vertices,
			Indices,
			Texture
		};

		virtual ~IBuffer() = default;


		virtual void Bind() const = 0;

		virtual void Create(const void* _data, size_t _size, BufferType _type) = 0;
		virtual void Create(const std::filesystem::path& _filename, BufferType _type) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLBuffer"); }
	};
}