#pragma once

#include "i_shader.h"

namespace MirageRHI
{
	class OpenGLShader : public IShader
	{
	private:
		GLuint m_Shader = 0;
		ShaderType m_Type;

	public:
		void Create(const char* _shaderPath, ShaderType _type) override;
		void Destroy() override;

		OpenGLShader& API_OpenGL() override { return *this; }
		GLuint GetGLHandle() const { return m_Shader; }
		ShaderType GetType() const { return m_Type; }

	private:
		bool CompileShader(const std::string& _path, unsigned int _type, unsigned int& _shaderOut);
	};
}