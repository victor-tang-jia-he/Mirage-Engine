#pragma once

#include <iostream>

DECLARE_LOG_CATEGORY_EXTERN(LogRHITexture)

namespace MirageRHI
{
    struct TextureDesc
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t Channel;
        bool bMipmaps;
    };

    class OpenGLTexture;

    class ITexture
    {
        public:

            virtual ~ITexture() = default;

            virtual void Create(const void* _data, TextureDesc _desc) = 0;
            virtual void Destroy() = 0;

            virtual void Bind() const = 0;
			virtual uint32_t GetID() const = 0;
            virtual OpenGLTexture& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLTexture"); }
    };
} // namespace MirageRHI
