#pragma once
#include <Resources/resource_interface.h>
#include <RHI/i_pipeline.h>
#include <Resources/shader.h>

namespace MirageRHI
{
	class IPipeline;
}

namespace MirageEngine::Core::Resources
{
	struct Pipeline : public IResource
	{
	public:
		MirageRHI::IPipeline* BindPipeline = nullptr;
		Shader* AttachedVertexShader = nullptr;
		Shader* AttachedFragmentShader = nullptr;

		Pipeline() = default;
	};
}
