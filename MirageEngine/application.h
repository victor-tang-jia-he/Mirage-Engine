#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "i_render_interface.h"
#include "i_instance.h"
#include "i_surface.h"
#include "i_device.h"
#include "i_swap_chain.h"
#include "i_render_pass.h"
#include "i_pipeline.h"
#include "i_pipeline_parameters.h"
#include "i_shader.h"
#include "i_framebuffer.h"
#include "i_buffer.h"
#include "i_uniform_buffer.h"
#include "i_command_buffer.h"

// Input
#include <Components/input_system.h>
namespace MirageEngine {
	namespace Core {
		namespace Components {
			class Camera;
			class MeshRenderer;
		}
	}
}

using namespace MirageRHI;

class Application
{
private:
	GLFWwindow* m_Window = nullptr;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;

	IRenderInterface::API m_API = IRenderInterface::API::OPENGL;

	IRenderInterface* m_RenderInterface = nullptr;
	IInstance* m_Instance = nullptr;
	ISurface* m_Surface = nullptr;
	IDevice* m_Device = nullptr;
	ISwapChain* m_SwapChain = nullptr;
	IRenderPass* m_RenderPass = nullptr;
	IPipelineParameters* m_PipelineParameters = nullptr;
	IPipelineParameters* m_ScreenPipelineParameters = nullptr;
	IPipeline* m_Pipeline = nullptr;
	IPipeline* m_ScreenPipeline = nullptr;
	IFramebuffer* m_Framebuffer = nullptr;
	IUniformBuffer* m_UniformBuffer = nullptr;
	ICommandBuffer* m_CommandBuffer = nullptr;

	// TODO : Edit in map
	MirageEngine::Core::Input::InputMapping* m_EditorMapping = nullptr;
	MirageEngine::Core::Input::InputMapping* m_PlayMapping = nullptr;

	MirageEngine::Core::Components::Camera* m_EngineCamera = nullptr;
	std::vector < std::shared_ptr < MirageEngine::Core::Components::MeshRenderer>> m_meshRenderer;
public:
	Application(IRenderInterface::API _api = IRenderInterface::API::OPENGL) : m_API(_api) {}

	void InitApp();
	void ShutDown();

	bool IsRunning() const;
	void BeginFrame();
	void RenderScene();
	void EndFrame();

	void* GetWindowHandle() const { return static_cast<void*>(m_Window); }

private:
	void InitWindow();
	void InitRHI();
	void InitScene();
	void InitInput();
	void InitGameObject();
};