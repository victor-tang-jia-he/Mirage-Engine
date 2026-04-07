#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "i_render_interface.h"

#include <SceneGraph/scene_graph.h>


namespace MirageEngine
{
	namespace Core::Components
	{
		class Transform;
		class MeshRenderer;
	}

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
		MirageRHI::IPipelineParameters* m_ScreenPipelineParameters = nullptr;
		MirageRHI::IPipeline* m_ScreenPipeline = nullptr;
		MirageRHI::IUniformBuffer* m_UniformBuffer = nullptr;
		MirageRHI::ICommandBuffer* m_CommandBuffer = nullptr;

	public:
		// Temp Default Pipeline, will be replaced when material system is done
		MirageRHI::IPipeline* m_Pipeline = nullptr;
		MirageRHI::IPipelineParameters* m_PipelineParameters = nullptr;

	public:
		void InitRenderer(MirageRHI::IRenderInterface::API _api = MirageRHI::IRenderInterface::API::OPENGL);
		void Shutdown();

		bool IsRunning() const;
		void BeginFrame();
		void RenderScene(SceneGraph _sceneGraph); // Finish When Scene and MeshRenderer are done
		void RenderScreen();
		void EndFrame();

		uint32_t GetFramebufferTextureID() const;
		void* GetWindowHandle() const { return static_cast<void*>(m_Window); }
		MirageRHI::IRenderInterface* GetRenderInterfaceHandle() const { return m_RenderInterface; }

	private:
		// Initialization Functions
		void InitWindow();
		void InitRHI();

		void DrawMesh(MirageEngine::Core::Components::MeshRenderer* _meshRenderer, MirageEngine::Core::Components::Transform* _meshTransform);
	};
}