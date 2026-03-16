#include "general_include.h"

#include "OpenGL/openGL_render_interface.h"

#include "i_instance.h"
#include "i_surface.h"
#include "i_device.h"
#include "i_swap_chain.h"
#include "i_render_pass.h"
#include "i_pipeline.h"
#include "i_shader.h"
#include "i_framebuffer.h"
#include "i_buffer.h"
#include "i_uniform_buffer.h"
#include "i_command_buffer.h"

int main(MirageRHI::IRenderInterface::API api)
{
	api = MirageRHI::IRenderInterface::API::OPENGL;

	MirageRHI::IRenderInterface* renderInterface = nullptr;

	MirageRHI::IInstance* instance = nullptr;
	MirageRHI::ISurface* surface = nullptr;
	MirageRHI::IDevice* device = nullptr;
	MirageRHI::ISwapChain* swapChain = nullptr;
	MirageRHI::IRenderPass* renderPass = nullptr;
	MirageRHI::IShader* vertexShader = nullptr;
	MirageRHI::IShader* fragmentShader = nullptr;
	MirageRHI::IPipeline* pipeline = nullptr;
	MirageRHI::IBuffer* VAO = nullptr;
	MirageRHI::IFramebuffer* framebuffer = nullptr;
	MirageRHI::IBuffer* bufferTriangleVertices = nullptr;
	MirageRHI::IBuffer* bufferTriangleIndices = nullptr;
	MirageRHI::ICommandBuffer* commandBuffer = nullptr;

	std::vector<Vertex> verticesTriangle;
	std::vector<uint32_t> indicesTriangle;

	if (!glfwInit()) {
		DEBUG_THROW("Failed to init GLFW");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 1920;
	int height = 1080;

	GLFWwindow* window = glfwCreateWindow(width, height, "Mirage Engine", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		DEBUG_THROW("Failed to create window");
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DEBUG_THROW("Failed to init GLAD\n");
	}

	glViewport(0, 0, width, height);

	// Select RHI API
	switch (api)
	{
	case MirageRHI::IRenderInterface::OPENGL:
		renderInterface = new MirageRHI::OpenGLRenderInterface();
		break;
	default:
		DEBUG_THROW("Unsupported RHI API");
	}
	DEBUG_LOG("Render Interface Created");

	// Instantiate All Classes
	instance = renderInterface->InstantiateInstance();
	surface = renderInterface->InstantiateSurface();
	device = renderInterface->InstantiateDevice();
	swapChain = renderInterface->InstantiateSwapChain();
	renderPass = renderInterface->InstantiateRenderPass();
	vertexShader = renderInterface->InstantiateShader();
	fragmentShader = renderInterface->InstantiateShader();
	pipeline = renderInterface->InstantiatePipeline();
	VAO = renderInterface->InstantiateBuffer();
	framebuffer = renderInterface->InstantiateFramebuffer();
	bufferTriangleVertices = renderInterface->InstantiateBuffer();
	bufferTriangleIndices = renderInterface->InstantiateBuffer();
	commandBuffer = renderInterface->InstantiateCommandBuffer();
	DEBUG_LOG("All Classes Instantiated");

	verticesTriangle = // Set Vertices to draw Triangle
	{
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
	};
	//uint32_t verticesTriangleListSize = (uint32_t)(sizeof(verticesTriangle[0]) * verticesTriangle.size());

	indicesTriangle = // Set Indexes to draw Triangle
	{
		0, 1, 2
	};
	uint32_t indicesTriangleListSize = (uint32_t)(sizeof(indicesTriangle[0]) * indicesTriangle.size());
	DEBUG_LOG("Vertex and Index Data Created");

	// Create All Classes
	instance->Create("Mirage");
	surface->Create(window, width, height);
	device->Create();
	swapChain->Create();
	renderPass->Create(Vector4(0.2f, 0, 0.1f, 1.0f), 1.0f);
	vertexShader->Create("assets/shaders/vertex.glsl", MirageRHI::IShader::ShaderType::Vertex);
	fragmentShader->Create("assets/shaders/fragment.glsl", MirageRHI::IShader::ShaderType::Fragment);
	pipeline->Create(vertexShader, fragmentShader);
	VAO->Create(NULL, 0, MirageRHI::IBuffer::BufferType::VertexAttributes);
	bufferTriangleVertices->Create(verticesTriangle.data(), verticesTriangle.size() * sizeof(Vertex), MirageRHI::IBuffer::BufferType::Vertices);
	bufferTriangleIndices->Create(indicesTriangle.data(), indicesTriangle.size() * sizeof(Vertex), MirageRHI::IBuffer::BufferType::Indices);
	commandBuffer->Create();
	DEBUG_LOG("All Classes Created\n");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// Setup to Draw
		commandBuffer->BeginRenderPass(renderPass);
		commandBuffer->BeginDraw();
		commandBuffer->BindPipeline(pipeline);
		commandBuffer->BindBuffers(bufferTriangleIndices, bufferTriangleVertices, VAO);
		Matrix4 model = Matrix4::CreateTranslationMatrix(Vector3(0, 0, -10));
		Matrix4 view = Matrix4::Identity();
		Matrix4 projection = Matrix4::GetPerspectiveMatrix(width, height, 0.1f, 100.f, 45.0f);
		projection = projection * view * model;
		pipeline->API_OpenGL().SetVec3("viewPos", Vector3());
		pipeline->API_OpenGL().SetMat4("pvm", projection);
		pipeline->API_OpenGL().SetMat4("view", view);
		pipeline->API_OpenGL().SetMat4("model", model);
		pipeline->API_OpenGL().SetBool("debugNoLight", true);
		commandBuffer->DrawIndexed(indicesTriangleListSize);
		commandBuffer->EndDraw();
		commandBuffer->EndRenderPass();
		commandBuffer->SubmitAndPresent(swapChain, surface);

		glfwPollEvents();
	}
	glfwMakeContextCurrent(window);

	glfwTerminate();
	return 0;
}