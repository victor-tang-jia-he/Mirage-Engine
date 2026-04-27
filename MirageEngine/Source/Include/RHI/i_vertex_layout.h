#pragma once

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIVertexLayout)

namespace MirageRHI
{
    class OpenGLVertexLayout;

    enum class AttributeType 
    {
        Float,
        Int
	};

    struct VertexAttribute 
    {
        std::string Name = "";
        uint32_t Size = 0; // Number of variables in the array
        AttributeType Type = AttributeType::Float;
        bool Normalized = false;
        uint32_t Stride = 0;
        void* Pointer = nullptr;
    };

    class IVertexLayout
    {
    public:
        virtual void Create() = 0;
        virtual void Destroy() = 0;

        virtual OpenGLVertexLayout& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLVertexLayout"); }

        virtual void AddAttribute(std::shared_ptr<VertexAttribute> _newAttribute, uint32_t _index) = 0;
        virtual void RemoveAttribute(uint32_t _targetAttributeIndex) = 0;
        virtual void RemoveAttribute(std::string _targetAttributeName) = 0;
        virtual void ClearAttributes() = 0;
    };
}