#pragma once

#include <iostream>

namespace MirageRHI
{
    class OpenGLRenderInterface;
    class IInstance;
    class ISurface;
    class IDevice;
    class ISwapChain;
    class IRenderPass;
    class IPipeline;
    struct IPipelineParameters;
    class IVertexLayout;
    class IShader;
    class IFramebuffer;
    class IBuffer;
    class ITexture;
    class ICommandBuffer;
    class IUniformBuffer;

    class IRenderInterface
    {
        public:

            enum class API
            {
                OPENGL
            };

            virtual ~IRenderInterface() = default;

            virtual OpenGLRenderInterface& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLRenderInterface"); }

            virtual std::shared_ptr<IInstance> InstantiateInstance() = 0;
            virtual std::shared_ptr<ISurface> InstantiateSurface() = 0;
            virtual std::shared_ptr<IDevice> InstantiateDevice() = 0;
            virtual std::shared_ptr<ISwapChain> InstantiateSwapChain() = 0;
            virtual std::shared_ptr<IRenderPass> InstantiateRenderPass() = 0;
            virtual std::shared_ptr<IPipeline> InstantiatePipeline() = 0;
            virtual std::shared_ptr<IPipelineParameters> InstantiatePipelineParameters() = 0;
            virtual std::shared_ptr<IVertexLayout> InstantiateVertexLayout() = 0;
            virtual std::shared_ptr<IShader> InstantiateShader() = 0;
            virtual std::shared_ptr<IFramebuffer> InstantiateFramebuffer() = 0;
            virtual std::shared_ptr<IBuffer> InstantiateBuffer() = 0;
            virtual std::shared_ptr<ITexture> InstantiateTexture() = 0;
            virtual std::shared_ptr<IUniformBuffer> InstantiateUniformBuffer() = 0;
            virtual std::shared_ptr<ICommandBuffer> InstantiateCommandBuffer() = 0;
    };
} // namespace MirageRHI
