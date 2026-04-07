#pragma once

#include <vector>

#include <renderer.h>
#include <SceneGraph/scene_graph.h>

// Input
#include <Components/input_system.h>
namespace MirageEngine 
{
	namespace Core 
	{
		namespace Components 
		{
			class Camera;
			class MeshRenderer;
		}
	}
}

class Application
{
public:
	MirageEngine::Renderer m_Renderer;
	MirageEngine::SceneGraph m_SceneGraph;

private:

	// TODO : Edit in map
	MirageEngine::Core::Input::InputMapping* m_EditorMapping = nullptr;
	MirageEngine::Core::Input::InputMapping* m_PlayMapping = nullptr;

	MirageEngine::Core::Components::Camera* m_EngineCamera = nullptr;
	std::vector < std::shared_ptr < MirageEngine::Core::Components::MeshRenderer>> m_meshRenderer;

public:
	void InitApp();
	void ShutDown();

private:
	void InitScene();
	void InitInput();
	void InitGameObject();
};