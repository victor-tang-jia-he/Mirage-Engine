#pragma once

#include "i_buffer.h"

namespace MirageRHI
{
	class OpenGLBuffer : public IBuffer
	{
    private:
        GLuint buffer;
        // TODO : Add enum different buffer types;
    public:
        // Creates and stores buffer data (vertexes, indices, textures, ...)
        void Create(const void* data, size_t size) override;
        void Destroy() override;

        OpenGLBuffer& API_OpenGL() override { return *this; }
        GLuint GetGLHandle() const { return buffer; }
	};
}