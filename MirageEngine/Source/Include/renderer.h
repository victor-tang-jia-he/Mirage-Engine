#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "i_render_interface.h"

namespace MirageEngine
{
	class Renderer
	{
	private:
		GLFWwindow* m_Window = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		MirageRHI::IRenderInterface::API m_API = MirageRHI::IRenderInterface::API::OPENGL;

		MirageRHI::IRenderInterface* m_RenderInterface = nullptr;
		MirageRHI::IInstance* m_Instance = nullptr;
		MirageRHI::ISurface* m_Surface = nullptr;
		MirageRHI::IDevice* m_Device = nullptr;
		MirageRHI::ISwapChain* m_SwapChain = nullptr;
		MirageRHI::IRenderPass* m_RenderPass = nullptr;
		MirageRHI::IPipelineParameters* m_PipelineParameters = nullptr;
		MirageRHI::IPipelineParameters* m_ScreenPipelineParameters = nullptr;
		MirageRHI::IPipeline* m_Pipeline = nullptr;
		MirageRHI::IPipeline* m_ScreenPipeline = nullptr;
		MirageRHI::IUniformBuffer* m_UniformBuffer = nullptr;
		MirageRHI::ICommandBuffer* m_CommandBuffer = nullptr;

	public:
		Renderer(MirageRHI::IRenderInterface::API _api = MirageRHI::IRenderInterface::API::OPENGL) : m_API(_api) {}

		void InitRenderer();
		void Shutdown();

	private:
		// Initialization Functions
		void InitWindow();
		void InitRHI();
	};
}