#pragma once

#include <vector>

#include <i_render_interface.h>

#include "openGL_instance.h"
#include "openGL_surface.h"
#include "openGL_device.h"
#include "openGL_swap_chain.h"
#include "openGL_render_pass.h"
#include "openGL_pipeline.h"
#include "openGL_pipeline_parameters.h"
#include "openGL_shader.h"
#include "openGL_framebuffer.h"
#include "openGL_buffer.h"
#include "openGL_texture.h"
#include "openGL_command_buffer.h"
#include "openGL_uniform_buffer.h"

namespace MirageRHI
{
	class OpenGLRenderInterface : public IRenderInterface
	{
	public:
		IInstance* InstantiateInstance() override { return new OpenGLInstance(); }
		ISurface* InstantiateSurface() override { return new OpenGLSurface(); }
		IDevice* InstantiateDevice() override { return new OpenGLDevice(); }
		ISwapChain* InstantiateSwapChain() override { return new OpenGLSwapChain(); }
		IRenderPass* InstantiateRenderPass() override { return new OpenGLRenderPass(); }
		IPipeline* InstantiatePipeline() override { return new OpenGLPipeline(); }
		IPipelineParameters* InstantiatePipelineParameters() override { return new OpenGLPipelineParameters(); }
		IShader* InstantiateShader() override { return new OpenGLShader(); }
		IFramebuffer* InstantiateFramebuffer() override { return new OpenGLFramebuffer(); }
		IBuffer* InstantiateBuffer() override { return new OpenGLBuffer(); }
		ITexture* InstantiateTexture() override { return new OpenGLTexture(); }
		IUniformBuffer* InstantiateUniformBuffer() override { return new OpenGLUniformBuffer(); }
		ICommandBuffer* InstantiateCommandBuffer() override { return new OpenGLCommandBuffer(); }

		OpenGLRenderInterface& API_OpenGL() override { return *this; }
	};
}