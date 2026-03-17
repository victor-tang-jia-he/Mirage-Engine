#pragma once

#include "i_pipeline.h"

namespace MirageRHI
{
	class OpenGLPipeline : public IPipeline
	{
	private:
		GLuint m_Program = 0;
		IShader* m_VertexShader;
		IShader* m_FragmentShader;

	public:
		void Create(IShader* _vertShader, IShader* _fragShader) override;
		void Destroy() override;

		OpenGLPipeline& API_OpenGL() override { return *this; }

		// Shader linking
		bool Link();

		// Set Shader Parameters
		void SetInt(const std::string& _name, int _value) const;
		void SetFloat(const std::string& _name, float _value) const;
		void SetVec3(const std::string& _name, const Vector3& _value) const;
		void SetMat3(const std::string& _name, const Matrix3& _mat) const;
		void SetMat4(const std::string& _name, const Matrix4& _mat) const;
		void SetBool(const std::string& _name, bool _value)const;

		// Getters
		GLuint GetProgramGLHandle() const { return m_Program; }
	};
}