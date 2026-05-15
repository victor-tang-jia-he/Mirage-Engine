#pragma once
#include <Resources/resource_interface.h>
#include <RHI/i_pipeline.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPipeline)

namespace MirageRHI
{
    class IPipeline;
}

namespace MirageEngine::Core::Resources
{
    class Shader;

    struct Pipeline : public IResource
    {
    private:
        std::shared_ptr<MirageRHI::IPipeline> PipelineBind = nullptr;
        std::shared_ptr<MirageRHI::IPipelineParameters> PipelineParameters = nullptr;
        std::shared_ptr<Shader> AttachedVertexShader = nullptr;
        std::shared_ptr<Shader> AttachedFragmentShader = nullptr;
        std::vector<MirageRHI::UBOBlockInfo> UboBlockInfo{};

        bool m_bUploaded = false;

    public:
        Pipeline(std::shared_ptr<MirageRHI::IPipeline> _pipelineBind,
            std::shared_ptr<MirageRHI::IPipelineParameters> _pipelineParameters, std::shared_ptr<Shader> _vertexShader,
            std::shared_ptr<Shader> _fragmentShader, std::vector<MirageRHI::UBOBlockInfo> _uboBlockInfo)
            : PipelineBind(_pipelineBind)
            , PipelineParameters(_pipelineParameters)
            , AttachedVertexShader(_vertexShader)
            , AttachedFragmentShader(_fragmentShader)
            , UboBlockInfo(_uboBlockInfo)
        {
        }

        ~Pipeline() override = default;

        void Bind();

        void SetNewVertexShader(std::shared_ptr<Shader> _newVertexShader)
        {
            AttachedVertexShader = _newVertexShader;
            m_bUploaded = false; // Mark for re-upload
        }

        void SetNewFragmentShader(std::shared_ptr<Shader> _newFragmentShader)
        {
            AttachedFragmentShader = _newFragmentShader;
            m_bUploaded = false; // Mark for re-upload
        }
    };
} // namespace MirageEngine::Core::Resources
