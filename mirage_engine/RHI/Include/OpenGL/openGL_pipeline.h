#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <matrix4.h>
#include <debug.h>

#include "i_pipeline.h"

using namespace MirageRHI::Debug;

namespace MirageRHI
{
	class IShader;
	struct IPipelineParameters;

	class OpenGLPipeline : public IPipeline
	{
	private:
		GLuint m_Program = 0;
		IPipelineParameters* m_Parameters;
		IShader* m_VertexShader;
		IShader* m_FragmentShader;

	public:
		void Create(IShader* _vertShader, IShader* _fragShader, IPipelineParameters* _pipelineParameters) override;
		void Destroy() override;

		OpenGLPipeline& API_OpenGL() override { return *this; }

		// Shader linking
		bool Link() const;

		// Program Binding
		void Bind() const;
		void Unbind() const;

		// Set Shader Parameters
		void SetInt(const std::string& _name, int _value) const;
		void SetFloat(const std::string& _name, float _value) const;
		void SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value) const;
		void SetMat3(const std::string& _name, const MirageEngine::Core::Math::Matrix3& _mat) const;
		void SetMat4(const std::string& _name, const MirageEngine::Core::Math::Matrix4& _mat) const;
		void SetBool(const std::string& _name, bool _value)const;

		// Apply Pipeline Parameters
		void ApplyParameters();

		// Getters
		GLuint GetProgramGLHandle() const { return m_Program; }
	};
}