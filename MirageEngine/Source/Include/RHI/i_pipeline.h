#pragma once

#include <iostream>
#include <matrix4.h>
#include <i_pipeline_parameters.h>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIPipeline)

namespace MirageRHI
{
    class OpenGLPipeline;
    class IShader;
    enum class UBOBinding : int;

    struct UBOBlockInfo
    {
        const char* Name;
        UBOBinding Binding;
    };

    class IPipeline
    {
    public:

        virtual ~IPipeline() = default;

        virtual void
            Create(std::shared_ptr<IShader> _vertShader, std::shared_ptr<IShader> _fragShader, std::shared_ptr<IPipelineParameters> _pipelineParameters, std::vector<UBOBlockInfo> _blocks = {})
            = 0;
        virtual void Destroy() = 0;

        virtual OpenGLPipeline& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipeline"); }

        virtual void SetInt(const std::string& _name, int _value) const = 0;
        virtual void SetFloat(const std::string& _name, float _value) const = 0;
        virtual void SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value) const = 0;
        virtual void SetVec4(const std::string& _name, const MirageEngine::Core::Math::Vector4& _value) const = 0;
        virtual void SetMat3(const std::string& _name, const MirageEngine::Core::Math::Matrix3& _mat) const = 0;
        virtual void SetMat4(const std::string& _name, const MirageEngine::Core::Math::Matrix4& _mat) const = 0;
        virtual void SetBool(const std::string& _name, bool _value) const = 0;
    };
} // namespace MirageRHI
