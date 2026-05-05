#pragma once

#include <EnTT/meta/meta.hpp>

DECLARE_LOG_CATEGORY_EXTERN(LogRegisterMeta);

namespace MirageEngine
{
	class RegisterMeta
	{
	public:
		RegisterMeta();
		~RegisterMeta();

		void RegisterAllMeta();

	private:
		void RegisterGameObjectMeta();

		void RegisterTransformMeta();

		void RegisterMeshRendererMeta();
		void RegisterCameraMeta();
		void RegisterInputMappingMeta();
		void RegisterInfluenceGridMeta();
		void RegisterInfluenceEmitterMeta();

		void RegisterMeshMeta();
		void RegisterTextureMeta();
		void RegisterMaterialMeta();
		void RegisterShaderMeta();

		template<typename T>
		void RegisterSharedPtr();

	};
}

#include "register_meta.inl"