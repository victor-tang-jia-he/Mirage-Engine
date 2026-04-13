#pragma once

#include "i_render_interface.h"

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogMirageRenderer)

struct GLFWwindow;

namespace MirageEngine
{
	class SceneGraph;
	class GameObject;

	namespace Core::Components
	{
		class Transform;
		class MeshRenderer;
	}

	class Renderer
	{
	public:
		// Temp Default Pipeline, will be replaced when material system is done
		MirageRHI::IPipeline* m_Pipeline = nullptr;
		MirageRHI::IPipelineParameters* m_PipelineParameters = nullptr;

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
		// --- Initialization ---
		void InitRenderer(MirageRHI::IRenderInterface::API _api = MirageRHI::IRenderInterface::API::OPENGL);
		void Shutdown();

		// --- Rendering ---
		bool IsRunning() const;
		// Manages the beginning of the frame, such as polling events and acquiring the current framebuffer
		void BeginFrame();
		// Renders the given SceneGraph, by rendering all its root GameObjects and their children recursively
		void RenderScene(SceneGraph _sceneGraph);
		// Sets the current framebuffer as the render target and displays the rendered image on the screen
		void RenderScreen();
		// Manages the end of the frame, such as presenting the rendered image and swapping buffers
		void EndFrame();

		// --- Getters ---
		// Used to display the scene in the editor viewport
		uint32_t GetFramebufferTextureID() const;
		void* GetWindowHandle() const { return static_cast<void*>(m_Window); }
		// Used by the resource manager to create GPU resources with the correct RHI API
		MirageRHI::IRenderInterface* GetRenderInterfaceHandle() const { return m_RenderInterface; }

	private:
		// --- Initialization Internal ---
		void InitWindow();
		void InitRHI();
		// Select the API used in the RHI
		void APISelection();
		// Instantiate all unique RHI classes
		void ClassInstantiation();
		// Create all unique RHI classes
		void ClassCreation();
		// Create the pipeline used for screen rendering (post processing, UI, etc...)
		void ScreenPipelineCreation();
		// Temp Pipeline Creation, will be replaced when material system is done !
		void TEMP_PIPELINE_PLACEHOLDER();

		// --- Rendering Internal ---
		// Renders a GameObject and all its children recursively
		void RenderObjectRecursive(GameObject* _obj);
		// Draws a mesh with the given MeshRenderer and Transform components
		void DrawMesh(MirageEngine::Core::Components::MeshRenderer* _meshRenderer, MirageEngine::Core::Components::Transform* _meshTransform);
	};
}