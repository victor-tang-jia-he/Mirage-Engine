#include <Resources/pipeline.h>
#include <Resources/resource_manager.h>
#include <Resources/shader.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogPipeline)

using namespace MirageEngine::Core::Resources;

void Pipeline::Bind()
{
    if (!PipelineBind)
    {
        LOG(logError, LogPipeline, "Pipeline ", p_Name, " has no valid m_Pipeline to bind !");
        return;
    }
    if (!AttachedVertexShader || !AttachedFragmentShader)
    {
        LOG(logError, LogPipeline, "Pipeline ", p_Name, " has invalid attached shader(s) !");
        return;
    }
    if (!PipelineParameters)
    {
        LOG(logError, LogPipeline, "Pipeline ", p_Name, " has no valid m_Pipeline parameters !");
        return;
    }
    if (!m_bUploaded)
    {
        m_bUploaded = true;
        PipelineBind->Create(AttachedVertexShader->ShaderBind, AttachedFragmentShader->ShaderBind, PipelineParameters);
        LOG(logTemp, LogPipeline, "Uploading Pipeline : ", p_Name);
    }
    else
    {
        LOG(logTemp, LogPipeline, "Pipeline : ", p_Name, " already binded");
    }
}
