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
		void RegisterCameraMeta();
		void RegisterInputMappingMeta();
		void RegisterInfluenceGridMeta();

	protected:
	private:
		void RegisterGameObjectMeta();
		void RegisterTransformMeta();
	};
}