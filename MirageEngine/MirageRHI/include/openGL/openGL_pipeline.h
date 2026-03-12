#pragma once

#include "../i_pipeline.h"

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
		void Create(const char* vertSrc, const char* fragSrc) override;
		void Destroy() override;

		OpenGLPipeline& API_OpenGL() override { return *this; }
		GLuint GetProgramGLHandle() const { return program; }

		void SetLayout();
	};
}