#pragma once

#include <iostream>
#include <i_pipeline_parameters.h>
#include <vector3.h>
#include <matrix3.h>
#include <matrix4.h>

namespace MirageRHI
{
	class OpenGLPipeline;
	class IShader;
	
	class IPipeline 
	{
	public:
		virtual ~IPipeline() = default;

		virtual void Create(IShader* _vertShader, IShader* _fragShader, IPipelineParameters* _pipelineParameters) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLPipeline& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipeline"); }

		virtual void SetInt(const std::string& _name, int _value) const = 0;
		virtual void SetFloat(const std::string& _name, float _value) const = 0;
		virtual void SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value) const = 0;
		virtual void SetMat3(const std::string& _name, const MirageEngine::Core::Math::Matrix3& _mat) const = 0;
		virtual void SetMat4(const std::string& _name, const MirageEngine::Core::Math::Matrix4& _mat) const = 0;
		virtual void SetBool(const std::string& _name, bool _value)const = 0;
	};
}
