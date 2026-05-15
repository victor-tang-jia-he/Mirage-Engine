#pragma once

#include <i_render_interface.h>

#include "openGL_instance.h"
#include "openGL_surface.h"
#include "openGL_device.h"
#include "openGL_swap_chain.h"
#include "openGL_render_pass.h"
#include "openGL_pipeline.h"
#include "openGL_pipeline_parameters.h"
#include "openGL_vertex_layout.h"
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
        std::shared_ptr<IInstance> InstantiateInstance() override { return std::make_shared<OpenGLInstance>(); }
        std::shared_ptr<ISurface> InstantiateSurface() override { return std::make_shared<OpenGLSurface>(); }
        std::shared_ptr<IDevice> InstantiateDevice() override { return std::make_shared<OpenGLDevice>(); }
        std::shared_ptr<ISwapChain> InstantiateSwapChain() override { return std::make_shared<OpenGLSwapChain>(); }
        std::shared_ptr<IRenderPass> InstantiateRenderPass() override { return std::make_shared<OpenGLRenderPass>(); }
        std::shared_ptr<IPipeline> InstantiatePipeline() override { return std::make_shared<OpenGLPipeline>(); }
        std::shared_ptr<IPipelineParameters> InstantiatePipelineParameters() override { return std::make_shared<OpenGLPipelineParameters>(); }
        std::shared_ptr<IVertexLayout> InstantiateVertexLayout() override { return std::make_shared<OpenGLVertexLayout>(); }
        std::shared_ptr<IShader> InstantiateShader() override { return std::make_shared<OpenGLShader>(); }
        std::shared_ptr<IFramebuffer> InstantiateFramebuffer() override { return std::make_shared<OpenGLFramebuffer>(); }
        std::shared_ptr<IBuffer> InstantiateBuffer() override { return std::make_shared<OpenGLBuffer>(); }
        std::shared_ptr<ITexture> InstantiateTexture() override { return std::make_shared<OpenGLTexture>(); }
        std::shared_ptr<IUniformBuffer> InstantiateUniformBuffer() override { return std::make_shared<OpenGLUniformBuffer>(); }
        std::shared_ptr<ICommandBuffer> InstantiateCommandBuffer() override { return std::make_shared<OpenGLCommandBuffer>(); }

        OpenGLRenderInterface& API_OpenGL() override { return *this; }

    };
} // namespace MirageRHI
