#pragma once
#include <Resources/resource_interface.h>
#include <RHI/i_shader.h>

namespace MirageRHI
{
	class IShader;
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Resources
		{
			struct Shader : public IResource
			{
				MirageRHI::IShader* ShaderBind = nullptr;

				Shader() = default;
			};
		}
	}
}