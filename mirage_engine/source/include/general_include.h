#pragma once

// GLFW configuration
#include <glad/glad.h>
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

#include "vector2.h"
#include "vector3.h"

#include "debug.h"

using namespace Bwt::Core::Maths;
struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};

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