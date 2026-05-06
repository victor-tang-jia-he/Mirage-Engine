#pragma once

#include <i_texture.h>

namespace MirageRHI
{
    class OpenGLTexture : public ITexture
    {
        private:

            uint32_t m_Texture = 0;

        public:

            void Create(const void* _data, TextureDesc _desc) override;
            void Destroy() override;

            void Bind() const override;

            OpenGLTexture& API_OpenGL() override { return *this; }
    };
} // namespace MirageRHI
