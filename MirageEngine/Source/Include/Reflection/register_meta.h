#pragma once

#include <EnTT/meta/meta.hpp>

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

		void RegisterMeshMeta();
		void RegisterTextureMeta();
		void RegisterMaterialMeta();
		void RegisterShaderMeta();

		void RegisterInfluenceGridMeta();
		void RegisterInfluenceEmitterMeta();
		void RegisterResponsiveCurveMeta();
		void RegisterNavAgentMeta();
		void RegisterUtilityAgentMeta();

		template<typename T>
		void RegisterSharedPtr();

	};
}

#include "register_meta.inl"