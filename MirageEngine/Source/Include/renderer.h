#pragma once

#include "i_render_interface.h"



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
	private:
		GLFWwindow* m_Window = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		MirageRHI::IRenderInterface::API m_API = MirageRHI::IRenderInterface::API::OPENGL;

		std::shared_ptr<MirageRHI::IRenderInterface> m_RenderInterface = nullptr;
		std::shared_ptr<MirageRHI::IInstance> m_Instance = nullptr;
		std::shared_ptr<MirageRHI::ISurface> m_Surface = nullptr;
		std::shared_ptr<MirageRHI::IDevice> m_Device = nullptr;
		std::shared_ptr<MirageRHI::ISwapChain> m_SwapChain = nullptr;
		std::shared_ptr<MirageRHI::IRenderPass> m_RenderPass = nullptr;
		std::shared_ptr<MirageRHI::IPipelineParameters> m_ScreenPipelineParameters = nullptr;
		std::shared_ptr<MirageRHI::IPipeline> m_ScreenPipeline = nullptr;
		std::shared_ptr<MirageRHI::IUniformBuffer> m_UniformBuffer = nullptr;
		std::shared_ptr<MirageRHI::ICommandBuffer> m_CommandBuffer = nullptr;

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
		// REdners a list of line
		void RenderLine();

		// --- Getters ---
		// Used to display the scene in the editor viewport
		uint32_t GetFramebufferTextureID() const;
		void* GetWindowHandle() const { return static_cast<void*>(m_Window); }
		// Used by the resource manager to create GPU resources with the correct RHI API
		std::shared_ptr<MirageRHI::IRenderInterface> GetRenderInterfaceHandle() const { return m_RenderInterface; }

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

		// --- Rendering Internal ---
		// Renders a GameObject and all its children recursively
		void RenderObjectRecursive(std::shared_ptr<MirageEngine::GameObject> _obj);
		// Draws a mesh with the given MeshRenderer and Transform
		void DrawMesh(std::shared_ptr<MirageEngine::Core::Components::MeshRenderer> _meshRenderer, const MirageEngine::Core::Components::Transform& _meshTransform);
	};
}