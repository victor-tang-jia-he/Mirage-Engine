#pragma once

#include <iostream>
#include <vector3.h>
#include <vector2.h>

using namespace MirageEngine::Core::Maths;

namespace MirageRHI
{
	struct OpenGLPipelineParameters;

	enum class VertexType
	{
		Simple,
		PBR
	};

	enum class CullingType
	{
		None,
		Front,
		Back,
		Front_And_Back
	};

	struct UnlitVertex
	{
		Vector3 Position;
		Vector3 Color;
	};

	struct LitVertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 UV;
	};

	struct IPipelineParameters
	{
	public:
		virtual void Create(VertexType _vertexType = VertexType::Simple, CullingType _cullingType = CullingType::Front, bool _depthTest = true) = 0;
		virtual void Destroy() = 0;

		virtual OpenGLPipelineParameters& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipelineParameters"); }
	};
}