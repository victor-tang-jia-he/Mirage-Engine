#pragma once

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

	protected:
	private:
		void RegisterGameObjectMeta();
		void RegisterTransformMeta();

		template<typename T>
		void RegisterSharedPtr();
	};
}