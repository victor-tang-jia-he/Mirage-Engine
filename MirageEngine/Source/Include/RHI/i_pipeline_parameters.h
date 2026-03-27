#pragma once

#include <iostream>

#include <vector3.h>
#include <vector2.h>

namespace MirageRHI
{
	struct OpenGLPipelineParameters;

	enum class VertexType
	{
		None,
		Simple,
		PBR
	};

	enum class CullingType
	{
		None,
		Front,
		Back
	};

	struct UnlitVertex
	{
		MirageEngine::Core::Math::Vector3 Position;
		MirageEngine::Core::Math::Vector3 Color;
	};

	struct LitVertex
	{
		MirageEngine::Core::Math::Vector3 Position;
		MirageEngine::Core::Math::Vector3 Normal;
		MirageEngine::Core::Math::Vector2 UV;
	};

	struct IPipelineParameters
	{
	public:
		virtual void Create(VertexType _vertexType, CullingType _cullingType, bool _depthTest) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLPipelineParameters& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipelineParameters"); }
	};
}