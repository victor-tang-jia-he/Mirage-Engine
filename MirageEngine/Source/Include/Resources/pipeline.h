#pragma once
#include <Resources/resource_interface.h>
#include <RHI/i_pipeline.h>
#include <Logs/log_macro.h>

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
		MirageRHI::IPipeline* PipelineBind = nullptr;
		MirageRHI::IPipelineParameters* PipelineParameters = nullptr;
		Shader* AttachedVertexShader = nullptr;
		Shader* AttachedFragmentShader = nullptr;

		bool m_bUploaded = false;
	public:

		Pipeline(MirageRHI::IPipeline* _pipelineBind, MirageRHI::IPipelineParameters* _pipelineParameters, Shader* _vertexShader, Shader* _fragmentShader)
			: PipelineBind(_pipelineBind), PipelineParameters(_pipelineParameters), AttachedVertexShader(_vertexShader), AttachedFragmentShader(_fragmentShader) {}

		void Bind();

		void SetNewVertexShader(Shader* _newVertexShader)
		{
			AttachedVertexShader = _newVertexShader;
			m_bUploaded = false; // Mark for re-upload
		}
		void SetNewFragmentShader(Shader* _newFragmentShader)
		{
			AttachedFragmentShader = _newFragmentShader;
			m_bUploaded = false; // Mark for re-upload
		}

	};
}
