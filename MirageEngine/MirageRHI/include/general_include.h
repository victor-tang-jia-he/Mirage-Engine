#pragma once

// GLFW configuration
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Standard library headers
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <optional>
#include <set>
#include <fstream>
#include <array>
#include <chrono>

#include "debug.h"

//struct Vertex
//{
//	glm::vec3 pos;
//	glm::vec3 color;
//	glm::vec2 texCoord;
//};

namespace MirageRHI
{
	class IRenderInterface;
	class IInstance;
	class ISurface;
	class IDevice;
	class ISwapChain;
	class IRenderPass;
	class IPipeline;
	class IFramebuffer;
	class ITexture;
	class IBuffer;
	class IUniformBuffer;
	class IDescriptors;
	class ICommandBuffer;

	class OpenGLRenderInterface;
	class OpenGLInstance;
	class OpenGLSurface;
	class OpenGLDevice;
	class OpenGLSwapChain;
	class OpenGLRenderPass;
	class OpenGLPipeline;
	class OpenGLFramebuffer;
	class OpenGLTexture;
	class OpenGLBuffer;
	class OpenGLUniformBuffer;
	class OpenGLDescriptors;
	class OpenGLCommandBuffer;
}