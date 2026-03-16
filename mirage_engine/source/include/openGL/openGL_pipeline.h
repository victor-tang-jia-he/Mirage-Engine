#pragma once

#include "i_pipeline.h"

namespace MirageRHI
{
	class OpenGLPipeline : public IPipeline
	{
	private:
		GLuint m_Program = 0;
		GLuint m_VertexShader = 0;
		GLuint m_FragmentShader = 0;
		GLuint m_VAO = 0;

	public:
		void Create(const char* _vertPath, const char* _fragPath) override;
		void Destroy() override;

		OpenGLPipeline& API_OpenGL() override { return *this; }
		GLuint GetProgramGLHandle() const { return m_Program; }

		// Shader Setup and linking
		bool SetVertexShader(const std::string& _path);
		bool SetFragmentShader(const std::string& _path);
		bool Link();

		// Applies the vertex attributes
		void SetLayout();

		void SetInt(const std::string& _name, int _value) const;
		void SetFloat(const std::string& _name, float _value) const;
		void SetVec3(const std::string& _name, const Vector3& _value) const;
		void SetMat3(const std::string& _name, const Matrix3& _mat) const;
		void SetMat4(const std::string& _name, const Matrix4& _mat) const;
		void SetBool(const std::string& _name, bool _value)const;

	private:
		bool CompileShader(const std::string& _path, unsigned int _type, unsigned int& _shaderOut);
	};
}