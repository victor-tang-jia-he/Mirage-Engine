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
#include <input_system.h>


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
	IShader* m_VertexShader = nullptr;
	IShader* m_FragmentShader = nullptr;
	IPipelineParameters* m_PipelineParameters = nullptr;
	IPipeline* m_Pipeline = nullptr;
	IFramebuffer* m_Framebuffer = nullptr;
	IBuffer* m_BufferTriangleVertices = nullptr;
	IBuffer* m_BufferTriangleIndices = nullptr;
	ICommandBuffer* m_CommandBuffer = nullptr;

	// TODO : Change to a list or mapped list (Mesh)
	std::vector<LitVertex> m_VerticesTriangle;
	std::vector<uint32_t> m_IndicesTriangle;
	uint32_t m_IndicesCount = 0;

public:
	Application(IRenderInterface::API _api = IRenderInterface::API::OPENGL) : m_API(_api) {}

	void InitApp();
	void RunMainLoop();
	void ShutDown();

	bool IsRunning() const;
	void BeginFrame();
	void RenderScene();
	void EndFrame();

	void* GetWindowHandle() const { return static_cast<void*>(m_Window); }

private:
	void InitWindow();
	void InitRHI();
	void InitScene(); // TODO
	void InitInput();
	void PrintInput(MirageEngine::Core::Input::CallbackInputInfo info); // debug
};