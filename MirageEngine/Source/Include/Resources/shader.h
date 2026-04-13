#pragma once
#include <Resources/resource_interface.h>
#include <RHI/i_shader.h>

namespace MirageRHI
{
	class IShader;
}

namespace MirageEngine::Core::Resources
{
	struct Shader : public IResource
	{
	public:
		MirageRHI::IShader* ShaderBind = nullptr;

		Shader() = default;
		~Shader()
		{
			if (ShaderBind)
				ShaderBind->Destroy();
		}
	};
}