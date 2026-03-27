#pragma once

#include <iostream>
#include "i_pipeline_parameters.h"
#include <Resources/resource_interface.h>

namespace MirageRHI
{
	class OpenGLPipeline;
	class IShader;
	
	class IPipeline : public MirageEngine::Core::Resources::IResource 
	{
	public:
		virtual ~IPipeline() = default;

		virtual void Create(IShader* _vertShader, IShader* _fragShader, IPipelineParameters* _pipelineParameters) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLPipeline& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipeline"); }
	};
}
