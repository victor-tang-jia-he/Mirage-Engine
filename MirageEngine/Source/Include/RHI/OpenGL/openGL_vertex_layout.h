#pragma once

#include <i_vertex_layout.h>

namespace MirageRHI
{
    class OpenGLVertexLayout : public IVertexLayout
    {
        private:

            std::unordered_map<uint32_t, std::shared_ptr<VertexAttribute>> m_Attributes;
            uint32_t m_VAO;

        public:

            void Create() override;
            void Destroy() override;

            OpenGLVertexLayout& API_OpenGL() override { return *this; }

            uint32_t GetVertexAttributesObjectHandle() const { return m_VAO; }

            void AddAttribute(std::shared_ptr<VertexAttribute> _newAttribute, uint32_t _index) override;
            void RemoveAttribute(uint32_t _targetAttributeIndex) override;
            void RemoveAttribute(std::string _targetAttributeName) override;
            void ClearAttributes() override;
            void SetLayout();
    };
} // namespace MirageRHI
