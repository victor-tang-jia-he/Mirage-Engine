#pragma once

#include <EnTT/meta/meta.hpp>

namespace MirageEngine
{
	class RegisterMeta
	{
		// Variables
	public:
	protected:
	private:

		// Methods
	public:
		RegisterMeta();
		~RegisterMeta();

		void RegisterAllMeta();
		void RegisterMeshRendererMeta();
		void RegisterMeshMeta();
		void RegisterCameraMeta();
		void RegisterInputMappingMeta();

		template<typename T>
		void RegisterSharedPtr();

	protected:
	private:
		void RegisterGameObjectMeta();
		void RegisterTransformMeta();

		void RegisterMaterialMeta();
		void RegisterShaderMeta();
	};
}

#include "register_meta.inl"