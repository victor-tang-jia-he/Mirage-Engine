#pragma once

#include "i_render_interface.h"

#include "openGL_instance.h"
#include "openGL_surface.h"
#include "openGL_device.h"
#include "openGL_swap_chain.h"
#include "openGL_render_pass.h"
#include "openGL_pipeline.h"
#include "openGL_framebuffer.h"
#include "openGL_buffer.h"
#include "openGL_command_buffer.h"

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
		IFramebuffer* InstantiateFramebuffer() override { return new OpenGLFramebuffer(); }
		IBuffer* InstantiateBuffer() override { return new OpenGLBuffer(); }
		ICommandBuffer* InstantiateCommandBuffer() override { return new OpenGLCommandBuffer(); }

		OpenGLRenderInterface& API_OpenGL() override { return *this; }
	};
}