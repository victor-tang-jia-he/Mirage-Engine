#pragma once

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <i_pipeline.h>

namespace MirageRHI
{
    class IShader;
    struct IPipelineParameters;

    class OpenGLPipeline : public IPipeline
    {
    private:
        GLuint m_Program = 0;
        std::shared_ptr<IPipelineParameters> m_Parameters = nullptr;
        std::shared_ptr<IShader> m_VertexShader = nullptr;
        std::shared_ptr<IShader> m_FragmentShader = nullptr;

    public:

        void Create(std::shared_ptr<IShader> _vertShader, std::shared_ptr<IShader> _fragShader,
            std::shared_ptr<IPipelineParameters> _pipelineParameters, std::vector<UBOBlockInfo> _blocks = {}) override;
        void Destroy() override;

        OpenGLPipeline& API_OpenGL() override { return *this; }

        // Shader linking
        bool Link() const;

        // Program Binding
        void Bind() const;
        void Unbind() const;

        // Set Shader Parameters
        void SetInt(const std::string& _name, int _value) const override;
        void SetFloat(const std::string& _name, float _value) const override;
        void SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value) const override;
        void SetVec4(const std::string& _name, const MirageEngine::Core::Math::Vector4& _value) const override;
        void SetMat3(const std::string& _name, const MirageEngine::Core::Math::Matrix3& _mat) const override;
        void SetMat4(const std::string& _name, const MirageEngine::Core::Math::Matrix4& _mat) const override;
        void SetBool(const std::string& _name, bool _value) const override;

        // Apply Pipeline Parameters
        void ApplyParameters();

        // Getters
        GLuint GetProgramGLHandle() const { return m_Program; }

    private:
        void BindUBOBlocks(std::vector<UBOBlockInfo> _blocks);
    };
} // namespace MirageRHI
