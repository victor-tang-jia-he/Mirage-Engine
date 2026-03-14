#pragma once

#include "i_pipeline.h"

namespace MirageRHI
{
	class OpenGLPipeline : public IPipeline
	{
	private:
		GLuint program = 0;
		GLuint vertexShader = 0;
		GLuint fragmentShader = 0;
		GLuint vao = 0;

	public:
		void Create(const char* vertPath, const char* fragPath) override;
		void Destroy() override;

		OpenGLPipeline& API_OpenGL() override { return *this; }
		GLuint GetProgramGLHandle() const { return program; }

		// Shader Setup and linking
		bool SetVertexShader(const std::string& path);
		bool SetFragmentShader(const std::string& path);
		bool Link();

		// Applies the vertex attributes
		void SetLayout();

		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec3(const std::string& name, const Vector3& value) const;
		void SetMat3(const std::string& name, const Matrix3& mat) const;
		void SetMat4(const std::string& name, const Matrix4& mat) const;
		void SetBool(const std::string& name, bool value)const;

	private:
		bool CompileShader(const std::string& path, unsigned int type, unsigned int& shaderOut);
	};
}