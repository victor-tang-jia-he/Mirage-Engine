#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <debug.h>
#include <string>
#include <fstream>
#include <sstream>

#include "i_shader.h"

namespace MirageRHI
{
	class OpenGLShader : public IShader
	{
	private:
		GLuint m_Shader = 0;
		ShaderType m_Type = ShaderType::None;

	public:
		void Create(const char* _shaderPath, ShaderType _type) override;
		void Destroy() override;

		OpenGLShader& API_OpenGL() override { return *this; }

		// Getters
		GLuint GetGLHandle() const { return m_Shader; }
		ShaderType GetType() const { return m_Type; }

	private:
		bool CompileShader(const std::string& _path, unsigned int _type, unsigned int& _shaderOut);
	};
}